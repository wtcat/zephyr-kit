# Components

zephyr_include_directories(.)

add_subdirectory(base)
add_subdirectory(stp)
add_subdirectory(protobuf-c)

add_subdirectory_ifdef(CONFIG_GUIX gui)
add_subdirectory_ifdef(CONFIG_BLE_COMPONENT ble)
add_subdirectory_ifdef(CONFIG_BLE_COMPONENT uecc)

