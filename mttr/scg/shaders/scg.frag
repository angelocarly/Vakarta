#version 450
#extension GL_ARB_separate_shader_objects : enable

layout( location = 0 ) in vec2 inUV;

layout( location = 0 ) out vec4 outColor;

layout(push_constant) uniform PushConstantsBlock
{
    vec2 mSize;
    vec2 mPos;
    vec4 mColorBackground;
    vec4 mReplacementColor;
    float mScale;
    float mEdgeSize;
} PushConstants;

layout(binding = 0) readonly buffer SymBuffer
{
    vec4 mSymBuffer[];
} uSymBuffer;

#define WIDTH 1600
#define HEIGHT 900

float sdCapsule( vec3 p, vec3 a, vec3 b, float r )
{
    vec3 pa = p - a, ba = b - a;
    float h = clamp( dot(pa,ba)/dot(ba,ba), 0.0, 1.0 );
    return length( pa - ba*h ) - r;
}

vec3 background( vec2 uv )
{
    vec2 t = fract( uv * 10.0f );
    if( t.x > 0.5f && t.y > 0.5f )
    {
        return vec3( 0.3f, 0.3f, 0.3f );
    }
    if( t.x < 0.5f && t.y < 0.5f )
    {
        return vec3( 0.3f, 0.3f, 0.3f );
    }
    return vec3( 0.1f, 0.1f, 0.1f );
}

void main()
{
    float aspect = WIDTH / float( HEIGHT );

    vec2 uv = inUV + vec2( -0.5f, -0.5f );
    uv *= PushConstants.mScale;
    uv += PushConstants.mPos / vec2( WIDTH, HEIGHT );
    uv.x *= aspect;
    vec2 p = uv;
    uv.x *= PushConstants.mSize.y / PushConstants.mSize.x;


    vec3 col = vec3( PushConstants.mColorBackground.rgb );

    // inner circle
    if( length( p ) < 0.5f + PushConstants.mEdgeSize / 2.0f && length( p ) > 0.5f - PushConstants.mEdgeSize / 2.0f )
    {
        col = PushConstants.mReplacementColor.rgb;
    }

    // outer circles
    int count = int( uSymBuffer.mSymBuffer[0].x * 20 );
    for( int i = 0; i < count; i++ )
    {
        float angle = float( i ) / float(count) * 6.28318530718f;
        vec2 pos = vec2( cos( angle ), sin( angle ) ) * 0.5f;
        if( abs( length( pos - p ) - .5 ) < PushConstants.mEdgeSize / 2.0f )
        {
            col = PushConstants.mReplacementColor.rgb * 0.5f;
        }

        for( int j = 0; j < count; j++ )
        {
            float angle = float( j ) / float(count) * 6.28318530718f + angle;
            vec2 ipos = pos + vec2( cos( angle ), sin( angle ) ) * 0.5f;
            if( abs( length( ipos - p ) - .5 ) < PushConstants.mEdgeSize / 2.0f )
            {
                col = PushConstants.mReplacementColor.rgb * 0.5f;
            }
        }
    }

    outColor = vec4( col, 1.0f );
    return;
}
