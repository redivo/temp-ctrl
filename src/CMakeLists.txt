include_directories (${SOURCE_INCLUDE_DIR})

# Build .c files
add_executable(temp-ctrl
    main.c
    general.c
    sts_cfg.c
    infra_${PLATFORM}.c
    io_${PLATFORM}.c
    temperature_${PLATFORM}.c
    timer_${PLATFORM}.c
    crt.o
)

# Build assembly files
add_custom_command(
    OUTPUT crt.o
    COMMAND arm-elf-as -mapcs-32 -mcpu=arm7tdmi -o ${BINARY_SRC_DIR}/crt.o ${SOURCE_SRC_DIR}/crt.S
    DEPENDS ${SOURCE_SRC_DIR}/crt.S
)

# Build .hex file
add_custom_command(
    OUTPUT temp-ctrl.hex
    COMMAND arm-elf-objcopy -O ihex temp-ctrl temp-ctrl.hex
    DEPENDS temp-ctrl
)
add_custom_target (hexfile ALL DEPENDS "temp-ctrl.hex")
