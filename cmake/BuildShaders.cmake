############## Build SHADERS #######################
function(build_shaders SHADER_SOURCE_DIR SHADER_OUTPUT_DIR)
    find_program(GLSLC_EXECUTABLE NAMES glslc REQUIRED)
    # Get all GLSL shader files (.vert and .frag)
    file(GLOB_RECURSE GLSL_SOURCE_FILES
            "${SHADER_SOURCE_DIR}/*.frag"
            "${SHADER_SOURCE_DIR}/*.vert"
    )

    if (GLSL_SOURCE_FILES)
        message(STATUS "Found GLSL files in ${SHADER_SOURCE_DIR}")
    else()
        message(WARNING "No shader files found in ${SHADER_SOURCE_DIR}")
    endif()

    set(SPIRV_BINARY_FILES "")

    foreach (GLSL ${GLSL_SOURCE_FILES})
        get_filename_component(FILE_NAME ${GLSL} NAME)
        set(SPIRV "${SHADER_OUTPUT_DIR}/${FILE_NAME}.spv")

        add_custom_command(
                OUTPUT ${SPIRV}
                COMMAND ${GLSLC_EXECUTABLE} ${GLSL} -o ${SPIRV}
                DEPENDS ${GLSL}
                COMMENT "Compiling shader ${GLSL} -> ${SPIRV}"
                VERBATIM
        )

        list(APPEND SPIRV_BINARY_FILES ${SPIRV})
    endforeach()

    # Create a global variable to store the compiled shader files
    set(SPIRV_BINARY_FILES ${SPIRV_BINARY_FILES} PARENT_SCOPE)
endfunction()

build_shaders("${PROJECT_SOURCE_DIR}/shaders" "${PROJECT_SOURCE_DIR}/shaders")

add_custom_target(compile_shaders
    DEPENDS ${SPIRV_BINARY_FILES}
    COMMENT "Building all shaders"
)