#ifndef __HOME_WINDOW_H__
#define __HOME_WINDOW_H__
#include "gx_api.h"
#include "stdbool.h"
extern GX_ANIMATION status_menu_animation;
extern GX_ANIMATION slide_animation;
UINT home_window_event(GX_WINDOW *window, GX_EVENT *event_ptr);
bool home_window_is_pen_down(void);

void home_window_init(GX_WIDGET *home);
#endif