cmake_minimum_required(VERSION 3.22.1)
project(GameEngine)

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
else ()
    message(WARNING "Unsupported compiler, exception handling flags not set")
endif ()

set(CMAKE_CXX_CPPCHECK "cppcheck")

# Create a library for the engine
file(GLOB_RECURSE SOURCE_FILES CONFIGURE_DEPENDS
        "${PROJECT_SOURCE_DIR}/src/*.cpp"
)

file(GLOB_RECURSE HEADER_FILES CONFIGURE_DEPENDS
        "${PROJECT_SOURCE_DIR}/include/*.h"
        "${PROJECT_SOURCE_DIR}/include/*.hpp"
        "${PROJECT_SOURCE_DIR}/include/*.inl"
)

# Exclude all source files in a "deprecated" directory
list(FILTER SOURCE_FILES EXCLUDE REGEX ".*/deprecated/.*")
list(FILTER HEADER_FILES EXCLUDE REGEX ".*/deprecated/.*")

# Include all source files
add_library(${PROJECT_NAME} STATIC ${SOURCE_FILES} ${HEADER_FILES})

#Add the SDL2 image and sdl2 ttf library also
find_package(Vulkan REQUIRED)
find_package(glfw3 REQUIRED)
find_package(glm REQUIRED)
find_package(Glog REQUIRED)

# Include directories for the engine library
target_link_libraries(${PROJECT_NAME} glog::glog glfw Vulkan::Vulkan glm::glm)
target_include_directories(${PROJECT_NAME} PUBLIC include)

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