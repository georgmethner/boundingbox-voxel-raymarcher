#version 460 core

out vec4 FragColor;
uniform sampler2D screen;
in vec2 UVs;

layout(binding = 2)
uniform usampler3D object_texture;

layout (binding = 0) buffer object_matrices
{
    mat4 object_matrix[];
};

uniform uint object_index;

uniform mat4 view;
uniform mat4 proj;

uniform float x1;

const float GRID = 16;
const float EPSY = 0.001;

const uint kVGAPalette[256] = uint[256](
0x00000u,0x0002au,0x00a80u,0x00aaau,0x2a000u,0x2a02au,0x2a540u,0x2aaaau,0x15555u,0x1557fu,0x15fd5u,0x15fffu,0x3f555u,0x3f57fu,0x3ffd5u,0x3ffffu,
0x00000u,0x05145u,0x08208u,0x0b2cbu,0x0e38eu,0x11451u,0x14514u,0x18618u,0x1c71cu,0x20820u,0x24924u,0x28a28u,0x2db6du,0x32cb2u,0x38e38u,0x3ffffu,
0x0003fu,0x1003fu,0x1f03fu,0x2f03fu,0x3f03fu,0x3f02fu,0x3f01fu,0x3f010u,0x3f000u,0x3f400u,0x3f7c0u,0x3fbc0u,0x3ffc0u,0x2ffc0u,0x1ffc0u,0x10fc0u,
0x00fc0u,0x00fd0u,0x00fdfu,0x00fefu,0x00fffu,0x00bffu,0x007ffu,0x0043fu,0x1f7ffu,0x277ffu,0x2f7ffu,0x377ffu,0x3f7ffu,0x3f7f7u,0x3f7efu,0x3f7e7u,
0x3f7dfu,0x3f9dfu,0x3fbdfu,0x3fddfu,0x3ffdfu,0x37fdfu,0x2ffdfu,0x27fdfu,0x1ffdfu,0x1ffe7u,0x1ffefu,0x1fff7u,0x1ffffu,0x1fdffu,0x1fbffu,0x1f9ffu,
0x2db7fu,0x31b7fu,0x36b7fu,0x3ab7fu,0x3fb7fu,0x3fb7au,0x3fb76u,0x3fb71u,0x3fb6du,0x3fc6du,0x3fdadu,0x3feadu,0x3ffedu,0x3afedu,0x36fedu,0x31fedu,
0x2dfedu,0x2dff1u,0x2dff6u,0x2dffau,0x2dfffu,0x2debfu,0x2ddbfu,0x2dc7fu,0x0001cu,0x0701cu,0x0e01cu,0x1501cu,0x1c01cu,0x1c015u,0x1c00eu,0x1c007u,
0x1c000u,0x1c1c0u,0x1c380u,0x1c540u,0x1c700u,0x15700u,0x0e700u,0x07700u,0x00700u,0x00707u,0x0070eu,0x00715u,0x0071cu,0x0055cu,0x0039cu,0x001dcu,
0x0e39cu,0x1139cu,0x1539cu,0x1839cu,0x1c39cu,0x1c398u,0x1c395u,0x1c391u,0x1c38eu,0x1c44eu,0x1c54eu,0x1c60eu,0x1c70eu,0x1870eu,0x1570eu,0x1170eu,
0x0e70eu,0x0e711u,0x0e715u,0x0e718u,0x0e71cu,0x0e61cu,0x0e55cu,0x0e45cu,0x1451cu,0x1651cu,0x1851cu,0x1a51cu,0x1c51cu,0x1c51au,0x1c518u,0x1c516u,
0x1c514u,0x1c594u,0x1c614u,0x1c694u,0x1c714u,0x1a714u,0x18714u,0x16714u,0x14714u,0x14716u,0x14718u,0x1471au,0x1471cu,0x1469cu,0x1461cu,0x1459cu,
0x00010u,0x04010u,0x08010u,0x0c010u,0x10010u,0x1000cu,0x10008u,0x10004u,0x10000u,0x10100u,0x10200u,0x10300u,0x10400u,0x0c400u,0x08400u,0x04400u,
0x00400u,0x00404u,0x00408u,0x0040cu,0x00410u,0x00310u,0x00210u,0x00110u,0x08210u,0x0a210u,0x0c210u,0x0e210u,0x10210u,0x1020eu,0x1020cu,0x1020au,
0x10208u,0x10288u,0x10308u,0x10388u,0x10408u,0x0e408u,0x0c408u,0x0a408u,0x08408u,0x0840au,0x0840cu,0x0840eu,0x08410u,0x08390u,0x08310u,0x08290u,
0x0b2d0u,0x0c2d0u,0x0d2d0u,0x0f2d0u,0x102d0u,0x102cfu,0x102cdu,0x102ccu,0x102cbu,0x1030bu,0x1034bu,0x103cbu,0x1040bu,0x0f40bu,0x0d40bu,0x0c40bu,
0x0b40bu,0x0b40cu,0x0b40du,0x0b40fu,0x0b410u,0x0b3d0u,0x0b350u,0x0b310u,0x00000u,0x00000u,0x00000u,0x00000u,0x00000u,0x00000u,0x00000u,0x00000u);

vec3 GetColor_VGAPalette( int index )
{
    uint c = kVGAPalette[index];
    return vec3(ivec3(c>>12,c>>6,c>>0)&63)/63.0;
}

vec2 box(in vec3 rdd, in vec3 roo, in vec3 rad)
{

    // ray-box intersection in box space
    vec3 m = 1.0/rdd;
    #if 1
    vec3 n = m*roo;
    vec3 k = abs(m)*rad;
    vec3 t1 = -n - k;
    vec3 t2 = -n + k;
    #else
    // more robust
    vec3 k = vec3(rdd.x>=0.0?rad.x:-rad.x, rdd.y>=0.0?rad.y:-rad.y, rdd.z>=0.0?rad.z:-rad.z);
    vec3 t1 = (-roo - k)*m;
    vec3 t2 = (-roo + k)*m;
    #endif
    float tN = max(max(t1.x, t1.y), t1.z);
    float tF = min(min(t2.x, t2.y), t2.z);

    // no intersection
    if (tN>tF || tF<0.0) return vec2(0);

    // this works as long as the ray origin is not inside the box
    return vec2(tN, tF);


}

vec3 trace(vec3 origin, vec3 direction, vec3 scale, float exit_distance)
{

    origin *= GRID * scale;


    vec3 point = floor(origin);
    vec3 steps = sign(direction);
    vec3 tDelta = 1.0 / max(abs(direction), EPSY);
    vec3 tMax = (1.0 - fract(origin * steps)) * tDelta;

    for(int i = 0; i < exit_distance; i++)
    {
        uint c = texelFetch(object_texture, ivec3(point), 0).r;

        if(c != 0)
            return vec3(c, i, origin - point);


        // branchless iteration
        bvec3 a = lessThan(tMax, tMax.yzx);
        bvec3 b = lessThanEqual(tMax, tMax.zxy);
        vec3 select = vec3(a) * vec3(b);

        point += select * steps;
        tMax += select * tDelta;
    }
    //return vec3(0.0);
    discard;
}

void main()
{
    ivec2 pixel_coodirs = ivec2(gl_FragCoord.xy);

    ivec2 screenSize = ivec2(1920, 1080);
    vec2 uv = vec2((pixel_coodirs.xy + vec2(0.5f, 0.5f)) / vec2(screenSize.x, screenSize.y) * 2.0 - 1.0);

    vec3 origin = vec3(view * vec4(0.0, 0.0, 0.0, 1.0));
    vec3 dir = vec3(proj * vec4(uv, 0.0, 1.0));

    dir = vec3(view * vec4(dir, 0.0f));
    dir = normalize(dir);

    vec3 scale = vec3(length(object_matrix[object_index][0]), length(object_matrix[object_index][1]), length(object_matrix[object_index][2]));
    float depth = gl_FragCoord.z / gl_FragCoord.w;

    vec3 rdd = (inverse(object_matrix[object_index])*vec4(dir, 0.0)).xyz;
    vec3 roo = (inverse(object_matrix[object_index])*vec4(origin, 1.0)).xyz;
    vec3 rtn = (inverse(object_matrix[object_index])*vec4( (origin + (dir * depth) + vec3(object_matrix[object_index] * vec4(0.5, 0.5, 0.5, 0.0)) ) , 1.0)).xyz;


    vec2 t_near_far = box(rdd, roo, scale);
    float ray_distance = t_near_far.y - depth + 1;


    //vec3 res = trace(roo + vec3(object_matrix[object_index] * vec4(0.5, 0.5, 0.5, 0.0)) - vec3(object_matrix[object_index][3]) + (normalize(rdd) * (gl_FragCoord.z / gl_FragCoord.w)), rdd, ray_distance*GRID);
    vec3 res = trace(rtn, rdd, scale, ray_distance*GRID);

    //if (res.x == 0)
    //    FragColor = vec4(1.0 / ray_distance);
    //else
    FragColor = vec4(GetColor_VGAPalette(int(res.x)), 1.0f);

}