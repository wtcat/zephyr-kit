#ifndef BASE_OBSERVER_CLASS_H_
#define BASE_OBSERVER_CLASS_H_

#ifdef __cplusplus
extern "C"{
#endif

#include "base/observer.h"

#ifdef OBSERVER_CLASS_DEFINE
#define _OBSERVER_CLASS
#else
#define _OBSERVER_CLASS extern
#endif

/*
 * method:
 *  int _class_name##_notify(unsigned long action, void *ptr)
 *  int _class_name##_add_observer(struct observer_base *obs)
 *  int _class_name##_remove_observer(struct observer_base *obs)
 */
#define OBSERVER_CLASS_DECLARE(_class_name) \
_OBSERVER_CLASS struct observer_base *__##_class_name##_obs_list_head; \
static inline int _class_name##_notify(unsigned long value, void *ptr) { \
    return observer_notify(&__##_class_name##_obs_list_head, value, ptr); \
} \
static inline int _class_name##_add_observer(struct observer_base *obs) { \
    return observer_cond_register(&__##_class_name##_obs_list_head, obs); \
} \
static inline int _class_name##_remove_observer(struct observer_base *obs) { \
    return observer_unregister(&__##_class_name##_obs_list_head, obs); \
}

#ifdef __cplusplus
}
#endif
#endif /* STP_PROTO_OBS_H_ */

