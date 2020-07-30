cmake_minimum_required(VERSION 3.15)

if(${CMAKE_VERSION} VERSION_LESS 3.15)
    cmake_policy(VERSION ${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION})
else()
    cmake_policy(VERSION 3.15)
endif()

project(SoftwareRasterizer VERSION 1.0
                            DESCRIPTION " A NICE PROJ"
                            LANGUAGES CXX)

set (CMAKE_CXX_STANDARD 11)
set(SDL2_PATH "C:/lib/SDL2-2.0.12")
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")
set(BIN_DIR ${CMAKE_SOURCE_DIR}/bin)


find_package(SDL2 REQUIRED)
find_package(OpenMP)

include_directories(${SDL2_INCLUDE_DIR})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS} -g -ffast-math -msse4.1")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -g ")
set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -g ")

add_subdirectory(src)