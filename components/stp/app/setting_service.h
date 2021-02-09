#ifndef STP_APP_SETTING_SERVICE_H_
#define STP_APP_SETTING_SERVICE_H_

#include "base/observer_class.h"
#ifdef CONFIG_PROTOBUF
#include "stp/proto/setting.pb-c.h"
#endif

#ifdef __cplusplus
extern "C"{
#endif


/*
 * Setting minor number 
 */
#define SET_TIME 0x01


/*
 * method:
 *  int setting_notify(unsigned long action, void *ptr)
 *  int setting_add_observer(struct observer_base *obs)
 *  int setting_remove_observer(struct observer_base *obs)
 */
OBSERVER_CLASS_DECLARE(setting)
    

#ifdef __cplusplus
}
#endif
#endif /* STP_APP_SETTING_SERVICE_H_ */
