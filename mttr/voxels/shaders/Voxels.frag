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
    int mWorldData[ WORLD_SIZE * WORLD_SIZE * WORLD_SIZE ];
} inWorldData;

float random(vec2 st) {
    return fract(sin(dot(st.xy,
    vec2(12.9898,78.233)))*
    43758.5453123);
}

struct RayResult
{
    vec3 color;
    vec3 position;
    vec3 normal;
    bool hit;
};

RayResult traceRay( vec3 origin, vec3 direction )
{
    // Check whether the ray will intersect the world box
    bool intersect = false;
    float startT = 1000000000000.0f;

    if( origin.x >= 0 && origin.y >= 0 && origin.z >= 0 && origin.x < WORLD_SIZE && origin.y < WORLD_SIZE && origin.z < WORLD_SIZE )
    {
        intersect = true;
        startT = 0.0f;
    }
    else for( int i = 0; i < 3; i++ )
    {
        // No intersections possible

        // Forward ray, check intersect at position 0
        float next = 0.0f;
        float axisdist = next - origin[ i ];
        float raydist = axisdist / direction[ i ];
        vec3 p = origin + direction * raydist;

        if( raydist < 0.0f ) continue;

        // Assert p is inside the world box
        int nextIndex = ( i + 1 ) % 3;
        if ( p[ nextIndex ] >= 0.0f && p[ nextIndex ] < WORLD_SIZE)
        {
            nextIndex = ( nextIndex + 1 ) % 3;
            if( p[ nextIndex ] >= 0.0f && p[ nextIndex ] < WORLD_SIZE )
            {
                intersect = true;
                if( raydist < startT ) startT = raydist;
            }
        }

        next = WORLD_SIZE;
        axisdist = next - origin[ i ];
        raydist = axisdist / direction[ i ];
        p = origin + direction * raydist;

        if( raydist < 0.0f ) continue;

        // Assert p is inside the world box
        nextIndex = ( i + 1 ) % 3;
        if ( p[ nextIndex ] >= 0.0f && p[ nextIndex ] < WORLD_SIZE)
        {
            nextIndex = ( nextIndex + 1 ) % 3;
            if( p[ nextIndex ] >= 0.0f && p[ nextIndex ] < WORLD_SIZE )
            {
                intersect = true;
                if( raydist < startT ) startT = raydist;
            }
        }
    }

    // the current position we're in
    vec3 rayPos = origin + direction * startT * 0.999f;
    ivec3 pos = ivec3( floor( rayPos ) );

    if( !intersect ) return RayResult( vec3( 0 ), vec3( 0 ), vec3( 0 ), false );

    // distance of the ray to the closest axis plane it'll intersect
    vec3 sidepos = vec3( -1.0f );
    for( int i = 0; i < 3; i++ )
    {
        vec3 next = rayPos;
        if( direction[i] > 0.0f )
        {
            next[i] = floor( rayPos[i] ) + 1.0f;
        }
        else if( direction[i] < 0.0f )
        {
            next[i] = ceil( rayPos[i] ) - 1.0f;
        }
        float nextStep = abs( next[i] - rayPos[i] );
        sidepos[i] = nextStep / abs( direction[i] );
    }

    // The distance the ray traverses for each axis unit cell
    vec3 deltaDist = vec3( 0.0f );
    for( int i = 0; i < 3; i++ )
    {
        deltaDist[i] = abs( 1.0f / direction[i] );
    }

    // the traversed distance for each dimension
    vec3 tDist = vec3(0.0f);

    // The distance on the ray
    float t = 0.0f;

    vec3 normal;
    for( int c = 0; c < 800; c++ )
    {
        vec3 tMax = vec3(0.0f);
        for (int i = 0; i < 3; i++)
        {
            // Only use the sidepos when it's still valid ( >= 0 )
            if ( sidepos[i] >= 0 ) tMax[i] = sidepos[i];
            else tMax[i] = deltaDist[i];
        }

        vec3 newTDist = tDist + tMax;

        if (newTDist.x < newTDist.y)
        {
            if (newTDist.x < newTDist.z)
            {
                tDist.x += tMax.x;
                t = tDist.x;
                pos.x += int(sign(direction.x));
                sidepos.x = -1;
                normal = vec3( sign( direction.x ), 0, 0 );
            }
            else
            {
                tDist.z += tMax.z;
                t = tDist.z;
                pos.z += int(sign(direction.z));
                sidepos.z = -1;
                normal = vec3( 0, 0, sign( direction.z ) );
            }
        }
        else
        {
            if (newTDist.y < newTDist.z)
            {
                tDist.y += tMax.y;
                t = tDist.y;
                pos.y += int(sign(direction.y));
                sidepos.y = -1;
                normal = vec3( 0, sign( direction.y ), 0 );
            }
            else
            {
                tDist.z += tMax.z;
                t = tDist.z;
                pos.z += int(sign(direction.z));
                sidepos.z = -1;
                normal = vec3( 0, 0, sign( direction.z ) );
            }
        }

        if( pos.x >= 0 && pos.y >= 0 && pos.z >= 0 && pos.x < WORLD_SIZE && pos.y < WORLD_SIZE && pos.z < WORLD_SIZE )
        {
            int theId = inWorldData.mWorldData[pos.x * WORLD_SIZE * WORLD_SIZE + pos.y * WORLD_SIZE + pos.z];
            if (theId >= 6) continue;

            if (theId > 0)
            {
                vec3 color = vec3(0);
                switch (theId)
                {
                    case 1: color = vec3(251, 133, 0) / 255; break;
                    case 2: color = vec3(255, 183, 3) / 255; break;
                    case 3: color = vec3(2, 48, 71) / 255; break;
                    case 4: color = vec3(33, 158, 188) / 255; break;
                    case 5: color = vec3(142, 202, 230) / 255; break;
                    default : color = vec3(1, 0, 0); break;
                }
                return RayResult( color, origin + direction * t, normal, true );
            }
        }
        else if( c > 1 ) break;
    }

    return RayResult( vec3( 0 ), vec3( 0 ), vec3( 0 ), false );
}

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
    RayResult res = traceRay( worldpos, worlddir );
    vec3 lightDir = normalize( vec3( 0.7f, 1.0f, 0.6f ) );
    vec3 col = res.color;
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
    outColor = vec4( col, 1.0f );
}
