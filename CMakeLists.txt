cmake_minimum_required(VERSION 3.30.2)
project(GameEngine CXX)

option(USE_SDL "Use SDL as the platform abstraction layer instead of raw Vulkan/glfw" ON)
option(USE_VULKAN "Use SDL as the platform abstraction layer instead of raw Vulkan/glfw" OFF)

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
add_subdirectory(vendor)
if (USE_VULKAN)
    find_package(Vulkan REQUIRED)
    find_package(glfw3 REQUIRED)
endif ()

find_package(Glog REQUIRED)
find_package(GTest REQUIRED)
find_package(glm REQUIRED)

# Enable testing with CTest
enable_testing()

# Create a library for the engine
file(GLOB_RECURSE MODULE_FILES CONFIGURE_DEPENDS
        "${PROJECT_SOURCE_DIR}/src/*.ixx"
        "${PROJECT_SOURCE_DIR}/src/*.cppm"
)

if (USE_SDL)
    set(EXCLUDE_PATTERNS
            ".*vulkan/.*"
    )
    foreach (pattern IN LISTS EXCLUDE_PATTERNS)
        list(FILTER MODULE_FILES EXCLUDE REGEX "${pattern}")
    endforeach ()
elseif (USE_VULKAN)
    set(EXCLUDE_PATTERNS
            ".*sdl/.*"
    )
    foreach (pattern IN LISTS EXCLUDE_PATTERNS)
        list(FILTER MODULE_FILES EXCLUDE REGEX "${pattern}")
    endforeach ()
endif ()

# Include all source files
add_library(${PROJECT_NAME} STATIC)
target_sources(${PROJECT_NAME} PUBLIC FILE_SET CXX_MODULES FILES ${MODULE_FILES})

target_link_libraries(${PROJECT_NAME} PRIVATE glog::glog glm::glm)
target_include_directories(${PROJECT_NAME} PUBLIC src)

if (USE_SDL)
    target_link_libraries(${PROJECT_NAME} PRIVATE glog::glog SDL3::SDL3 SDL3_image::SDL3_image)
    target_include_directories(${PROJECT_NAME} PUBLIC ${SDL3_SOURCE_DIR}/include)
    target_compile_definitions(${PROJECT_NAME} PRIVATE
            $<$<BOOL:${USE_SDL}>:USE_SDL>
    )
elseif (USE_VULKAN)
    target_link_libraries(${PROJECT_NAME} PRIVATE glog::glog glfw Vulkan::Vulkan)
endif ()

# Add the test folder to the project
add_subdirectory(test)
add_subdirectory(benchmark)