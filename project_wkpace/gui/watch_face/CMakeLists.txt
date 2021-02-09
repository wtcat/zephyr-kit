zephyr_library_include_directories(
  .
  watch_face_theme
  pixelmap_mirror
)

zephyr_library_sources(
    demo_guix_main.c
	guix_simple_specifications.c
	guix_simple_resources.c
	watch_face_theme/watch_face_port.c
	watch_face_theme/watch_face_theme.c
	watch_face_theme/watch_face_theme_draw.c
	watch_face_theme/watch_face_theme_1_conf.c
	watch_face_theme/watch_face_manager.c
	pixelmap_mirror/pixelmap_mirror.c
)
