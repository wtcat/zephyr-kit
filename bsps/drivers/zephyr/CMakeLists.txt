# Device driver subdirectories

zephyr_include_directories(.)

add_subdirectory(misc)
add_subdirectory_ifdef(CONFIG_SERIAL_HAS_DRIVER serial)
add_subdirectory_ifdef(CONFIG_GPIO gpio)
add_subdirectory_ifdef(CONFIG_I2C i2c)

