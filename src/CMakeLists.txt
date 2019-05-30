include_directories (${SOURCE_INCLUDE_DIR})

# General
set(EXTRA_DEPS "")
set(EXTRA_LIBS "")

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/${PLATFORM})

# Build main program
add_executable(temp-ctrl
    main.c
    general.c
    sts_cfg.c
)
add_dependencies(temp-ctrl ${EXTRA_DEPS})
target_link_libraries(temp-ctrl ${EXTRA_LIBS})
