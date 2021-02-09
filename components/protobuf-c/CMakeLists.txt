zephyr_library_named(protobuf)

zephyr_library_sources_ifdef(CONFIG_PROTOBUF
  protobuf-c.c
)

