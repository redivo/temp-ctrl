####################################################################################################
# Find packages and libraries
####################################################################################################
find_package (GTest REQUIRED)
find_package (Threads REQUIRED)

# Only compile tests for PC
if (NOT PLATFORM STREQUAL "pc")
    message(STATUS "Skipping tests compilation for platfortm " ${PLATFORM})
    return()
endif(NOT PLATFORM STREQUAL "pc")

include_directories (${INCLUDE_SRC_DIR})

####################################################################################################
# General configuration
####################################################################################################
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")


####################################################################################################
# Build
####################################################################################################
add_executable (test
    dummy.cc
)

target_link_libraries (test
    ${GTEST_BOTH_LIBRARIES}
    ${CMAKE_THREAD_LIBS_INIT}
)
