zephyr_library_include_directories(
  .
  watch_face_theme
  alarm
  utils
)

zephyr_library_sources(
	utils/custom_checkbox.c
	utils/custom_util.c
	utils/custom_pixelmap_button_draw.c
	utils/custom_vertical_total_rows.c
	utils/custom_horizontal_total_rows.c
	utils/custom_binres_theme_load.c
	utils/custom_pixelmap_mirror.c
	utils/custom_icon_utils.c
)

zephyr_library_sources(
	watch_face_theme/watch_face_port.c
	watch_face_theme/watch_face_theme.c
	watch_face_theme/watch_face_theme_draw.c
	watch_face_theme/watch_face_theme_1_conf.c
	watch_face_theme/watch_face_manager.c
	watch_face_theme/watch_face_theme_customize.c
)

zephyr_library_sources(
	guix_simple_specifications.c
	guix_simple_resources.c
    demo_guix_main.c
	wf_window.c
	wf_list_window.c
	home_window.c
	app_list_window.c
	app_list_style_grid_view.c
	app_list_define.c
	app_compass_window.c
	control_center_window.c
)

zephyr_linker_sources(RAM_SECTIONS app_info.ld)

