#include "idk/slang.hpp"

[[vk::binding(idk::SSBO_BIND_NBODY_CURR, 0)]]
StructuredBuffer<idk::SsboNBody, Std430DataLayout> curr_;

struct VertexInput
{
    vec4 pos;
    uint id : SV_VertexID;
};

struct VertexOutput
{
    float2 uv;
    float4 sv_position : SV_Position;
};


[shader("vertex")]
VertexOutput vertmain(VertexInput vsin)
{
    VertexOutput vsout;

    const mat4 view = mat4(1.0f);
    const mat4 proj = mat4(1.0f);

    // 2 triangles = 6 verts per particle, no geometry shader needed
    uint particleId = vsin.id / 6;
    uint cornerId   = vsin.id % 6;

    // Two triangles making a quad
    float2 corners[6] = { {-1,-1}, {1,-1}, {1,1}, {-1,-1}, {1,1}, {-1,1} };
    float2 corner = corners[cornerId];

    float3 pos  = curr_[0].pos[particleId].xyz;
    float  size = 0.01; // world-space radius

    // Billboard: offset in view space, then project
    float4 viewPos = mul(view, float4(pos, 1.0));
    viewPos.xy += corner * size;

    vsout.sv_position = mul(proj, viewPos);
    vsout.uv  = corner; // -1..1, use for circle clipping

    return vsout;
}


[shader("fragment")]
float4 fragmain(VertexOutput fsin) : SV_Target0
{
    return vec4(1.0, 0.0, 0.0, 1.0);
}
