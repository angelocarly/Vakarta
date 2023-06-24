
if(LINUX)
    set( GLSL_VALIDATOR "/usr/bin/glslangValidator" )
else()
    set( GLSL_VALIDATOR "/usr/local/bin/glslangValidator" )
endif()

foreach( GLSL ${GLSL_SOURCE_FILES} )
    get_filename_component( FILE_NAME ${GLSL} NAME )
    set(
            SPIRV
            "${SHADER_DIR}/${FILE_NAME}.spv"
    )
    message( ${SPIRV} )
    add_custom_command(
            OUTPUT ${SPIRV}
            COMMAND ${CMAKE_COMMAND} -E make_directory "${SHADER_DIR}"
            COMMAND ${GLSL_VALIDATOR} --target-env vulkan1.0 -V ${PROJECT_ROOT_DIR}/vkrt/graphics/${GLSL} -o ${SPIRV}
            DEPENDS ${GLSL} )
    list( APPEND SPIRV_BINARY_FILES ${SPIRV} )
endforeach( GLSL )

add_custom_target(
        Shaders
        DEPENDS ${SPIRV_BINARY_FILES}
)

add_dependencies( VkrtGraphics Shaders )
