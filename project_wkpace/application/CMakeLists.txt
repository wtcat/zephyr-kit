#add_subdirectory_ifdef(CONFIG_USERSPACE usermode)
add_subdirectory(sensor_testsuites)

target_sources(app PRIVATE
  fatfs_unit_test.c
)

