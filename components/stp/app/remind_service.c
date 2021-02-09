#include <string.h>

#include <device.h>
#include <net/buf.h>
#include <logging/log.h>
LOG_MODULE_DECLARE(stp_app, CONFIG_STP_APP_LOG_LEVEL);

#include "stp/opc_proto.h"
#include "stp/stp_core.h"

#define OBSERVER_CLASS_DEFINE
#include "remind_service.h"


static void message_remind_service(const void *buf, size_t size,
    struct net_buf *obuf)
{
    uint8_t resp[2] = {MESSAGE_REMIND, };
    Remind__Message *msg;

    msg = remind__message__unpack(NULL, size, buf);
    if (msg == NULL) {
        LOG_ERR("%s(): Receive text message failed\n", __func__);
        resp[1] = 0x1;
        goto _resp;
    }
    
    /* Dump information */
    LOG_INF("Time:%d\n", msg->timestamp);
    LOG_INF("Name:%s\n", msg->people);
    LOG_INF("Phone:%s\n", msg->phone);
    LOG_INF("Type:%d\n", msg->type);
    LOG_INF("Text:%s\n", msg->text);

    /* Notify */
    remind_notify(MESSAGE_REMIND, msg);

    /* Free buffer */
    remind__message__free_unpacked(msg, NULL);
    resp[1] = 0x00;
_resp:
    net_buf_add_mem(obuf, resp, sizeof(resp));
}
STP_SERVICE(remind, OPC_CLASS_REMIND, MESSAGE_REMIND, message_remind_service);

static void calling_remind_service(const void *buf, size_t size,
    struct net_buf *obuf)
{
    uint8_t resp[2] = {CALLING_REMIND, };
    Remind__Call *call;

    call = remind__call__unpack(NULL, size, buf);
    if (call == NULL) {
        LOG_ERR("%s(): Receive calling message failed\n", __func__);
        resp[1] = 0x2;
        goto _resp;
    }

    /* Dump information */
    LOG_INF("Time:%d\n", call->timestamp);
    LOG_INF("Name:%s\n", call->people);
    LOG_INF("Phone:%s\n", call->phone);
    
    /* Notify */
    remind_notify(CALLING_REMIND, call);

    /* Free buffer */
    remind__call__free_unpacked(call, NULL);
    resp[1] = 0x00;
_resp:
    net_buf_add_mem(obuf, resp, sizeof(resp));
}
STP_SERVICE(remind, OPC_CLASS_REMIND, CALLING_REMIND, calling_remind_service);

