cmake_minimum_required(VERSION 2.6)
project (temp-ctrl)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O0") # debug, no optimisation
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --coverage") # enabling coverage

####################################################################################################
# Sets
####################################################################################################
set (INCLUDE_SRC_DIR ${CMAKE_SOURCE_DIR}/include)
set (SOURCE_SRC_DIR ${CMAKE_SOURCE_DIR}/src)

####################################################################################################
# Subdirectories
####################################################################################################
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/src)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/test)
