add_subdirectory_ifdef(CONFIG_HAS_APOLLO_LIB mcu)

zephyr_include_directories(utils)

zephyr_library_sources(
  utils/am_util_delay.c
)

zephyr_library_sources_ifdef(CONFIG_BLE_COMPONENT 
  utils/am_util_ble.c
)


