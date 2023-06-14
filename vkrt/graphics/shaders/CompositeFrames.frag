#version 450
#extension GL_ARB_separate_shader_objects : enable

layout( location = 0 ) in vec2 inUV;

layout( location = 0 ) out vec4 outColor;

layout( binding = 0 ) uniform sampler2D inImage;

void main()
{
    vec4 col = vec4( 0 );
    if( inUV.x < 0.5 ) col = texture( inImage, inUV ).rgba;
    else col = vec4( 1, 0, 0, 1 );

    outColor = col;
}
