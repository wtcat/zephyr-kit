#include <zephyr.h>

#include <string.h>

#include "stp/stp.h"
#include "stp/stp_core.h"
#include "stp/opc_proto.h"

#include <logging/log.h>
LOG_MODULE_REGISTER(test);


#if CONFIG_STP
static int msg_send_test(int major, int minor, 
    const char *info, size_t len)
{
    struct stp_chan *chan = opc_chan_get(major);
    struct opc_subhdr *hdr = opc_alloc(len);
    struct stp_bufv v[1];
    
    hdr->minor = minor;
    hdr->len = ltons(len);
    memcpy(hdr->data, info, len);

    v[0].buf = hdr;
    v[0].len = sizeof(struct opc_subhdr) + len;
    return stp_sendmsgs(chan, v, 1);
}

static int msgs_send_test(void)
{
    struct stp_chan *chan = opc_chan_get(OPC_CLASS_OTA);
    struct opc_subhdr *hdr_1, *hdr_2, *hdr_3;
    struct stp_bufv v[3];

    hdr_1 = opc_alloc(7 + sizeof(*hdr_1));
    hdr_1->minor = 0x01;
    hdr_1->len = ltons(7);
    memcpy(hdr_1->data, "Thanks", 7);
    v[0].buf = hdr_1;
    v[0].len = sizeof(struct opc_subhdr) + 7;

    hdr_2 = opc_alloc(11 + sizeof(*hdr_2));
    hdr_2->minor = 0x02;
    hdr_2->len = ltons(11);
    memcpy(hdr_2->data, "Don't move", 11);
    v[1].buf = hdr_2;
    v[1].len = sizeof(struct opc_subhdr) + 11;


    hdr_3 = opc_alloc(4 + sizeof(*hdr_3));
    hdr_3->minor = 0x03;
    hdr_3->len = ltons(4);
    memcpy(hdr_3->data, "GO!", 4);
    v[2].buf = hdr_3;
    v[2].len = sizeof(struct opc_subhdr) + 4;

    return stp_sendmsgs(chan, v, ARRAY_SIZE(v));
}

static void action_ota(const void *buf, size_t size, 
    struct net_buf *outbuf)
{
    char temp[32];
    int len;

    len = size < 31? size: 30;
    memset(temp, 0, 32);
    memcpy(temp, buf, len);
    //LOG_ERR("%s\n", temp);
}

STP_SERVICE(ota_1, 0x03, 0x01, action_ota);
STP_SERVICE(ota_2, 0x03, 0x02, action_ota);
#endif 






