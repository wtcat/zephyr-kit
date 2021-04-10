# Components

add_subdirectory(init)

zephyr_sources(
  observer.c
  timeconv.c
  mac_addr.c
  schedule_service.c
  blkio.c
  vibration.c
  env.c
)




