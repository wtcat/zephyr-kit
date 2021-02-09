#ifndef WATCH_FACE_THEME_H
#define WATCH_FACE_THEME_H
#include "string.h"
#include "stdlib.h"
#include <stdint.h>

#define WATCH_FACE_HDR_MAGIC_NUM  0x457d6f

struct watch_face_header {
    uint32_t ih_magic;
    uint16_t ih_hdr_size;           /* Size of image header (bytes). */
	uint16_t ih_element_cnt;        /* cnts of element */
    uint32_t ih_bin_size;           /* Does not include header. */
}__attribute__ ((aligned (4)));

typedef enum  {
	ELEMENT_TYPE_BG      = 0,
	ELEMENT_TYPE_YEAR,
	ELEMENT_TYPE_MONTH,
	ELEMENT_TYPE_DAY,
	ELEMENT_TYPE_HOUR,
	ELEMENT_TYPE_MIN,
	ELEMENT_TYPE_SEC,
	ELEMENT_TYPE_AM_PM,
	ELEMENT_TYPE_WEEK_DAY,
	ELEMENT_TYPE_HEART_RATE,
	ELEMENT_TYPE_CALORIE,
	ELEMENT_TYPE_STEPS,
	ELEMENT_TYPE_BATTERY_CAPACITY,
	ELEMENT_TYPE_WEATHER,
	ELEMENT_TYPE_TEMP,
}element_type_enum;

typedef enum {
	ELEMENT_DISP_TPYE_SINGLE_IMG = 0,   
	ELEMENT_DISP_TPYE_MULTI_IMG_1,      
	ELEMENT_DISP_TPYE_MULTI_IMG_N,
	ELEMENT_DISP_TPYE_ARC_PROGRESS_BAR,
}element_disp_tpye_enum;

typedef enum {
	ELEMENT_ROTATE_TYPE_NONE = 0,
	ELEMENT_ROTATE_TYPE_ROTATING, //rotate by element type
}element_rotate_type_enum;

struct watch_face_element_style {
	uint8_t  element_disp_tpye;
	uint8_t  element_type;
	uint8_t  element_rotate_tpye;
	uint8_t  reverse;
	uint16_t resource_id_start;
	uint16_t resource_id_end;
	
	uint16_t x_pos;
	uint16_t y_pos;
	int16_t  angle_start;
	int16_t  angle_end;
	uint16_t width;       //for multi_img_n or arc
	uint16_t brush_width; //for arc
}__attribute__ ((aligned (4)));


void watch_face_theme_deinit(void);
int watch_face_theme_init(unsigned char * theme_root_addr);
uint8_t watch_face_theme_element_cnts_get(void);
struct watch_face_element_style * watch_face_theme_element_styles_get(void);
uint16_t watch_face_theme_hdr_size_get(void);
#endif
