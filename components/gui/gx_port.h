#ifndef LIB_GX_PORT_H_
#define LIB_GX_PORT_H_

#include <stdint.h>
#include "guix_zephyr_notify.h"

typedef INT    GX_BOOL;
typedef SHORT  GX_VALUE;
#define GX_VALUE_MAX                        0x7FFF


/* Define the basic system parameters.  */
#ifndef GX_THREAD_STACK_SIZE
#define GX_THREAD_STACK_SIZE                (6 * 1024)
#endif

#ifndef GX_TIMER_THREAD_STACK_SIZE
#define GX_TIMER_THREAD_STACK_SIZE          (4 * 1024)
#endif

#ifndef GX_TICKS_SECOND
#define GX_TICKS_SECOND                     20
#endif

#define GX_CONST                            const
#define GX_INCLUDE_DEFAULT_COLORS
#define GX_MAX_ACTIVE_TIMERS                32
#define GX_SYSTEM_TIMER_MS                  10   /* ms */
#define GX_SYSTEM_TIMER_TICKS               2           
#define GX_MAX_VIEWS                        32
#define GX_MAX_DISPLAY_HEIGHT               800
#define GX_SYSTEM_THREAD_PRIORITY           9

/* Override define */
#define GX_CALLER_CHECKING_EXTERNS
#define GX_THREADS_ONLY_CALLER_CHECKING
#define GX_INIT_AND_THREADS_CALLER_CHECKING
#define GX_NOT_ISR_CALLER_CHECKING
#define GX_THREAD_WAIT_CALLER_CHECKING

#ifdef __cplusplus
extern "C"{
#endif

struct GX_DISPLAY_STRUCT;

#ifdef CONFIG_GUI_SPLIT_BINRES
struct GX_THEME_STRUCT;
struct GX_STRING_STRUCT;
#endif

struct guix_driver {
    UINT (*setup)(struct GX_DISPLAY_STRUCT *display);
    struct guix_driver *next;
    UINT id;
#ifdef CONFIG_GUI_SPLIT_BINRES
    VOID (*mmap)(VOID);
    VOID (*unmap)(VOID);
    void *map_base;
#endif
};

#ifdef CONFIG_GUI_SPLIT_BINRES
UINT guix_binres_load(struct guix_driver *drv, INT theme_id, 
    struct GX_THEME_STRUCT **theme, struct GX_STRING_STRUCT ***language);
#endif
UINT guix_main(UINT disp_id, struct guix_driver *drv);

int guix_driver_register(struct guix_driver *drv);
        
#ifdef __cplusplus
}
#endif
#endif /* LIB_GX_PORT_H_ */

