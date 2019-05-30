# General
set(EXTRA_DEPS "")
set(EXTRA_LIBS "")

# Build basic library
add_library(basics STATIC
    infra.c
    io.c
    temperature.c
    timer.c
)
set(EXTRA_LIBS ${EXTRA_LIBS} basics)
set(EXTRA_DEPS ${EXTRA_DEPS} basics)

# Set parent scope
set(EXTRA_DEPS ${EXTRA_DEPS} PARENT_SCOPE)
set(EXTRA_LIBS ${EXTRA_LIBS} PARENT_SCOPE)
