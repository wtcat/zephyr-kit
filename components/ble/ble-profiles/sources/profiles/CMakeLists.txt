# Components

zephyr_include_directories(
  include
  .
)

add_subdirectory_ifdef(BLE_PROFILES_DIS  dis)
add_subdirectory_ifdef(BLE_PROFILES_GATT gatt)
add_subdirectory_ifdef(BLE_PROFILES_GAP  gap)
add_subdirectory_ifdef(BLE_PROFILES_HID  hid)


