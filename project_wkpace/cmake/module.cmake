#Cmake for zephyr module

set(BSP_ROOT        ${CMAKE_CURRENT_SOURCE_DIR}/../bsps)
set(MODULES_ROOT    ${CMAKE_CURRENT_SOURCE_DIR}/../zephyr_pkg)
set(COMPONENTS_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/../components)

list(APPEND BOARD_ROOT ${BSP_ROOT})
list(APPEND SOC_ROOT   ${BSP_ROOT})
list(APPEND DTS_ROOT   ${BSP_ROOT})

# Base modules
list(APPEND ZEPHYR_EXTRA_MODULES 
    ${BSP_ROOT}/drivers
    ${COMPONENTS_ROOT}
)

# Optional modules list
if (EXISTS ${MODULES_ROOT}/gui)
list(APPEND ZEPHYR_EXTRA_MODULES 
    ${MODULES_ROOT}/gui)
endif()

if (EXISTS ${MODULES_ROOT}/fatfs)
list(APPEND ZEPHYR_EXTRA_MODULES 
    ${MODULES_ROOT}/fatfs)
endif()

if (EXISTS ${MODULES_ROOT}/littlefs)
list(APPEND ZEPHYR_EXTRA_MODULES 
    ${MODULES_ROOT}/littlefs)
endif()

if (EXISTS ${MODULES_ROOT}/nffs)
list(APPEND ZEPHYR_EXTRA_MODULES 
    ${MODULES_ROOT}/nffs)
endif()

if (EXISTS ${MODULES_ROOT}/segger)
list(APPEND ZEPHYR_EXTRA_MODULES 
    ${MODULES_ROOT}/segger)
endif()

if (EXISTS ${MODULES_ROOT}/mcumgr)
list(APPEND ZEPHYR_EXTRA_MODULES 
    ${MODULES_ROOT}/mcumgr)
endif()

if (EXISTS ${MODULES_ROOT}/mcuboot)
list(APPEND ZEPHYR_EXTRA_MODULES 
    ${MODULES_ROOT}/mcuboot)
endif()

if (EXISTS ${MODULES_ROOT}/loramac-node)
list(APPEND ZEPHYR_EXTRA_MODULES 
    ${MODULES_ROOT}/loramac-node)
endif()
