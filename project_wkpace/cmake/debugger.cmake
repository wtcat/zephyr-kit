#Cmake for debug

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR})
set(GDB_SERVER   JLinkGDBServerExe)
set(JLINK_EXE    JLinkExe)
set(DEVICE_MODEL AMA3B2KK-KBR)
set(CMAKE_DEBUGER ${ZEPHYR_SDK_INSTALL_DIR}/arm-zephyr-eabi/bin/arm-zephyr-eabi-gdb)



add_custom_target(gdb
    COMMAND
    ${CMAKE_DEBUGER} -x ${CMAKE_CURRENT_SOURCE_DIR}/gdbx.sh ${EXECUTABLE_OUTPUT_PATH}/zephyr.elf
)

add_custom_target(gdbserver
    COMMAND
    ${GDB_SERVER} -select USB -device ${DEVICE_MODEL} -endian little -if SWD -speed 2000 -ir -noLocalhostOnly &
)

add_custom_target(load
    COMMAND
    ${JLINK_EXE} -autoconnect 1 -device ${DEVICE_MODEL} -if swd -speed 2000 -commandfile ${CMAKE_CURRENT_SOURCE_DIR}/download.jlink
)

