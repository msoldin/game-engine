cmake_minimum_required(VERSION 3.30.2)
project(GameEngine CXX)

############## Build Library #######################
# Check the compiler type
if (CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
    # Visual Studio compiler
    # Ignore till exceptions are fixed
    # set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /EHc-")
elseif (CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    # GCC or Clang compiler
    # Ignore till exceptions are fixed
    # set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-exceptions")
    set(CMAKE_CXX_STANDARD 23)
    set(CMAKE_CXX_EXTENSIONS OFF)
    set(CMAKE_CXX_STANDARD_REQUIRED ON)
else ()
    message(WARNING "Unsupported compiler, exception handling flags not set")
endif ()

add_subdirectory(vendor)

# Create a library for the engine
file(GLOB_RECURSE MODULE_FILES CONFIGURE_DEPENDS
    "${PROJECT_SOURCE_DIR}/src/*.ixx"
    "${PROJECT_SOURCE_DIR}/src/*.cppm"
)

# Include all source files
add_library(${PROJECT_NAME} STATIC)
target_sources(${PROJECT_NAME} PUBLIC FILE_SET CXX_MODULES FILES ${MODULE_FILES})

#Add the SDL2 image and sdl2 ttf library also
find_package(Vulkan REQUIRED)
find_package(glfw3 REQUIRED)
find_package(glm REQUIRED)
find_package(Glog REQUIRED)

# Include directories for the engine library
target_link_libraries(${PROJECT_NAME} glog::glog glfw Vulkan::Vulkan glm::glm raylib)
target_include_directories(${PROJECT_NAME} PUBLIC include src)

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