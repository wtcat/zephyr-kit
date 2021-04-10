#include "app_compass_window.h"
#include "guix_simple_resources.h"
#include "guix_simple_specifications.h"
#include "app_list_define.h"
#include "drivers_ext/sensor_priv.h"
static uint32_t angree = 0;
void app_compass_window_draw(GX_WIDGET *widget)
{
	gx_window_draw((GX_WINDOW *)widget);
	GX_PIXELMAP *map = NULL;
	gx_context_pixelmap_get(GX_PIXELMAP_ID_APP_COMPASS_SMALL, &map);
	if (map != NULL) {
		gx_canvas_pixelmap_rotate(widget->gx_widget_size.gx_rectangle_left + 13,
								  widget->gx_widget_size.gx_rectangle_top + 55, map, angree, -1, -1);
	}
}

#define COMPASS_TIME_ID 12
VOID screen_switch(GX_WIDGET *parent, GX_WIDGET *new_screen);
GX_VALUE gx_point_x_first;
static GX_VALUE delta_x;

const struct device *dev;
UINT app_compass_window_event(GX_WINDOW *window, GX_EVENT *event_ptr)
{
	switch (event_ptr->gx_event_type) {
	case GX_EVENT_SHOW:
		gx_system_timer_start(window, COMPASS_TIME_ID, 1, 1);
		dev = device_get_binding("sensor_pah8112");
		if (dev != NULL) {
			struct sensor_value value;
			value.val1 = 1;
			sensor_attr_set(dev, (enum sensor_channel)SENSOR_CHAN_SPO2,
							(enum sensor_attribute)SENSOR_ATTR_SENSOR_START_STOP, &value);
		}
		break;
	case GX_EVENT_HIDE:
		gx_system_timer_stop(window, COMPASS_TIME_ID);
		dev = device_get_binding("sensor_pah8112");
		if (dev != NULL) {
			struct sensor_value value;
			value.val1 = 0;
			sensor_attr_set(dev, (enum sensor_channel)SENSOR_CHAN_SPO2,
							(enum sensor_attribute)SENSOR_ATTR_SENSOR_START_STOP, &value);
		}

		break;
	case GX_EVENT_TIMER: {
		UINT timer_id = event_ptr->gx_event_payload.gx_event_timer_id;
		if (timer_id == COMPASS_TIME_ID) {
			angree += 10;
			if (angree >= 360) {
				angree = 0;
			}
			gx_system_dirty_mark(window);
			dev = device_get_binding("sensor_pah8112");
			if (dev != NULL) {
				struct sensor_value value;
				sensor_channel_get(dev, (enum sensor_channel)SENSOR_CHAN_HEART_RATE, &value);
				if (value.val2)
					printk("sensor channel get val1: %d\n", value.val1);
				sensor_channel_get(dev, (enum sensor_channel)SENSOR_CHAN_SPO2, &value);
				if (value.val2)
					printk("sensor channel get val1: %d\n", value.val1);
			}
			return 0;
		}
		break;
	}
	case GX_EVENT_PEN_DOWN: {
		gx_point_x_first = event_ptr->gx_event_payload.gx_event_pointdata.gx_point_x;
		return 0;
	}
	case GX_EVENT_PEN_UP: {
		delta_x = GX_ABS(event_ptr->gx_event_payload.gx_event_pointdata.gx_point_x - gx_point_x_first);
		if (delta_x >= 50) {
			screen_switch((GX_WIDGET *)&root_window, (GX_WIDGET *)&root_window.root_window_home_window);
		}
		return 0;
	}
	}
	return gx_widget_event_process(window, event_ptr);
}

GUI_APP_DEFINE(compass, APP_ID_11_COMPASS, (GX_WIDGET *)&app_compass_window, GX_PIXELMAP_ID_APP_LIST_ICON_COMPASS_90_90,
			   GX_PIXELMAP_ID_APP_LIST_ICON_90_OVERLAY);
