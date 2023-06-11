#version 450
#extension GL_ARB_separate_shader_objects : enable

layout( location = 0 ) in vec2 inUV;

layout( location = 0 ) out vec4 outColor;

layout(push_constant) uniform PushConstants {
    int frameIndex;
} pcs;

void main()
{
    vec3 col = vec3( 0 );
    if( pcs.frameIndex == 0 )
    {
        col = vec3( 1, 0, 0 );
    }
    else if( pcs.frameIndex == 1 )
    {
        col = vec3( 0, 1, 0 );
    }
    else if( pcs.frameIndex == 2 )
    {
        col = vec3( 0, 0, 1 );
    }
    outColor = vec4( col, 1.0f );
}
