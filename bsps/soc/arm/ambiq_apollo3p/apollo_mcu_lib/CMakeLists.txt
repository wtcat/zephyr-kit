add_subdirectory_ifdef(CONFIG_HAS_APOLLO_LIB mcu)

zephyr_include_directories(utils)
zephyr_library_sources(utils/am_util_delay.c)


