/* This is a small demo of the high-performance GUIX graphics framework. */

#include "device.h"
#include "gx_api.h"
#include <stdint.h>
#include <stdio.h>
#include <sys/_timespec.h>
#include <time.h>

#include "guix_simple_resources.h"
#include "guix_simple_specifications.h"
#include "gx_widget.h"

#include "custom_pixelmap_mirror.h"
#include "drivers/counter.h"
#include "drivers_ext/rtc.h"
#include "init.h"
#include "soc.h"
#include "sys/time_units.h"

#include "alarm_list_ctl.h"
#include "home_window.h"
#include "posix/time.h"
#include "watch_face_manager.h"
#include "wf_list_window.h"
#include "wf_window.h"

#include "app_list_style_grid_view.h"
#include "app_list_window.h"
#include "app_compass_window.h"

#include "control_center_window.h"
#include <sys/timeutil.h>

#define MIRROR_UPDATE_EVERY_REFRESH 1

GX_WINDOW *p_main_screen;
GX_WINDOW_ROOT *root;
GX_WIDGET *current_screen;

UINT guix_main(UINT disp_id, struct guix_driver *drv)
{
	// get ticks from rtc counter
	const struct device *rtc = device_get_binding("apollo3p_rtc");
	int ticks;

	// sync rtc ticks to system time
	if (rtc) {
		counter_get_value(rtc, &ticks);
		uint64_t us_cnt = counter_ticks_to_us(rtc, ticks);

		// convert tick to timespec,then set to posix real time
		struct timespec time;
		time.tv_sec = us_cnt / 1000000UL;
		time.tv_nsec = 0;
		clock_settime(CLOCK_REALTIME, &time);
	}

	gx_system_initialize(); // Initialize GUIX.
	gx_studio_display_configure(HONBOW_DISP, drv->setup, 0, HONBOW_DISP_THEME_1, &root);

	gx_studio_named_widget_create("root_window", (GX_WIDGET *)root, (GX_WIDGET **)&p_main_screen);
	current_screen = (GX_WIDGET *)&root_window.root_window_home_window;
	gx_widget_event_process_set((GX_WIDGET *)&root_window.root_window_home_window, home_window_event);
	home_window_init((GX_WIDGET *)&root_window.root_window_home_window);

	gx_widget_draw_set(&root_window.root_window_wf_window, wf_window_draw);
	gx_widget_event_process_set(&root_window.root_window_wf_window, wf_window_event);

	gx_studio_named_widget_create("control_center_window", GX_NULL, GX_NULL);
	control_center_window_init((GX_WIDGET *)&control_center_window);
	gx_widget_event_process_set((GX_WIDGET *)&control_center_window, control_center_window_event);

	gx_studio_named_widget_create("notify_center_window", GX_NULL, GX_NULL);
	gx_widget_fill_color_set(&notify_center_window, GX_COLOR_ID_WHITE, GX_COLOR_ID_WHITE, GX_COLOR_ID_WHITE);

	gx_studio_named_widget_create("app_compass_window", GX_NULL, GX_NULL);

	gx_studio_named_widget_create("app_list_window", GX_NULL, GX_NULL);
	gx_widget_event_process_set(&app_list_window, app_list_window_event);
	gx_widget_draw_set(&app_list_window, app_list_window_draw);
	app_list_style_grid_view_init((GX_WINDOW *)&app_list_window);

	gx_widget_draw_set(&app_compass_window, app_compass_window_draw);
	gx_widget_event_process_set(&app_compass_window, app_compass_window_event);

	wf_mgr_init(drv);
	uint8_t id_default = wf_mgr_get_default_id();
	GX_WIDGET *wf_widget = (GX_WIDGET *)&root_window.root_window_wf_window;
	if (0 != wf_mgr_theme_select(id_default, wf_widget)) {
		wf_mgr_theme_select(WF_THEME_DEFAULT_ID, wf_widget);
	}

	gx_studio_named_widget_create("wf_list", GX_NULL, GX_NULL);
	wf_list_children_create((GX_HORIZONTAL_LIST *)&wf_list);

#if 0
	//alarm_list_ctl_init();
	alarm_list_children_create(&alarm_window.alarm_window_alarm_list);
	extern void ui_alarm_callback(void *data);
	alarm_list_ctl_reg_callback(ui_alarm_callback);
#endif

	gx_animation_create(&slide_animation);
	gx_animation_landing_speed_set(&slide_animation, 80);
	gx_animation_create(&status_menu_animation);
	gx_animation_landing_speed_set(&status_menu_animation, 80);

	gx_widget_show(root);

	gx_system_start(); // start GUIX thread

	return 0;
}

VOID screen_switch(GX_WIDGET *parent, GX_WIDGET *new_screen)
{
	gx_widget_detach(current_screen);
	gx_widget_attach(parent, new_screen);
	current_screen = new_screen;
}