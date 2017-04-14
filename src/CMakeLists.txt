include_directories (${INCLUDE_SRC_DIR})

SET(CMAKE_SYSTEM_NAME Generic)
SET(CMAKE_C_COMPILER arm-elf-gcc)
set(CMAKE_C_FLAGS "-c")

add_executable(tmp-ctrl
    mm.c
)
