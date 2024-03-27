project(GameEngine)

############## Build SHADERS #######################

# get all .vert and .frag files in shaders directory
file(GLOB_RECURSE GLSL_SOURCE_FILES
        "${PROJECT_SOURCE_DIR}/shaders/*.frag"
        "${PROJECT_SOURCE_DIR}/shaders/*.vert"
)

foreach (GLSL ${GLSL_SOURCE_FILES})
  get_filename_component(FILE_NAME ${GLSL} NAME)
  set(SPIRV "${PROJECT_SOURCE_DIR}/shaders/${FILE_NAME}.spv")
  add_custom_command(
          OUTPUT ${SPIRV}
          COMMAND glslc ${GLSL} -o ${SPIRV}
          DEPENDS ${GLSL})
  list(APPEND SPIRV_BINARY_FILES ${SPIRV})
endforeach (GLSL)

message(STATUS "Configuring GameEngine")

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/cmake/")

# Add source files for the engine
set(SOURCES
        src/model.cpp
        src/window.cpp
        src/device.cpp
        src/pipeline.cpp
        src/swap_chain.cpp
        src/example_app.cpp
        src/game_object.cpp
        src/renderer.cpp
        src/render_system.cpp
        src/camera.cpp
        src/deprecated/GameEngine.cpp
        src/deprecated/RenderManager.cpp
        src/deprecated/InputManager.cpp
        src/deprecated/SceneManager.cpp
        src/deprecated/Timer.cpp
)

# Add header files for the engine
set(HEADERS
        include/model.h
        include/device.h
        include/window.h
        include/pipeline.h
        include/swap_chain.h
        include/example_app.h
        include/game_object.h
        include/renderer.h
        include/render_system.h
        include/deprecated/GameEngine.h
        include/deprecated/RenderManager.h
        include/deprecated/InputManager.h
        include/deprecated/InputDefinitions.h
        include/deprecated/Scene.h
        include/deprecated/SceneManager.h
        include/deprecated/Timer.h
        include/camera.h
)

message(STATUS "${SPIRV_BINARY_FILES}")

# Create a library for the engine
add_library(GameEngineLib ${SOURCES} ${HEADERS} ${SPIRV_BINARY_FILES})

#Add the SDL2 image and sdl2 ttf librarie also
find_package(Vulkan REQUIRED)
find_package(glfw3 REQUIRED)
find_package(glm REQUIRED)
#Deprecated needs to be removed
find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_ttf REQUIRED)
find_package(Glog REQUIRED)

# Include directories for the engine library
target_link_libraries(GameEngineLib SDL2::Main SDL2::Image SDL2::TTF glog::glog glfw Vulkan::Vulkan glm::glm)
target_include_directories(GameEngineLib PUBLIC include)