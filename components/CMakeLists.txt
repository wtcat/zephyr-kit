# Components

zephyr_include_directories(.)

add_subdirectory_ifdef(CONFIG_BLE_COMPONENT ble)
add_subdirectory_ifdef(CONFIG_BLE_COMPONENT uecc)
add_subdirectory(base)
add_subdirectory(gui)
