/*************************************
 ************************************/
#ifndef __WATCH_FACE_THEME_DRAW_H
#define __WATCH_FACE_THEME_DRAW_H


#include <stdio.h>
#include <stdint.h>

#include "gx_api.h"

#include "guix_simple_resources.h"
#include "guix_simple_specifications.h"

#include "watch_face_theme.h"
#include "watch_face_port.h"

#include "watch_face_manager.h"

typedef enum {
	MULTI_IMG_TYPE_NXX = 0, //temperature
	MULTI_IMG_TYPE_XX,
	MULTI_IMG_TYPE_XXX,
	MULTI_IMG_TYPE_XXXX,
	MULTI_IMG_TYPE_XXXXX,
	MULTI_IMG_TYPE_XXXXXX,
}multi_img_type_enum;

void _watch_face_draw_main(GX_WINDOW* widget);

#endif
