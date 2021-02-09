#include <string.h>

#include <device.h>
#include <net/buf.h>
#include <logging/log.h>
LOG_MODULE_DECLARE(stp_app, CONFIG_STP_APP_LOG_LEVEL);

#include "stp/opc_proto.h"
#include "stp/stp_core.h"

#define OBSERVER_CLASS_DEFINE
#include "setting_service.h"


static void time_update_service(const void *buf, size_t size,
    struct net_buf *obuf)
{
    uint8_t resp[2] = {SET_TIME, };
    Setting__Time *time;
       
    time = setting__time__unpack(NULL, size, buf);
    if (time == NULL) {
        LOG_ERR("%s(): Unpack data failed\n", __func__);
        resp[1] = 0x1;
        goto _resp;
    }

    /* Notify */
    setting_notify(SET_TIME, time);

    /* Free buffer */
    setting__time__free_unpacked(time, NULL);
    resp[1] = 0x00;
_resp:
    net_buf_add_mem(obuf, resp, sizeof(resp));
}
STP_SERVICE(setting, OPC_CLASS_SETTING, SET_TIME, time_update_service);

