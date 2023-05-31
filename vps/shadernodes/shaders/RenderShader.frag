#version 450
#extension GL_ARB_separate_shader_objects : enable

layout( location = 0 ) flat in vec3 inNormal;
layout( location = 1 ) in vec3 inColor;

layout( location = 0 ) out vec4 outColor;

void main()
{
    vec3 lightdir = normalize( vec3( 0.0f, -1.0f, 1.0f ) );
    float light = max( dot( lightdir, inNormal ), 0.0f ) + .1f;
    vec3 color = light * inColor;

    outColor = vec4( color, 1.0f );
}
