
zephyr_sources_ifdef(CONFIG_APOLLO_MSPI_NOR apollo_mspi_nor.c)

zephyr_include_directories_ifdef(CONFIG_APOLLO_SOC_FLAH, ./)
zephyr_sources_ifdef(CONFIG_APOLLO_SOC_FLAH soc_flash_apollo.c)


