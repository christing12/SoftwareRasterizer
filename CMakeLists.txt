cmake_minimum_required(VERSION 3.15)

if(${CMAKE_VERSION} VERSION_LESS 3.15)
    cmake_policy(VERSION ${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION})
else()
    cmake_policy(VERSION 3.15)
endif()

project(SoftwareRasterizer VERSION 1.0
                            DESCRIPTION " A NICE PROJ"
                            LANGUAGES CXX)



# Setting up some variables
set (CMAKE_CXX_STANDARD 11)
set(SDL2_PATH "C:/lib/SDL2-2.0.12")
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")
set(BIN_DIR ${CMAKE_SOURCE_DIR}/bin)
set(TARGET SoftwareRasterizer)

set(SOURCES
    src/Camera.cpp
    src/DisplayManager.cpp
    src/Engine.cpp
    src/EngineMath.cpp
    src/InputManager.cpp
    src/main.cpp
    src/Mesh.cpp
    src/Renderer.cpp
    src/RenderManager.cpp
    src/RenderObj.cpp
    src/Scene.cpp
    src/Texture.cpp
)

set(HEADERS
    inc/Buffer.h
    inc/Camera.h
    inc/DisplayManager.h
    inc/Engine.h
    inc/EngineMath.h
    inc/InputManager.h
    inc/Material.h
    inc/Mesh.h
    inc/Renderer.h
    inc/RenderManager.h
    inc/RenderObj.h
    inc/Scene.h
    inc/Shader.h
    inc/Texture.h
)


find_package(SDL2 REQUIRED)
find_package(OpenMP)

include_directories(${SDL2_INCLUDE_DIR})
include_directories(${PROJECT_SOURCE_DIR}/inc/)
include_directories(${PROJECT_SOURCE_DIR}/extern/)


# Setting up flags
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS} -g -ffast-math -msse4.1")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -g ")
set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -g ")


add_executable(${TARGET} ${SOURCES} ${HEADERS})
target_link_libraries(${TARGET} ${SDL2_LIBRARY})


# COPYING ASSETS TO THE RIGHT FOLDER
set(ASSETS_SRC "${CMAKE_SOURCE_DIR}/data")
set(ASSETS_DST "$<TARGET_FILE_DIR:${TARGET}>/data")
if (WIN32)
    set(ASSETS_CMD copy_directory)
    set(ASSETS_MSG "copy assets")
else()
    set(ASSETS_CMD create_symlink)
    set(ASSETS_MSG "Link assets")
endif()

install(TARGETS ${TARGET} RUNTIME DESTINATION ${BIN_DIR})