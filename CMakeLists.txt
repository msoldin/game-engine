cmake_minimum_required(VERSION 3.22.1)
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

message(STATUS "${SPIRV_BINARY_FILES}")

############## Build Library #######################

# Create a library for the engine
add_library(${PROJECT_NAME})
add_subdirectory(src)

#Add the SDL2 image and sdl2 ttf librarie also
find_package(Vulkan REQUIRED)
find_package(glfw3 REQUIRED)
find_package(glm REQUIRED)
find_package(Glog REQUIRED)

# Include directories for the engine library
target_link_libraries(${PROJECT_NAME} glog::glog glfw Vulkan::Vulkan glm::glm)
target_include_directories(${PROJECT_NAME} PUBLIC include)