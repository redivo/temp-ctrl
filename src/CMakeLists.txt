include_directories (${INCLUDE_SRC_DIR})

set(TARGET "pc")

add_executable(temp-ctrl
    main.c
    general.c
    sts_cfg.c
    infra_${TARGET}.c
    io_${TARGET}.c
    temperature_${TARGET}.c
    timer_${TARGET}.c
)
