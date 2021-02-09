#include "watch_face_theme.h"

#define ELEMENT_MAX_CNT  30
static struct   watch_face_header watch_face_header_curr;
static struct   watch_face_element_style element_styles[ELEMENT_MAX_CNT];
static uint8_t  _element_cnts_actual = 0;

static int watch_face_theme_verify(unsigned char * theme_root_addr)
{
	struct watch_face_header watch_face_header_tmp;
	memcpy((void *)&watch_face_header_tmp, theme_root_addr, 
				sizeof(struct watch_face_header));
	if (watch_face_header_tmp.ih_magic != WATCH_FACE_HDR_MAGIC_NUM) {
		return -1;
	}
	return 0;
}

int watch_face_theme_init(unsigned char * theme_root_addr)
{
	uint32_t offset = 0;
	if (watch_face_theme_verify(theme_root_addr)) {
		_element_cnts_actual = 0;
		return -1;
	}
	memcpy((void *)&watch_face_header_curr, theme_root_addr+offset, 
				sizeof(struct watch_face_header));
	offset += sizeof(struct watch_face_header);

	_element_cnts_actual = watch_face_header_curr.ih_element_cnt >= ELEMENT_MAX_CNT ?
							ELEMENT_MAX_CNT : watch_face_header_curr.ih_element_cnt;
	for (uint16_t i = 0; i < _element_cnts_actual; i++) {
		struct watch_face_element_style *p_element = &element_styles[i];
		memcpy((char *)p_element, theme_root_addr+offset,
					sizeof(struct watch_face_element_style));
		offset += sizeof(struct watch_face_element_style);
	}
	return 0;
}

void watch_face_theme_deinit(void)
{
	_element_cnts_actual = 0;
}

uint8_t watch_face_theme_element_cnts_get(void)
{
	return _element_cnts_actual;
}

struct watch_face_element_style  * watch_face_theme_element_styles_get(void)
{
	return element_styles;
}

uint16_t watch_face_theme_hdr_size_get(void)
{
	return watch_face_header_curr.ih_hdr_size;
}
