cmake_minimum_required(VERSION 3.8)

target_include_directories(app
	PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}/
)

target_sources(app
	PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}/weather.c
	${CMAKE_CURRENT_SOURCE_DIR}/stopwatch.c
	${CMAKE_CURRENT_SOURCE_DIR}/settings.c
	${CMAKE_CURRENT_SOURCE_DIR}/message.c
	#${CMAKE_CURRENT_SOURCE_DIR}/map.c
	${CMAKE_CURRENT_SOURCE_DIR}/keyboard.c
	${CMAKE_CURRENT_SOURCE_DIR}/healthy.c
	${CMAKE_CURRENT_SOURCE_DIR}/guix_smart_watch_specifications.c
	#${CMAKE_CURRENT_SOURCE_DIR}/game.c
	#${CMAKE_CURRENT_SOURCE_DIR}/expression_evaluate.c
	${CMAKE_CURRENT_SOURCE_DIR}/demo_guix_smart_watch.c
	${CMAKE_CURRENT_SOURCE_DIR}/custom_pixelmap_button_draw.c
	${CMAKE_CURRENT_SOURCE_DIR}/custom_checkbox.c
	${CMAKE_CURRENT_SOURCE_DIR}/contact_info.c
	${CMAKE_CURRENT_SOURCE_DIR}/contact.c
	${CMAKE_CURRENT_SOURCE_DIR}/clock_add.c
	${CMAKE_CURRENT_SOURCE_DIR}/clock.c
	#${CMAKE_CURRENT_SOURCE_DIR}/calculator.c
	${CMAKE_CURRENT_SOURCE_DIR}/alarm.c
        ${CMAKE_CURRENT_SOURCE_DIR}/scroll_wheel.c
)

if (NOT CONFIG_GUI_SPLIT_BINRES)
target_sources(app
	PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}/guix_smart_watch_resources.c
)
endif ()
