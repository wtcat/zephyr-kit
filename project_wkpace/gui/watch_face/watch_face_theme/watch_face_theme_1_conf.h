#ifndef __WATCH_FACE_THEME_1_H__
#define __WATCH_FACE_THEME_1_H__

#include "watch_face_theme.h"
#include "guix_simple_resources.h"

struct watch_face_theme1_ctl{
	struct watch_face_header head;
	#if 1

	struct watch_face_element_style bg;  //background

	struct watch_face_element_style hour; //hour
	struct watch_face_element_style min; // min

	struct watch_face_element_style heart_rate;
	struct watch_face_element_style temp;


	struct watch_face_element_style week;
	struct watch_face_element_style day;	
	struct watch_face_element_style month;

	struct watch_face_element_style steps_progress_bar;
	struct watch_face_element_style calories_progress_bar;

	struct watch_face_element_style steps_num;
	struct watch_face_element_style calories_num;

	struct watch_face_element_style am_pm;
	struct watch_face_element_style batt_num;
	struct watch_face_element_style batt_pointer;
	#endif
	struct watch_face_element_style hour_pointer;//hour pointer
	struct watch_face_element_style min_ponter;
	struct watch_face_element_style sec_pointer;
}__attribute__ ((aligned (4)));

#endif
