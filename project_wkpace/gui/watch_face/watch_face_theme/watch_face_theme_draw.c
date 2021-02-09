#include "watch_face_theme_draw.h"
#include "gx_api.h"
#include "watch_face_theme.h"
#include <stdint.h>

extern int _watch_face_color_get(uint32_t index, void * addr);
extern int _watch_face_pixelmap_get(uint32_t index, void ** addr);

static uint16_t watch_face_get_rotate_degree(element_type_enum type, 
												uint16_t degree_total)
{
	uint16_t degree_steps = 0;
	uint16_t degree;
	switch (type){
		case ELEMENT_TYPE_HOUR:
			degree_steps = degree_total/12;
			degree = watch_face_get_hour()%12 * degree_steps;
			degree += watch_face_get_min() * degree_steps/60;
			break;
		case ELEMENT_TYPE_MIN:
			degree_steps = degree_total/60;
			degree = watch_face_get_min() * degree_steps;
			break;
		case ELEMENT_TYPE_SEC:
			degree_steps = degree_total/60;
			degree = watch_face_get_sec() * degree_steps;
			break;
		case ELEMENT_TYPE_BATTERY_CAPACITY: //TODO: need more scale??? 
			degree_steps = degree_total/20;
			degree = watch_face_get_batt_cap()/5 * degree_steps;
			break;
		default: //TODO: support more element!
			degree = 0;
			break;
	}
	return degree;
}

//single img only need check rotate or not
static void watch_face_draw_single_img(struct watch_face_element_style *style,
										GX_WINDOW* widget)
{
	GX_PIXELMAP* map;
	int16_t degree_total = style->angle_end - 
								style->angle_start;
	degree_total =  degree_total <= 0 ? degree_total + 360 : 
					(degree_total > 360 ? degree_total % 360 : degree_total);

	_watch_face_pixelmap_get(style->resource_id_start, (void **)&map);

	if (style->element_rotate_tpye == ELEMENT_ROTATE_TYPE_ROTATING)	{
		uint16_t degree = watch_face_get_rotate_degree(style->element_type,
															degree_total);
		degree += style->angle_start;
		GX_VALUE top = style->y_pos + widget->gx_widget_size.gx_rectangle_top;
		GX_VALUE left = style->x_pos + widget->gx_widget_size.gx_rectangle_left;
		gx_canvas_pixelmap_rotate(left, top, map, degree, -1, -1);
	}else if (style->element_rotate_tpye == ELEMENT_ROTATE_TYPE_NONE) {
		GX_VALUE top = style->y_pos + widget->gx_widget_size.gx_rectangle_top;
		GX_VALUE left = style->x_pos + widget->gx_widget_size.gx_rectangle_left;
		gx_canvas_pixelmap_draw(left, top, map);
	}
}

//select one image from supplied images
static void watch_face_draw_multi_img_one(struct watch_face_element_style *style,
											GX_WINDOW* widget)
{
	GX_PIXELMAP* map;
	uint8_t resource_cnts = style->resource_id_end - 
							style->resource_id_start + 1;
	uint16_t id = 0;
	
	switch (style->element_type) {
		case ELEMENT_TYPE_MONTH:
			if (resource_cnts != 12) {
				return;
			}
			uint8_t curr_month = watch_face_get_month(); //month: 0~11
			if ((curr_month > 11)) {
				curr_month = 0;
			}
			id = curr_month + style->resource_id_start;
			if (id > style->resource_id_end) {
				id = style->resource_id_end;
			}
			break;
		case ELEMENT_TYPE_WEEK_DAY:
			if (resource_cnts != 7) {
				return;
			}
			uint8_t curr_week_day = watch_face_get_week(); //week: 1~7
			if ((curr_week_day > 7) || (curr_week_day < 1)) {
				curr_week_day = 1;
			}
			id = curr_week_day - 1 + style->resource_id_start;
			if (id > style->resource_id_end) {
				id = style->resource_id_end;
			}
			break;
		case ELEMENT_TYPE_WEATHER:
			if (resource_cnts != 15) {
				return;
			}
			uint8_t curr_weather_type = watch_face_get_weather_type();
			if (curr_weather_type > CLOCK_SKIN_WEATHER_CLOUDY_NIGHT) {
				curr_weather_type = CLOCK_SKIN_WEATHER_SUNNY;
			}
			id = curr_weather_type + style->resource_id_start;
			if (id > style->resource_id_end) {
				id = style->resource_id_end;
			}
			break;
		case ELEMENT_TYPE_BATTERY_CAPACITY://0 10 20 30 40 50 60 70 80 90 100
			if (resource_cnts != 11) {
				return;
			}
			uint8_t curr_cap = watch_face_get_batt_cap();
			if (curr_cap > 100) {
				curr_cap = 100;
			}
			id = curr_cap/10 + style->resource_id_start;
			if (id > style->resource_id_end) {
				id = style->resource_id_end;
			}
			break;
		case ELEMENT_TYPE_AM_PM:
			if (resource_cnts != 2) {
				return;
			}
			id = watch_face_get_am_pm_type() + style->resource_id_start;
			break;
		default://do nothing
			return;
	}
	_watch_face_pixelmap_get(id, (void **)&map);
	GX_VALUE top = style->y_pos + widget->gx_widget_size.gx_rectangle_top;
	GX_VALUE left = style->x_pos + widget->gx_widget_size.gx_rectangle_left;
	gx_canvas_pixelmap_draw(left, top, map);
}

static int32_t power_of_ten(uint8_t n)
{	
	int32_t value = 1;
	for (uint8_t i = 0; i < n; i++) {
		value =  value * 10;
	}
	return value;
}

/*****************************************************
	0 1 2 3 4 5 6 7 8 9 0 -
	select two or more images from supplied images
 *****************************************************/
static void disp_multi_img_n_display(uint8_t type, int32_t value, 
									 struct watch_face_element_style *style,
									 GX_WINDOW* widget)
{
	uint8_t cnts = 0;
	GX_PIXELMAP* map = NULL;
	uint16_t resource_start = style->resource_id_start;
	uint8_t  resource_cnts = style->resource_id_end - 
								style->resource_id_start + 1;
	switch (type) {
		case MULTI_IMG_TYPE_NXX:
			cnts = 3;
			break;
		case MULTI_IMG_TYPE_XX:
			cnts = 2;
			break;
		case MULTI_IMG_TYPE_XXX:
			cnts = 3;
			break;
		case MULTI_IMG_TYPE_XXXX:
			cnts = 4;
			break;
		case MULTI_IMG_TYPE_XXXXX:
			cnts = 5;
			break;
		case MULTI_IMG_TYPE_XXXXXX:
			cnts = 6;
			break;
		default:
			return;
	}
	uint16_t id   =  resource_start;
	GX_VALUE top = style->y_pos + widget->gx_widget_size.gx_rectangle_top;
	GX_VALUE left = style->x_pos + widget->gx_widget_size.gx_rectangle_left;

	uint8_t first_no_zero_flag = 0;
	for (uint8_t i = 0; i < cnts; i++) {
		if ((i == 0) && (type == MULTI_IMG_TYPE_NXX)){ //temperature or other
			if (value < 0) {
				if (resource_cnts >= 12) { //must 12 images
					_watch_face_pixelmap_get(resource_start+11, (void **)&map);
				} else {
					map = GX_NULL;
				}
			} else {
				map = GX_NULL;
			}
		} else {
			int32_t value_tmp = value < 0 ? -value:value;
			uint8_t digital_num = value_tmp/power_of_ten(cnts - i - 1)%10;

			if (0 == first_no_zero_flag){
				if (0 == digital_num){
					if ((i != cnts - 1) && (resource_cnts >= 11)) {
						id = resource_start + 10;
					} else {
						id = resource_start;
					}
				} else {
					first_no_zero_flag = 1;
					id = resource_start + digital_num;
				}
			} else {
				id = resource_start + digital_num;
			}
			_watch_face_pixelmap_get(id, (void **)&map);
		}
		if (map != NULL){
			gx_canvas_pixelmap_draw(left, top, map);
		}
		left += style->width; //move to next position!
	}
}

//display multi image at a time, for example, hour need two image:XX
static void watch_face_draw_multi_img_n(struct watch_face_element_style *style,
										GX_WINDOW* widget)
{
	uint8_t type  = MULTI_IMG_TYPE_XX;
	int32_t value = 0;
	
	switch (style->element_type) {
		case ELEMENT_TYPE_MONTH:
			type  = MULTI_IMG_TYPE_XX;
			value = watch_face_get_month() + 1; //month: 0~11
			break;
		case ELEMENT_TYPE_DAY:
			type  = MULTI_IMG_TYPE_XX;
			value = watch_face_get_day();
			break;
		case ELEMENT_TYPE_HOUR:
			type  = MULTI_IMG_TYPE_XX;
			value = watch_face_get_hour();
			break;
		case ELEMENT_TYPE_MIN:
			type  = MULTI_IMG_TYPE_XX;
			value = watch_face_get_min();
			break;
		case ELEMENT_TYPE_SEC:
			type  = MULTI_IMG_TYPE_XX;
			value = watch_face_get_sec();
			break;
		case ELEMENT_TYPE_HEART_RATE:
			type  = MULTI_IMG_TYPE_XX;
			value = watch_face_get_heart_rate();
			break;
		case ELEMENT_TYPE_TEMP:
			type  = MULTI_IMG_TYPE_NXX;
			value = watch_face_get_temp();
			break;
		case ELEMENT_TYPE_STEPS:
			type  = MULTI_IMG_TYPE_XXXXX;
			value = watch_face_get_steps();
			break;
		case ELEMENT_TYPE_CALORIE:
			type  = MULTI_IMG_TYPE_XXXX;
			value = watch_face_get_calories();
			break;
		case ELEMENT_TYPE_BATTERY_CAPACITY:
			type  = MULTI_IMG_TYPE_XXX;
			value = watch_face_get_batt_cap();
			break;
		default: //TODO: add more
			return;
	}
	disp_multi_img_n_display(type, value, style, widget);
}

/***********************************************************
   angle_start: gx_canvas_arc_draw function's end angle,
				it will always keep a value of setting.
   angle_end:   gx_canvas_arc_draw function's start angle,
				it will reduce with the percent.
 **********************************************************/
static void watch_face_draw_arc(struct watch_face_element_style *style,
								GX_WINDOW* widget)
{
	uint8_t percent = 0;
	switch(style->element_type) {
		case ELEMENT_TYPE_STEPS:
			{
				uint32_t steps = watch_face_get_steps();
				percent = steps > 10000 ? 100 : steps/100;
			}
			break;
		case ELEMENT_TYPE_CALORIE:
			{
				uint32_t calories = watch_face_get_calories();
				percent = (calories >= 240) ? 100 : (calories*100/240);
			}
			break;
		case ELEMENT_TYPE_BATTERY_CAPACITY:
			{
				percent = watch_face_get_batt_cap();

				percent = percent > 100 ? 100 : percent;
			}
			break;
		default:
			return;
	}

	GX_DRAW_CONTEXT *current_context;
	gx_system_draw_context_get(&current_context);
	GX_COLOR brush_color;
	_watch_face_color_get(style->resource_id_start, &brush_color);
	gx_brush_define(&current_context->gx_draw_context_brush, brush_color, 0, GX_BRUSH_ALIAS);
	gx_context_brush_width_set(style->brush_width);

	int32_t x_center = style->x_pos + widget->gx_widget_size.gx_rectangle_left + (style->width >> 1);
	int32_t y_center = style->y_pos + widget->gx_widget_size.gx_rectangle_top + (style->width >> 1);
	int32_t angle_total = style->angle_start - style->angle_end;

	angle_total =  angle_total <= 0 ? angle_total + 360 : 
						(angle_total > 360 ? angle_total % 360 : angle_total);

	uint32_t r = style->width>>1;

	int32_t end_angle = style->angle_start;
	int32_t start_angle = 0;
	if (0 != percent){
		start_angle = style->angle_end + 
						(100 - percent) * angle_total /100;
	} else {
		start_angle = end_angle - 1;
	}
	gx_canvas_arc_draw(x_center,y_center,r,start_angle,end_angle);
}

void _watch_face_draw_main(GX_WINDOW* widget)
{
	posix_time_update ();
	
	uint8_t element_cnts = watch_face_theme_element_cnts_get();
	struct watch_face_element_style * styles_to_show = 
						watch_face_theme_element_styles_get();
	for (uint8_t i = 0; i < element_cnts; i++){
		struct watch_face_element_style *curr = &styles_to_show[i];
		switch (curr->element_disp_tpye) {
			case ELEMENT_DISP_TPYE_SINGLE_IMG:
				watch_face_draw_single_img(curr, widget);
				break;
			case ELEMENT_DISP_TPYE_MULTI_IMG_1:
				watch_face_draw_multi_img_one(curr, widget);
				break;
			case ELEMENT_DISP_TPYE_MULTI_IMG_N:
				watch_face_draw_multi_img_n(curr, widget);
				break;
			case ELEMENT_DISP_TPYE_ARC_PROGRESS_BAR:
				watch_face_draw_arc(curr, widget);
				break;
			default:
				break;
		}
	}
}
