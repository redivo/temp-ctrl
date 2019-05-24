include_directories (${SOURCE_INCLUDE_DIR})

# Build assembly files
if (COMPILE_ASSEMBLY)
    set(EXTRA_FILES_TO_COMPILE "${BINARY_SRC_DIR}/crt.o")
    add_custom_command(
        OUTPUT ${BINARY_SRC_DIR}/crt.o
        COMMAND arm-elf-as -mapcs-32 -mcpu=arm7tdmi -o ${BINARY_SRC_DIR}/crt.o ${SOURCE_SRC_DIR}/crt.S
        DEPENDS ${SOURCE_SRC_DIR}/crt.S
    )
endif (COMPILE_ASSEMBLY)

# Build .c files
if (COMPILE_C)
    add_executable(temp-ctrl
        main.c
        general.c
        sts_cfg.c
        infra_${PLATFORM}.c
        io_${PLATFORM}.c
        temperature_${PLATFORM}.c
        timer_${PLATFORM}.c
        ${EXTRA_FILES_TO_COMPILE}
    )
    if (EXTRA_FILES_TO_COMPILE)
        add_dependencies(temp-ctrl
            ${EXTRA_FILES_TO_COMPILE}
        )
    endif (EXTRA_FILES_TO_COMPILE)
endif (COMPILE_C)

# Build .hex file
if (COMPILE_HEX)
    add_custom_command(
        OUTPUT temp-ctrl.hex
        COMMAND arm-elf-objcopy -O ihex temp-ctrl temp-ctrl.hex
        DEPENDS temp-ctrl
    )
    add_custom_target (hexfile ALL DEPENDS "temp-ctrl.hex")

    # Installation
    install(CODE "execute_process(COMMAND lpc21isp ${BINARY_SRC_DIR}/temp-ctrl.hex ${TTY} 115200 12000)")
endif (COMPILE_HEX)
