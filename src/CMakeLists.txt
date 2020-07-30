include_directories(${PROJECT_SOURCE_DIR}/inc/)
include_directories(${PROJECT_SOURCE_DIR}/extern/)

set(SOURCES
    Camera.cpp
    DisplayManager.cpp
    Engine.cpp
    EngineMath.cpp
    InputManager.cpp
    main.cpp
    Mesh.cpp
    Renderer.cpp
    RenderManager.cpp
    RenderObj.cpp
    Scene.cpp
    Texture.cpp
)

set(TARGET SoftwareRasterizer)
add_executable(${TARGET} ${SOURCES})

target_link_libraries(${TARGET} ${SDL2_LIBRARY})

install(TARGETS ${TARGET} RUNTIME DESTINATION ${BIN_DIR})