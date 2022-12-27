#version 450
#extension GL_ARB_separate_shader_objects : enable

layout ( std140, binding = 0 ) uniform UniformBufferObject
{
    mat4 mView;
    int mWidth;
    int mHeight;
} ubo;

//layout( location = 0 ) in vec3 inFragmentColor;
//layout( location = 1 ) in vec2 inTexCoord;
layout( location = 0 ) out vec4 outColor;

void main()
{
    outColor = vec4( vec3( 1.0f, 0.0f, 0.0f ), 1.0F );
}
