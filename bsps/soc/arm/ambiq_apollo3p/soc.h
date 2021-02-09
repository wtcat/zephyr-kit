#ifndef _APOLLO3P_SOC_H_
#define _APOLLO3P_SOC_H_

#include <sys/util.h>

#ifndef _ASMLANGUAGE

#include <apollo3p.h>
#include <am_mcu_apollo.h>
#include <am_util_delay.h>

#if defined(CONFIG_BT) || defined(CONFIG_BLE_COMPONENT)
#include <am_util_ble.h>
#endif

/* Add include for DTS generated information */
#include <devicetree.h>

#endif /* !_ASMLANGUAGE */

#endif /* _STM32F4_SOC_H_ */
