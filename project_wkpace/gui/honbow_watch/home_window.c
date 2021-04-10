#include "home_window.h"
#include "custom_pixelmap_mirror.h"
#include "custom_util.h"
#include "guix_simple_resources.h"
#include "guix_simple_specifications.h"
#include "wf_window.h"
#include "sys/util.h"
typedef struct {
	GX_WIDGET widget;
	GX_RESOURCE_ID color_id;
} dot_t;

typedef struct {
	GX_WIDGET parent;
	dot_t left2_window;
	dot_t left1_window;
	dot_t center_window;
	dot_t right1_window;
	dot_t right2_window;
} dot_tips_union_t;

dot_tips_union_t circle_tips;
extern void circle_tips_disp_adjust(void);

/* Define menu screen list. */
GX_WIDGET *status_menu_screen_list[] = {
	(GX_WIDGET *)&root_window.root_window_wf_window,
	(GX_WIDGET *)&control_center_window,
	GX_NULL,
};
#define ANIMATION_ID_MENU_STATUS_MENU 2

extern VOID screen_switch(GX_WIDGET *parent, GX_WIDGET *new_screen);

GX_ANIMATION status_menu_animation;

VOID start_status_menu_animation(GX_WINDOW *window)
{
	GX_ANIMATION_INFO slide_animation_info;

	memset(&slide_animation_info, 0, sizeof(GX_ANIMATION_INFO));
	slide_animation_info.gx_animation_parent = (GX_WIDGET *)window;
	slide_animation_info.gx_animation_style = GX_ANIMATION_SCREEN_DRAG | GX_ANIMATION_VERTICAL | GX_ANIMATION_WRAP;
	slide_animation_info.gx_animation_id = ANIMATION_ID_MENU_STATUS_MENU;
	slide_animation_info.gx_animation_frame_interval = 1;
	slide_animation_info.gx_animation_slide_screen_list = status_menu_screen_list;

	// new property for window shift limit.
	slide_animation_info.gx_animation_range_limit_type = GX_ANIMATION_LIMIT_STYLE_VERTICAL_UP;

	gx_animation_drag_enable(&status_menu_animation, (GX_WIDGET *)window, &slide_animation_info);
}

GX_WIDGET *menu_screen_list[] = {
	GX_NULL, GX_NULL, (GX_WIDGET *)&root_window.root_window_wf_window, (GX_WIDGET *)&app_list_window, GX_NULL, GX_NULL,
};

#define ANIMATION_ID_MENU_SLIDE 1

GX_ANIMATION slide_animation;

VOID start_slide_animation(GX_WINDOW *window)
{
	GX_ANIMATION_INFO slide_animation_info;

	memset(&slide_animation_info, 0, sizeof(GX_ANIMATION_INFO));
	slide_animation_info.gx_animation_parent = (GX_WIDGET *)window;
	slide_animation_info.gx_animation_style = GX_ANIMATION_SCREEN_DRAG | GX_ANIMATION_HORIZONTAL;
	slide_animation_info.gx_animation_id = ANIMATION_ID_MENU_SLIDE;
	slide_animation_info.gx_animation_frame_interval = 1;
	uint8_t index = 0;
	for (index = 0; index < sizeof(menu_screen_list) / sizeof(GX_WIDGET *); index++) {
		if (menu_screen_list[index] != NULL) {
			break;
		}
	}
	slide_animation_info.gx_animation_slide_screen_list = &menu_screen_list[index];
	gx_animation_drag_enable(&slide_animation, (GX_WIDGET *)window, &slide_animation_info);
}

/*****************************************************************
	root window event for slide left right up down control
 *****************************************************************/
static GX_VALUE last_x = 0;
static GX_VALUE last_y = 0;
static volatile uint8_t enter_h = 0;
static volatile uint8_t enter_v = 0;
static GX_VALUE delta_y;
static GX_VALUE delta_x;
volatile static uint8_t home_window_enter_pen_down = 0;

bool home_window_is_pen_down(void)
{
	return home_window_enter_pen_down == 1 ? true : false;
}

static uint8_t wf_mirror_created = 0;
#define CLOCK_SHOW_WF_TIMER 1
#define CLOCK_DOT_DISP_TIMER 2
UINT home_window_event(GX_WINDOW *window, GX_EVENT *event_ptr)
{
	switch (event_ptr->gx_event_type) {
	case GX_EVENT_SHOW: {
		if (0 == wf_mirror_created) {
			GX_CANVAS *canvas_tmp;
			gx_widget_canvas_get((GX_WIDGET *)window, &canvas_tmp);
			mirror_obj_create_copy(&wf_mirror_mgr.wf_mirror_obj, canvas_tmp);
			wf_mirror_created = 1;
		}
		// gx_widget_front_move(&circle_tips.parent, NULL);
	} break;
	case GX_EVENT_HIDE:
		// mirror_obj_delete(&wf_mirror_mgr.wf_mirror_obj);
		break;
	case GX_EVENT_TIMER:
		if (event_ptr->gx_event_payload.gx_event_timer_id == CLOCK_SHOW_WF_TIMER) {
			// gx_system_timer_stop(window, CLOCK_SHOW_WF_TIMER);
			if ((status_menu_animation.gx_animation_status == GX_ANIMATION_IDLE) &&
				(slide_animation.gx_animation_status == GX_ANIMATION_IDLE) &&
				(root_window.root_window_wf_window.gx_widget_status & GX_STATUS_VISIBLE)) {
				screen_switch(window->gx_widget_parent, (GX_WIDGET *)&wf_list);
				home_window_enter_pen_down = 0;
			}
			return 0;
		} else if (event_ptr->gx_event_payload.gx_event_timer_id == CLOCK_DOT_DISP_TIMER) {
			gx_widget_hide(&circle_tips.parent);
			return 0;
		} else {
			break;
		}
	case GX_EVENT_PEN_DOWN:
	case GX_EVENT_PEN_DRAG:
		wf_window_mirror_set(true);
		if ((home_window_enter_pen_down) && (!enter_h) && (!enter_v)) {
			delta_y = GX_ABS(event_ptr->gx_event_payload.gx_event_pointdata.gx_point_y - last_y);
			delta_x = GX_ABS(event_ptr->gx_event_payload.gx_event_pointdata.gx_point_x - last_x);
			if ((delta_x > 5) || (delta_y > 5)) {
				gx_system_timer_stop(window, CLOCK_SHOW_WF_TIMER);
				GX_WINDOW *parent_window = window;
				if (delta_y > delta_x) {
					start_status_menu_animation(parent_window);
					enter_h = 1;
					gx_widget_hide(&circle_tips.parent);
				} else {
					start_slide_animation(parent_window);
					enter_v = 1;
					gx_system_timer_stop(window, CLOCK_DOT_DISP_TIMER);
					gx_widget_front_move(&circle_tips.parent, NULL);
					circle_tips_disp_adjust();
					gx_widget_show(&circle_tips.parent);
				}
				// send pen down event to animation's parent window.
				GX_EVENT tmp = *event_ptr;
				tmp.gx_event_payload.gx_event_pointdata.gx_point_x = last_x;
				tmp.gx_event_payload.gx_event_pointdata.gx_point_y = last_y;
				tmp.gx_event_type = GX_EVENT_PEN_DOWN;
				tmp.gx_event_target = (struct GX_WIDGET_STRUCT *)parent_window;
				gx_system_event_send(&tmp);

				tmp.gx_event_payload.gx_event_pointdata.gx_point_x =
					event_ptr->gx_event_payload.gx_event_pointdata.gx_point_x;
				tmp.gx_event_payload.gx_event_pointdata.gx_point_y =
					event_ptr->gx_event_payload.gx_event_pointdata.gx_point_y;
				tmp.gx_event_type = GX_EVENT_PEN_DRAG;
				gx_system_event_send(&tmp);
			}
		}

		if (slide_animation.gx_animation_original_event_process_function != NULL) {
			gx_widget_front_move(&circle_tips.parent, NULL);
			// circle_tips_disp_adjust();
			gx_widget_show(&circle_tips.parent);
		}

		if (!home_window_enter_pen_down) {
			home_window_enter_pen_down = 1;
			gx_system_timer_start(window, CLOCK_SHOW_WF_TIMER, 100, 0);
			last_y = event_ptr->gx_event_payload.gx_event_pointdata.gx_point_y;
			last_x = event_ptr->gx_event_payload.gx_event_pointdata.gx_point_x;
		}
		break;

	case GX_EVENT_PEN_UP:
		gx_system_timer_stop(window, CLOCK_SHOW_WF_TIMER);
		home_window_enter_pen_down = 0;
	case GX_EVENT_ANIMATION_COMPLETE:
		if (root_window.root_window_wf_window.gx_widget_status & GX_STATUS_VISIBLE) {
			if ((status_menu_animation.gx_animation_status == GX_ANIMATION_IDLE) &&
				(slide_animation.gx_animation_status == GX_ANIMATION_IDLE)) {
				if (enter_h) {
					gx_animation_drag_disable(&status_menu_animation, (GX_WIDGET *)window);
					enter_h = 0;
				}
				if (enter_v) {
					gx_animation_drag_disable(&slide_animation, (GX_WIDGET *)window);
					enter_v = 0;
				}
			}
		}

		if (slide_animation.gx_animation_status == GX_ANIMATION_IDLE) {
			circle_tips_disp_adjust();
			gx_widget_front_move(&circle_tips.parent, NULL);
			gx_system_timer_start(window, CLOCK_DOT_DISP_TIMER, 40, 0);
		}
		break;
	case GX_EVENT_KEY_DOWN:
		if (event_ptr->gx_event_payload.gx_event_ushortdata[0] == GX_KEY_HOME) {
			return 0;
		}
		break;
	case GX_EVENT_KEY_UP:
		if (event_ptr->gx_event_payload.gx_event_ushortdata[0] == GX_KEY_HOME) {
			GX_EVENT event;
			event.gx_event_type = GX_EVENT_TERMINATE;
			event.gx_event_target = NULL;
			// printk("GUIX enter in sleep\n");
			gx_system_event_send(&event);
			return 0;
		}
		break;
	default:
		break;
	}
	return gx_window_event_process(window, event_ptr);
}

void circle_tips_draw(GX_WIDGET *widget)
{
	GX_DRAW_CONTEXT *current_context;
	gx_system_draw_context_get(&current_context);

	dot_t *dot = CONTAINER_OF(widget, dot_t, widget);
	GX_COLOR color = 0;
	gx_context_color_get(dot->color_id, &color);

	if (dot == &circle_tips.center_window) {
		gx_brush_define(&current_context->gx_draw_context_brush, color, color, GX_BRUSH_SOLID_FILL);
	} else {
		gx_brush_define(&current_context->gx_draw_context_brush, color, color, GX_BRUSH_SOLID_FILL);
	}

	gx_context_brush_width_set(0);
	INT r = 5;
	INT x = widget->gx_widget_size.gx_rectangle_left + r;
	INT y = widget->gx_widget_size.gx_rectangle_top + r;
	gx_canvas_circle_draw(x, y, r);
}

void home_window_draw(GX_WIDGET *home)
{
	gx_widget_draw(home);
}

void circle_window_draw(GX_WIDGET *widget)
{
	gx_widget_children_draw(widget);
}

void home_window_init(GX_WIDGET *home)
{
#if 1
	GX_RECTANGLE child;
	gx_utility_rectangle_define(&child, 123, 8, 123 + 74 - 1, 8 + 10 - 1);
	gx_widget_create(&circle_tips.parent, NULL, home, GX_STYLE_BORDER_NONE | GX_STYLE_ENABLED | GX_STYLE_TRANSPARENT,
					 GX_ID_NONE, &child);

	GX_VALUE x_start = 123; // need absolut pos
	GX_VALUE y_start = 8;
	gx_utility_rectangle_define(&child, x_start, y_start, x_start + 9, y_start + 9);
	gx_widget_create(&circle_tips.left2_window.widget, NULL, &circle_tips.parent,
					 GX_STYLE_BORDER_NONE | GX_STYLE_ENABLED, GX_ID_NONE, &child);
	gx_widget_draw_set(&circle_tips.left2_window.widget, circle_tips_draw);

	x_start += 16;
	gx_utility_rectangle_define(&child, x_start, y_start, x_start + 9, y_start + 9);
	gx_widget_create(&circle_tips.left1_window.widget, NULL, &circle_tips.parent,
					 GX_STYLE_BORDER_NONE | GX_STYLE_ENABLED, GX_ID_NONE, &child);
	gx_widget_draw_set(&circle_tips.left1_window.widget, circle_tips_draw);

	x_start += 16;
	gx_utility_rectangle_define(&child, x_start, y_start, x_start + 9, y_start + 9);
	gx_widget_create(&circle_tips.center_window.widget, NULL, &circle_tips.parent,
					 GX_STYLE_BORDER_NONE | GX_STYLE_ENABLED, GX_ID_NONE, &child);
	gx_widget_draw_set(&circle_tips.center_window.widget, circle_tips_draw);

	x_start += 16;
	gx_utility_rectangle_define(&child, x_start, y_start, x_start + 9, y_start + 9);
	gx_widget_create(&circle_tips.right1_window.widget, NULL, &circle_tips.parent,
					 GX_STYLE_BORDER_NONE | GX_STYLE_ENABLED, GX_ID_NONE, &child);
	gx_widget_draw_set(&circle_tips.right1_window.widget, circle_tips_draw);

	x_start += 16;
	gx_utility_rectangle_define(&child, x_start, y_start, x_start + 9, y_start + 9);
	gx_widget_create(&circle_tips.right2_window.widget, NULL, &circle_tips.parent,
					 GX_STYLE_BORDER_NONE | GX_STYLE_ENABLED, GX_ID_NONE, &child);
	gx_widget_draw_set(&circle_tips.right2_window.widget, circle_tips_draw);

	gx_widget_hide(&circle_tips.parent);
#endif
}

void circle_tips_disp_adjust(void)
{
	if (menu_screen_list[0] == NULL) {
		gx_widget_hide(&circle_tips.left2_window.widget);
	} else {
		if (menu_screen_list[0]->gx_widget_status & GX_STATUS_VISIBLE) {
			circle_tips.left2_window.color_id = GX_COLOR_ID_WHITE;
		} else {
			circle_tips.left2_window.color_id = GX_COLOR_ID_GRAY;
		}
	}

	if (menu_screen_list[1] == NULL) {
		gx_widget_hide(&circle_tips.left1_window.widget);
	} else {
		if (menu_screen_list[1]->gx_widget_status & GX_STATUS_VISIBLE) {
			circle_tips.left1_window.color_id = GX_COLOR_ID_WHITE;
		} else {
			circle_tips.left1_window.color_id = GX_COLOR_ID_GRAY;
		}
	}
	if (menu_screen_list[2] == NULL) {
		gx_widget_hide(&circle_tips.center_window.widget);
	} else {
		if (menu_screen_list[2]->gx_widget_status & GX_STATUS_VISIBLE) {
			circle_tips.center_window.color_id = GX_COLOR_ID_WHITE;
		} else {
			circle_tips.center_window.color_id = GX_COLOR_ID_GRAY;
		}
	}
	// menu_screen_list[2] always show
	if (menu_screen_list[3] == NULL) {
		gx_widget_hide(&circle_tips.right1_window.widget);
	} else {
		if (menu_screen_list[3]->gx_widget_status & GX_STATUS_VISIBLE) {
			circle_tips.right1_window.color_id = GX_COLOR_ID_WHITE;
		} else {
			circle_tips.right1_window.color_id = GX_COLOR_ID_GRAY;
		}
	}
	if (menu_screen_list[4] == NULL) {
		gx_widget_hide(&circle_tips.right2_window.widget);
	} else {
		if (menu_screen_list[4]->gx_widget_status & GX_STATUS_VISIBLE) {
			circle_tips.right2_window.color_id = GX_COLOR_ID_WHITE;
		} else {
			circle_tips.right2_window.color_id = GX_COLOR_ID_GRAY;
		}
	}
}