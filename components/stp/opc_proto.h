#ifndef OPC_PROTO_H_
#define OPC_PROTO_H_

#include <stdint.h>
#include <sys/rb.h>
#include <sys/__assert.h>
#include <toolchain.h>

#include "stp/stp.h"

#ifdef __cplusplus
extern "C"{
#endif

#define MAX_OPC_HEADER (sizeof(struct opc_hdr) + \
                         sizeof(struct opc_subhdr) + \
                         sizeof(struct opc_filehdr))

struct net_buf;

struct opc_hdr {
    uint8_t major;
} __packed;

struct opc_subhdr {
    uint8_t minor;
    uint16_t len;
    uint8_t data[];
}__packed;

struct opc_filehdr {
    uint16_t maxno;
    uint16_t seqno;
    uint8_t data[];
}__packed;

struct opc_chan {
    struct stp_chan base;
    uint8_t major;
    uint8_t flags;
};

struct opc_service_node {
#ifdef CONFIG_STP_RBTREE
    struct rbnode node;
#endif
    void (*action)(const void *buf, size_t size, 
        struct net_buf *outbuf);
    uint16_t opcode;
};

#define _OPC_NAME(n1, n2, n3) __opc_var_##n1##_##n2##_##n3
    
#define _STP_SERVICE(_name, _major, _minor, _action) \
    __in_section(stp_node_, _major, _minor) __used static const \
    struct opc_service_node _OPC_NAME(_name, _major, _minor) = { \
        .action = _action, \
        .opcode = (uint16_t)OPC_CODE(_major, _minor) \
    }
    
#ifdef CONFIG_STP_RBTREE
#define STP_SERVICE(_name, _major, _minor, _action) \
    _STP_SERVICE(_name, _major, _minor, _action)
    
#else /* !CONFIG_STP_RBTREE */
#define STP_SERVICE(_name, _major, _minor, _action) \
    _STP_SERVICE(_name, _major, _minor, _action)

#endif /* CONFIG_STP_RBTREE */

#define OPCF_REQ      0x01
#define OPCF_NOACK    0x02
#define OPC_CODE(_m, _n) (((uint16_t)(_m) << 8) | (_n))


/*
 * OPC class
 */
#define OPC_CLASS_OTA       0x01
#define OPC_CLASS_INFO      0x02
#define OPC_CLASS_SETTING   0x03
#define OPC_CLASS_REMIND    0x08
#define OPC_CLASS_LAST 0x10



#define opc_alloc(size) __builtin_alloca(size)

extern const struct opc_chan opc_chan_aggregation[];
static inline struct stp_chan *opc_chan_get(int type)
{
    __ASSERT_NO_MSG(type < OPC_CLASS_LAST);
    return (struct stp_chan *)&opc_chan_aggregation[type].base;
}

#ifdef __cplusplus
}
#endif
#endif /* OPC_PROTO_H_ */

