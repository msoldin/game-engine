add_subdirectory(core)
add_subdirectory(ecs)
target_sources(${PROJECT_NAME} PRIVATE
        camera.cpp
        device.cpp
        game_object.cpp
        model.cpp
        pipeline.cpp
        render_system.cpp
        renderer.cpp
        swap_chain.cpp
        window.cpp
)