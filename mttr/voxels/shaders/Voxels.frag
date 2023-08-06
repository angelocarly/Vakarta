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
    float dist;
};

float sdCapsule( vec3 p, vec3 a, vec3 b, float r )
{
    vec3 pa = p - a, ba = b - a;
    float h = clamp( dot(pa,ba)/dot(ba,ba), 0.0, 1.0 );
    return length( pa - ba*h ) - r;
}

float sdfOuterBox( vec3 p, float t )
{
    float r = 0.2f * t / WORLD_SIZE;
    float w = WORLD_SIZE;
    float d =   sdCapsule( p, vec3( 0, 0, 0 ), vec3( w, 0, 0 ), r );
    d = min( d, sdCapsule( p, vec3( 0, 0, w ), vec3( w, 0, w ), r ) );
    d = min( d, sdCapsule( p, vec3( 0, w, 0 ), vec3( w, w, 0 ), r ) );
    d = min( d, sdCapsule( p, vec3( 0, w, w ), vec3( w, w, w ), r ) );

    d = min( d, sdCapsule( p, vec3( 0, 0, 0 ), vec3( 0, w, 0 ), r ) );
    d = min( d, sdCapsule( p, vec3( 0, 0, w ), vec3( 0, w, w ), r ) );
    d = min( d, sdCapsule( p, vec3( w, 0, 0 ), vec3( w, w, 0 ), r ) );
    d = min( d, sdCapsule( p, vec3( w, 0, w ), vec3( w, w, w ), r ) );

    d = min( d, sdCapsule( p, vec3( 0, 0, 0 ), vec3( 0, 0, w ), r ) );
    d = min( d, sdCapsule( p, vec3( 0, w, 0 ), vec3( 0, w, w ), r ) );
    d = min( d, sdCapsule( p, vec3( w, 0, 0 ), vec3( w, 0, w ), r ) );
    d = min( d, sdCapsule( p, vec3( w, w, 0 ), vec3( w, w, w ), r ) );
    return d;
}

RayResult traceOuterBox( vec3 origin, vec3 direction )
{
    float t = 0.0f;
    for( int i = 0; i < 500; i++ )
    {
        float d = sdfOuterBox( origin + direction * t, t );
        t += d;

        if( d < 0.0001f )
        {
            return RayResult( vec3( 1 ), origin + direction * t, vec3( 0 ), true, t );
        }

        if( t > 10000.0f )
        {
            break;
        }
    }
    return RayResult( vec3( 0 ), vec3( 0 ), vec3( 0 ), false, 0 );
}

RayResult traceRay( vec3 origin, vec3 direction )
{
    // Check whether the ray will intersect the world box
    bool intersect = false;
    float startT = 1000000000000.0f;

    // Do we start inside the world box?
    if( origin.x >= 0 && origin.y >= 0 && origin.z >= 0 && origin.x < WORLD_SIZE && origin.y < WORLD_SIZE && origin.z < WORLD_SIZE )
    {
        intersect = true;
        startT = 0.0f;
    }
    // If not, try to calculate the distance to the world box
    else for( int i = 0; i < 3; i++ )
    {
        // Forward ray, check intersect at position 0
        float next;
        if( direction[ i ] > 0.0f ) next = 0.0f;
        else next = WORLD_SIZE;
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

    if( !intersect ) return RayResult( vec3( 0 ), vec3( 0 ), vec3( 0 ), false, 0 );

    // distance of the ray to the closest axis plane it'll intersect
    vec3 sidepos = vec3( -1.0f );
    for( int i = 0; i < 3; i++ )
    {
        vec3 next = rayPos;
        if( direction[i] > 0.0f )
        {
            next[i] = ceil( rayPos[i] );
        }
        else if( direction[i] < 0.0f )
        {
            next[i] = floor( rayPos[i] );
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
            vec4 theCell = inWorldData.mWorldData[pos.x * WORLD_SIZE * WORLD_SIZE + pos.y * WORLD_SIZE + pos.z];
            if( theCell.a > 0.0f )
            {
                return RayResult( theCell.rgb, origin + direction * t, normal, true, t );
            }
        }
        else if( c > 1 ) break;
    }

    return RayResult( vec3( 0 ), vec3( 0 ), vec3( 0 ), false, 0 );
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
