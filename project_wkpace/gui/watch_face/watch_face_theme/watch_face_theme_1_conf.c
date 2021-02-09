#include "watch_face_theme_1_conf.h"
#include "guix_simple_resources.h"
#include "gx_api.h"
#include "watch_face_theme.h"


struct watch_face_theme1_ctl watch_face_theme_1 = {
	//head
	.head.ih_magic = WATCH_FACE_HDR_MAGIC_NUM,
	.head.ih_hdr_size = sizeof(struct watch_face_theme1_ctl),
	.head.ih_element_cnt = (sizeof(struct watch_face_theme1_ctl) - sizeof(struct watch_face_header))/sizeof(struct watch_face_element_style),
	.head.ih_bin_size = 0,//internal theme, have no use.
	#if 1

	//element 1: background
	.bg.element_disp_tpye = ELEMENT_DISP_TPYE_SINGLE_IMG,
	.bg.element_type = ELEMENT_TYPE_BG,
	.bg.x_pos = 0,
	.bg.y_pos = 0,
	.bg.resource_id_start = GX_PIXELMAP_ID__,
	.bg.element_rotate_tpye = ELEMENT_ROTATE_TYPE_NONE,
	
	//element2: hour
	.hour_pointer.element_disp_tpye = ELEMENT_DISP_TPYE_SINGLE_IMG,
	.hour_pointer.element_type = ELEMENT_TYPE_HOUR,
	.hour_pointer.x_pos = 159,
	.hour_pointer.y_pos = 0,
	.hour_pointer.resource_id_start = GX_PIXELMAP_ID_HOUR_ID,
	.hour_pointer.element_rotate_tpye = ELEMENT_ROTATE_TYPE_ROTATING,
	.hour_pointer.angle_start = 0,
	.hour_pointer.angle_end = 360,

	//element3: minute
	.min_ponter.element_disp_tpye = ELEMENT_DISP_TPYE_SINGLE_IMG,
	.min_ponter.element_type = ELEMENT_TYPE_MIN,
	.min_ponter.x_pos = 155,
	.min_ponter.y_pos = 0,
	.min_ponter.resource_id_start = GX_PIXELMAP_ID_MIN_ID,
	.min_ponter.element_rotate_tpye = ELEMENT_ROTATE_TYPE_ROTATING,
	.min_ponter.angle_start = 0,
	.min_ponter.angle_end = 360,

	//element4: sec
	.sec_pointer.element_disp_tpye = ELEMENT_DISP_TPYE_SINGLE_IMG,
	.sec_pointer.element_type = ELEMENT_TYPE_SEC,
	.sec_pointer.x_pos = 162,
	.sec_pointer.y_pos = 0,
	.sec_pointer.resource_id_start = GX_PIXELMAP_ID_SEC_ID,
	.sec_pointer.element_rotate_tpye = ELEMENT_ROTATE_TYPE_ROTATING,
	.sec_pointer.angle_start = 0,
	.sec_pointer.angle_end = 360,
	//element5: hour
	.hour.element_disp_tpye = ELEMENT_DISP_TPYE_MULTI_IMG_N,
	.hour.element_type = ELEMENT_TYPE_HOUR,
	.hour.x_pos = 218,
	.hour.y_pos = 163,
	.hour.resource_id_start = GX_PIXELMAP_ID_NUM_0,
	.hour.resource_id_end = GX_PIXELMAP_ID_NUM_A,
	.hour.width = 15,
	.hour.element_rotate_tpye = ELEMENT_ROTATE_TYPE_NONE,

	//element6: min
	.min.element_disp_tpye = ELEMENT_DISP_TPYE_MULTI_IMG_N,
	.min.element_type = ELEMENT_TYPE_MIN,
	.min.x_pos = 254,
	.min.y_pos = 163,
	.min.resource_id_start = GX_PIXELMAP_ID_NUM_0,
	.min.resource_id_end = GX_PIXELMAP_ID_NUM_A,
	.min.element_rotate_tpye = ELEMENT_ROTATE_TYPE_NONE,
	.min.width = 15,

	//heart_rate	
	.heart_rate.element_disp_tpye = ELEMENT_DISP_TPYE_MULTI_IMG_N,
	.heart_rate.element_type = ELEMENT_TYPE_HEART_RATE,
	.heart_rate.x_pos = 254,
	.heart_rate.y_pos = 134,
	.heart_rate.resource_id_start = GX_PIXELMAP_ID_NUM_0,
	.heart_rate.resource_id_end = GX_PIXELMAP_ID_NUM_A,
	.heart_rate.element_rotate_tpye = ELEMENT_ROTATE_TYPE_NONE,
	.heart_rate.width = 15,

	//temp	
	.temp.element_disp_tpye = ELEMENT_DISP_TPYE_MULTI_IMG_N,
	.temp.element_type = ELEMENT_TYPE_TEMP,
	.temp.x_pos = 236,
	.temp.y_pos = 258,
	.temp.resource_id_start = GX_PIXELMAP_ID_NUM_0,
	.temp.resource_id_end = GX_PIXELMAP_ID_NUM_B,
	.temp.element_rotate_tpye = ELEMENT_ROTATE_TYPE_NONE,
	.temp.width = 15,

	//week	
	.week.element_disp_tpye = ELEMENT_DISP_TPYE_MULTI_IMG_1,
	.week.element_type = ELEMENT_TYPE_WEEK_DAY,
	.week.x_pos = 204,
	.week.y_pos = 185,
	.week.resource_id_start = GX_PIXELMAP_ID_WEEK_1,
	.week.resource_id_end = GX_PIXELMAP_ID_WEEK_7,
	.week.element_rotate_tpye = ELEMENT_ROTATE_TYPE_NONE,
	.week.width = 15,
	
	//day	
	.day.element_disp_tpye = ELEMENT_DISP_TPYE_MULTI_IMG_N,
	.day.element_type = ELEMENT_TYPE_DAY,
	.day.x_pos = 256,
	.day.y_pos = 185,
	.day.resource_id_start = GX_PIXELMAP_ID_NUM_0,
	.day.resource_id_end = GX_PIXELMAP_ID_NUM_A,
	.day.element_rotate_tpye = ELEMENT_ROTATE_TYPE_NONE,
	.day.width = 15,
	
	//month	
	.month.element_disp_tpye = ELEMENT_DISP_TPYE_MULTI_IMG_1,
	.month.element_type = ELEMENT_TYPE_MONTH,
	.month.x_pos = 290,
	.month.y_pos = 185,
	.month.resource_id_start = GX_PIXELMAP_ID_MONTH_1,
	.month.resource_id_end = GX_PIXELMAP_ID_MONTH_C,
	.month.element_rotate_tpye = ELEMENT_ROTATE_TYPE_NONE,
	.month.width = 15,

	.steps_progress_bar.element_disp_tpye = ELEMENT_DISP_TPYE_ARC_PROGRESS_BAR,
	.steps_progress_bar.element_type = ELEMENT_TYPE_STEPS,
	.steps_progress_bar.x_pos = 55,
	.steps_progress_bar.y_pos = 131,
	.steps_progress_bar.width = 92,
	.steps_progress_bar.resource_id_start = GX_COLOR_ID_BLUE,
	.steps_progress_bar.resource_id_end = GX_COLOR_ID_DISABLED_FILL,
	.steps_progress_bar.angle_start = 90,
	.steps_progress_bar.angle_end = 90,
	.steps_progress_bar.brush_width = 2,

	.calories_progress_bar.element_disp_tpye = ELEMENT_DISP_TPYE_ARC_PROGRESS_BAR,
	.calories_progress_bar.element_type = ELEMENT_TYPE_CALORIE,
	.calories_progress_bar.x_pos = 144,
	.calories_progress_bar.y_pos = 70,
	.calories_progress_bar.width = 70,
	.calories_progress_bar.resource_id_start = GX_COLOR_ID_BLUE,
	.calories_progress_bar.resource_id_end = GX_COLOR_ID_DISABLED_FILL,
	.calories_progress_bar.angle_start = 60,
	.calories_progress_bar.angle_end = 120,
	.calories_progress_bar.brush_width = 2,

	//calories_num
	.calories_num.element_disp_tpye = ELEMENT_DISP_TPYE_MULTI_IMG_N,
	.calories_num.element_type = ELEMENT_TYPE_STEPS,
	.calories_num.x_pos = 65,
	.calories_num.y_pos = 169,
	.calories_num.resource_id_start = GX_PIXELMAP_ID_KAL_NUM_0,
	.calories_num.resource_id_end = GX_PIXELMAP_ID_KAL_NUM_MINUS,
	.calories_num.element_rotate_tpye = ELEMENT_ROTATE_TYPE_NONE,
	.calories_num.width = 15,

	//calories_num
	.steps_num.element_disp_tpye = ELEMENT_DISP_TPYE_MULTI_IMG_N,
	.steps_num.element_type = ELEMENT_TYPE_CALORIE,
	.steps_num.x_pos = 154,
	.steps_num.y_pos = 92,
	.steps_num.resource_id_start = GX_PIXELMAP_ID_KAL_NUM_0,
	.steps_num.resource_id_end = GX_PIXELMAP_ID_KAL_NUM_MINUS,
	.steps_num.element_rotate_tpye = ELEMENT_ROTATE_TYPE_NONE,
	.steps_num.width = 15,

	//am_pm
	.am_pm.element_disp_tpye = ELEMENT_DISP_TPYE_MULTI_IMG_1,
	.am_pm.element_type = ELEMENT_TYPE_AM_PM,
	.am_pm.x_pos = 291,
	.am_pm.y_pos = 160,
	.am_pm.resource_id_start = GX_PIXELMAP_ID_AM_PM_0,
	.am_pm.resource_id_end = GX_PIXELMAP_ID_AM_PM_1,
	.am_pm.element_rotate_tpye = ELEMENT_ROTATE_TYPE_NONE,

	//batt_num
	.batt_num.element_disp_tpye = ELEMENT_DISP_TPYE_MULTI_IMG_N,
	.batt_num.element_type = ELEMENT_TYPE_BATTERY_CAPACITY,
	.batt_num.x_pos = 232,
	.batt_num.y_pos = 288,
	.batt_num.resource_id_start = GX_PIXELMAP_ID_BATT_NUM_0,
	.batt_num.resource_id_end = GX_PIXELMAP_ID_BATT_NUM_A,
	.batt_num.element_rotate_tpye = ELEMENT_ROTATE_TYPE_NONE,
	.batt_num.width = 8,

	//batt_pointer
	.batt_pointer.element_disp_tpye = ELEMENT_DISP_TPYE_SINGLE_IMG,
	.batt_pointer.element_type = ELEMENT_TYPE_BATTERY_CAPACITY,
	.batt_pointer.x_pos = 164,
	.batt_pointer.y_pos = 203,
	.batt_pointer.resource_id_start = GX_PIXELMAP_ID_BATT_ID,
	.batt_pointer.element_rotate_tpye = ELEMENT_ROTATE_TYPE_ROTATING,
	.batt_pointer.angle_start = 270,
	.batt_pointer.angle_end = 90,
	#endif
};
