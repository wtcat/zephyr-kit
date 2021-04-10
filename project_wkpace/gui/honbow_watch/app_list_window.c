#include "app_list_window.h"
#include "app_list_style_grid_view.h"
void app_list_window_draw(GX_WINDOW *widget)
{
	// gx_window_draw(widget);
	gx_widget_children_draw(widget);
}

UINT app_list_window_event(GX_WINDOW *window, GX_EVENT *event_ptr)
{
	switch (event_ptr->gx_event_type) {
	case GX_EVENT_SHOW: {
		break;
	}
	case GX_EVENT_HIDE: {
		break;
	}
	}
	return gx_window_event_process(window, event_ptr);
}