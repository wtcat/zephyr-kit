# SPDX-License-Identifier: Apache-2.0

#zephyr_library()

add_subdirectory(AmbiqMicro)
add_subdirectory(apollo_mcu_lib)
zephyr_library_sources(soc.c)

zephyr_linker_sources(SECTIONS ccm.ld)


