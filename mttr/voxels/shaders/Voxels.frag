#version 450
#extension GL_ARB_separate_shader_objects : enable

layout( location = 0 ) in vec2 inUV;

layout( location = 0 ) out vec4 outColor;

layout(push_constant) uniform PushConstants
{
    ivec2 screenSize;
    ivec2 _pad;
    vec4 mCameraPosition;
} pcs;

float sdf( vec3 p )
{
    return length( p ) - 1.0f;
}

vec3 traceRay( vec3 origin, vec3 direction )
{
    float t = 0;
    for( int i = 0; i < 200; i++ )
    {
        float d = sdf( origin + direction * t );
        if( d < 0.001f )
        {
            return vec3( 1.0f, 0.0f, 1.0f );
        }

        t += d;
    }
    return vec3( 0.0f, 0.0f, 0.0f );
}

void main()
{
    // UV
    vec2 uv = inUV * 2.0f - 1.0f;
    float aspect = float( pcs.screenSize.x ) / float( pcs.screenSize.y );
    uv.x *= aspect;

    vec3 origin = vec3( uv, -3.0f ) + pcs.mCameraPosition.xyz;
    vec3 direction = vec3( 0.0f, 0.0f, 1.0f );
    vec3 col = traceRay( origin, direction );
    outColor = vec4( col, 1.0f );
}
