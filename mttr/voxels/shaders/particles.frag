#version 450
#extension GL_ARB_separate_shader_objects : enable

layout( location = 0 ) in vec2 inUV;

layout( location = 0 ) out vec4 outColor;

layout(push_constant) uniform PushConstantsBlock
{
    mat4 mMVP;
    mat4 mViewProjection;
} PushConstants;

#define WORLD_SIZE 128
layout( binding = 0 ) readonly buffer InWorldDataBlock
{
    vec4 mWorldData[ WORLD_SIZE * WORLD_SIZE * WORLD_SIZE ];
} inWorldData;

void main()
{
    // UV
    vec2 uv = inUV * 2.0f - 1.0f;

    float near = 1.0f;
    float far = 1000.0f;
    vec4 worldpos4 = ( ( inverse( PushConstants.mMVP ) * vec4( uv, -1, 1 ) ) );
    vec3 worldpos = worldpos4.xyz / worldpos4.w * near;
    vec3 worlddir = normalize( ( -( PushConstants.mViewProjection ) * vec4( -uv * ( far - near ), far + near, far - near ) ).xyz);

    // Ray
    RayResult box = traceOuterBox( worldpos, worlddir );
    RayResult res = traceRay( worldpos, worlddir );

    vec3 col = vec3( 0 );
    if( ( box.hit && !res.hit ) || ( box.hit && box.dist < res.dist ) )
    {
        // Outer box hit
        col = box.color;
    }
    else if( ( res.hit && !box.hit ) || ( res.hit && res.dist < box.dist ) )
    {
        // World hit
        vec3 lightDir = normalize( vec3( 0.7f, 1.0f, 0.6f ) );
        col = res.color;
        col *= max( dot( res.normal, -lightDir ), .3f );
        if( res.hit )
        {
            // Shadow mapping
            vec3 lightPos = res.position - lightDir * 0.001f;
            RayResult lightRes = traceRay( lightPos, lightDir );

            if( lightRes.hit )
            {
                col *= 0.2f;
            }
        }
    }

    outColor = vec4( col, 1.0f );
}
