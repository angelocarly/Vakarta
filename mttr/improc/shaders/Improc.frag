#version 450
#extension GL_ARB_separate_shader_objects : enable

layout( location = 0 ) in vec2 inUV;

layout( location = 0 ) out vec4 outColor;

layout(push_constant) uniform PushConstantsBlock
{
    vec2 mSize;
    vec2 mPos;
    vec4 mColorTreshold;
    vec4 mReplacementColor;
    float mScale;
} PushConstants;

layout(binding = 0) uniform sampler2D uTexture;

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
    vec2 uv = inUV;
    uv *= PushConstants.mScale;
    uv += PushConstants.mPos / vec2( WIDTH, HEIGHT );
    uv.x *= PushConstants.mSize.y / PushConstants.mSize.x;
    uv.x *= aspect;

    if( uv.x < 0.0f || uv.x > 1.0f || uv.y < 0.0f || uv.y > 1.0f )
    {
        vec2 buv = inUV;
        buv *= PushConstants.mScale;
        buv += PushConstants.mPos / vec2( WIDTH, HEIGHT );
        buv.x *= aspect;
        outColor = vec4( background( buv ), 1.0f );
        return;
    }

    vec3 col = texture( uTexture, uv ).rgb;

    if( col.r < PushConstants.mColorTreshold.r )
    {
        col.r *= PushConstants.mReplacementColor.r;
    }
    if( col.g < PushConstants.mColorTreshold.g )
    {
        col.g *= PushConstants.mReplacementColor.g;
    }
    if( col.b < PushConstants.mColorTreshold.b )
    {
        col.b *= PushConstants.mReplacementColor.b;
    }

    outColor = vec4( col, 1.0f );
    return;
}
