# General
set(EXTRA_DEPS "")
set(EXTRA_LIBS "")

# Build assembly files
add_custom_command(
    OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/crt.o
    COMMAND arm-none-eabi-as -mapcs-32 -mcpu=arm7tdmi -o ${CMAKE_CURRENT_BINARY_DIR}/crt.o ${CMAKE_CURRENT_SOURCE_DIR}/crt.S
    DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/crt.S
)
add_custom_target(crt ALL DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/crt.o)
set(EXTRA_DEPS ${EXTRA_DEPS} crt)
set(EXTRA_LIBS ${EXTRA_LIBS} ${CMAKE_CURRENT_BINARY_DIR}/crt.o)

# Build basic library
add_library(basics STATIC
    infra.c
    io.c
    temperature.c
    timer.c
    ${CMAKE_CURRENT_BINARY_DIR}/crt.o
)
add_dependencies(basics crt)
set(EXTRA_LIBS ${EXTRA_LIBS} basics)
set(EXTRA_DEPS ${EXTRA_DEPS} basics)

# Build .hex file
add_custom_command(
    OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/temp-ctrl.hex
    COMMAND arm-none-eabi-objcopy -O ihex ${CMAKE_CURRENT_BINARY_DIR}/../temp-ctrl ${CMAKE_CURRENT_BINARY_DIR}/temp-ctrl.hex
    DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/../temp-ctrl
)
add_custom_target(hexfile ALL DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/temp-ctrl.hex)

# Installation
install(CODE "execute_process(COMMAND lpc21isp ${CMAKE_CURRENT_BINARY_DIR}/temp-ctrl.hex ${TTY} 115200 12000)")

# Set parent scope
set(EXTRA_DEPS ${EXTRA_DEPS} PARENT_SCOPE)
set(EXTRA_LIBS ${EXTRA_LIBS} PARENT_SCOPE)
