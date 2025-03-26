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

# Add CMake Script folder
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")
include(BuildShaders)

#Add required libs for build
find_package(Vulkan REQUIRED)
find_package(glfw3 REQUIRED)
find_package(glm REQUIRED)
find_package(Glog REQUIRED)
find_package(GTest REQUIRED)
#add_subdirectory(vendor)

# Enable testing with CTest
enable_testing()

# Create a library for the engine
file(GLOB_RECURSE MODULE_FILES CONFIGURE_DEPENDS
    "${PROJECT_SOURCE_DIR}/src/*.ixx"
    "${PROJECT_SOURCE_DIR}/src/*.cppm"
)

# Include all source files
add_library(${PROJECT_NAME} STATIC)
target_sources(${PROJECT_NAME} PUBLIC FILE_SET CXX_MODULES FILES ${MODULE_FILES})

# Include directories for the engine library
target_link_libraries(${PROJECT_NAME} PRIVATE glog::glog glfw Vulkan::Vulkan glm::glm)
target_include_directories(${PROJECT_NAME} PUBLIC src)

# Add the test folder to the project
add_subdirectory(test)
add_subdirectory(benchmark)