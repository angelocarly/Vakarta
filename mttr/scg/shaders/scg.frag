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
    int mSymBuffer[];
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

float sdSegment( in vec2 p, in vec2 a, in vec2 b )
{
    vec2 pa = p-a, ba = b-a;
    float h = clamp( dot(pa,ba)/dot(ba,ba), 0.0, 1.0 );
    return length( pa - ba*h );
}

float shape( vec2 p, int sides, float size, float r )
{
    vec2 pos = vec2( 0 );
    float dist = size;
    float prevangle = float( sides - 1 ) / float(sides) * 6.28318530718f + r;
    vec2 prevpos = pos + vec2( cos( prevangle ), sin( prevangle ) ) * dist;
    float t = 99999999.0f;
    for( int i = 0; i < sides; i++ )
    {
        float angle = float( i ) / float(sides) * 6.28318530718f + r;
        vec2 tpos = pos + vec2( cos( angle ), sin( angle ) ) * dist;
        float l = sdSegment( p, tpos, prevpos );
        t = min( t, abs( l ) );
        prevpos = tpos;
    }

    return t;
}

vec3 firstshape( vec2 p )
{

    vec3 col = vec3( PushConstants.mColorBackground.rgb );

    // inner circle
    vec2 pos = vec2( 0 );
    float l = max( 1.0f - abs( length( pos - p ) - .5 ), .0f ) * PushConstants.mEdgeSize;
    col += pow( l, 30 ) * PushConstants.mReplacementColor.rgb;

    // outer circles
    int count = int( uSymBuffer.mSymBuffer[0].x * 20 );
    for( int i = 0; i < count; i++ )
    {
        float angle = float( i ) / float(count) * 6.28318530718f;

        float t = shape( p - pos, 3, 1.0f, angle );
        col += pow( max( ( 1.0 - t ) * PushConstants.mEdgeSize, 0.0 ), 30 ) * PushConstants.mReplacementColor.rgb;

        float dist = 1.0f;
        vec2 pos = vec2( cos( angle ), sin( angle ) ) * dist;
        {
            float l = max( 1.0f - abs( length( pos - p ) - .5 ), .0f ) * PushConstants.mEdgeSize;
            col += pow( l, 30 ) * PushConstants.mReplacementColor.rgb;
        }

        for( int j = 0; j < 1; j++ )
        {
            float angle = float( j ) / float(count) * 6.28318530718f + angle;
            vec2 ipos = pos + vec2( cos( angle ), sin( angle ) ) * dist;
            {
                float l = max( 1.0f - abs( length( ipos - p ) - .5 ), .0f ) * PushConstants.mEdgeSize;
                col += pow( l, 30 ) * PushConstants.mReplacementColor.rgb;
            }
        }
    }

    return col;
}

vec3 second( vec2 p )
{
    vec3 col = vec3( PushConstants.mColorBackground.rgb );

    // Registers
    int ir = 0;
    int reg[8];
    float radius = 0.5f;
    for( int i = 0; i < 8; i++ ) reg[ i ] = 0;

    for( int instcount = 0; instcount < 100; instcount++ ) // max 100 steps
    {
        int data = uSymBuffer.mSymBuffer[ir];
        int inst = data & 0xFF;
        int arg0 = ( data >> 8 ) & 0xFF;
        int arg1 = ( data >> 16 ) & 0xFF;
        int arg2 = ( data >> 24 ) & 0xFF;

        switch( inst )
        {
            case 0: // exit
            {
                return col;
            }
            case 1: // increase register arg0 with arg1
            {
                reg[ arg0 ] += arg1;
                break;
            }
            case 2: // decrease register arg0 with arg1
            {
                reg[ arg0 ] -= arg1;
                break;
            }
            case 3: // increase register arg0 with reg arg1
            {
                reg[ arg0 ] += reg[ arg1 ];
                break;
            }
            case 4: // decrease register arg0 with reg arg1
            {
                reg[ arg0 ] -= reg[ arg1 ];
                break;
            }
            case 5: // increase rangle with arg0
            {
                reg[ arg0 ] += arg1;
                break;
            }
            case 6: // decrease rangle with arg0
            {
                reg[ arg0 ] -= arg1;
                break;
            }
            case 7: // jmp to arg0
            {
                ir = arg0;
                continue;
            }
            case 0xC: // Set range
            {
                radius = float( arg0 );
                break;
            }
            case 0xD: // set range to inner range ngon with n=arg1, outer range of arg2
            {
                float alpha = 6.28318530718f / float( arg0 );
                radius = radius / cos( alpha / 2.0f );
                break;
            }
            case 0xE: // shape with range register at arg0
            {
                float t = shape( p, arg0, radius, float( reg[ arg2 ] ) / 255.0f * 6.28318530718f );
                t = pow( max( ( 1.0 - t ) , 0.0 ), 30 * PushConstants.mEdgeSize );
                vec2 pos = vec2(0);
                col += t * PushConstants.mReplacementColor.rgb;
                break;
            }
            case 0xF: // circle with range register at arg0
            {
                vec2 pos = vec2(0);
                float l = max( 1.0f - abs( length( pos - p ) - radius ), .0f);
                col += pow(l, 30 * PushConstants.mEdgeSize) * PushConstants.mReplacementColor.rgb;
                break;
            }
        }

        ir++;
    }


    return col;
}

vec3 third( vec2 p )
{
    vec3 col = vec3( PushConstants.mColorBackground.rgb );

    int ir = 0;
    int symindex = 0;
    int sym[ 6 ];
    int range[ 6 ];
    int radius[ 6 ];
    vec2 pos = vec2( 0 );
    int maxlevel = 0;

    for( int instcount = 0; instcount < 100; instcount++ ) // max 100 steps
    {
        int data = uSymBuffer.mSymBuffer[ir];
        int inst = data & 0xFF;
        int arg0 = ( data >> 8 ) & 0xFF;
        int arg1 = ( data >> 16 ) & 0xFF;
        int arg2 = ( data >> 24 ) & 0xFF;

        switch( inst )
        {
            case 0: // exit
            {
                return col;
            }
            case 1: // set symmetry
            {
                sym[ instcount ] = arg0;
                radius[ instcount ] = arg1;
                range[ instcount ] = arg2;
                maxlevel = max( maxlevel, instcount );
                break;
            }
            case 0xF: // Draw (circle)
            {
                vec2 pos[ 6 ];
                float angle[ 6 ];
                int indices[ 6 ];
                for( int i = 0; i < 6; i++ ) indices[ i ] = 0;
                int level = 0;
                int i = indices[ level ];
                int ops = 0;

                while( true )
                {
                    ops++;
                    if( ops > 100 ) return vec3( 1, 0, 0 );

                    // Check if we're done for this level
                    if( i == sym[ level ] )
                    {
                        level--;

                        if( level < 0 ) return col;

                        i = indices[ level ];
                        i++;
                        continue;
                    }

                    // We're on a level with index i
                    // Do a draw
                    int count = sym[ level ];

                    float cangle = float(i) / float(count) * 6.28318530718f;
                    if( level > 0 ) cangle += angle[ level - 1 ];

                    vec2 cpos = vec2(cos(cangle), sin(cangle)) * range[ level ];
                    if( level > 0 ) cpos += pos[ level - 1 ];

                    // Children
                    if( level < maxlevel )
                    {
                        indices[ level ] = i;
                        pos[ level ] = cpos;
                        angle[ level ] = cangle;
                        level++;
                        i = 0;
                        continue;
                    }

                    // Draw a circle
                    {
                        float l = max(1.0f - abs(length(cpos - p) - radius[ level ]), .0f) * PushConstants.mEdgeSize;
                        col += pow(l, 30) * PushConstants.mReplacementColor.rgb;
                    }

                    i++;
                }
                break;
            }
        }

        ir++;
    }


    return col;
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

    vec3 col = third( p );

    outColor = vec4( col, 1.0f );
    return;
}
