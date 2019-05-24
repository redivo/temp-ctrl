cmake_minimum_required(VERSION 2.6)
project (temp-ctrl)


####################################################################################################
# Sets
####################################################################################################
set(SOURCE_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)
set(SOURCE_SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(BINARY_SRC_DIR ${CMAKE_CURRENT_BINARY_DIR}/src)


####################################################################################################
# Platform
####################################################################################################
# Use "camke -D PLATFORM=<desired target>"

if (PLATFORM STREQUAL "pucrsDev")
    message(STATUS "Building for PUCRS Devboard - " ${PLATFORM})
    set(CMAKE_C_COMPILER arm-elf-gcc)
    #set(CMAKE_C_FLAGS ${CMAKE_C_FLAGS} "-H")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -nostartfiles -T${SOURCE_SRC_DIR}/lpc2378_flash.ld")
    set(COMPILE_C TRUE)
    set(COMPILE_ASSEMBLY TRUE)
    set(COMPILE_HEX TRUE)

elseif (PLATFORM STREQUAL "pc")
    set(COMPILE_C TRUE)
    set(COMPILE_TESTS TRUE)
    message(STATUS "Building for PC - " ${PLATFORM})

else(PLATFORM STREQUAL "pucrsDev")
    message(FATAL_ERROR "Unknown platform - " ${PLATFORM})

endif(PLATFORM STREQUAL "pucrsDev")

#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O0") # debug, no optimisation
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --coverage") # enabling coverage


####################################################################################################
# Subdirectories
####################################################################################################
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/src)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/test)
