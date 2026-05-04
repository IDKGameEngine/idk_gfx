#include "idk/slang.hpp"

[[vk::binding(idk::UboCameraData::BIND_IDX, 0)]]
ConstantBuffer<idk::UboCameraData, Std140DataLayout> uboCamera_;

[[vk::binding(idk::BIND_NBODY_IN, 0)]]
StructuredBuffer<idk::NBodyVertex, Std430DataLayout> vertIn;

struct VertexOutput
{
    float2 uv;
    float4 sv_position : SV_Position;
};


[shader("vertex")]
VertexOutput vertmain(uint vertId : SV_VertexID, out float sv_pointsize : SV_PointSize)
{
    VertexOutput vsout;

    float3 pos  = vertIn[vertId].pos.xyz;
    float  mass = vertIn[vertId].pos.w;
    float  size = 0.2; // world-space radius

    // // Billboard: offset in view space, then project
    float4 viewPos = mul(uboCamera_.V, float4(pos, 1.0));
    float4 projPos = mul(uboCamera_.P, float4(viewPos.xyz, 1.0));

    vsout.sv_position = vec4(projPos.xyz, 1.0);
    sv_pointsize = 4.0f * mass;

    return vsout;
}


[shader("fragment")]
float4 fragmain(VertexOutput fsin) : SV_Target0
{
    return vec4(1.0, 0.0, 0.0, 1.0);
}
