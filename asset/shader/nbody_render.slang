#include "idk/slang.hpp"

[[vk::binding(idk::UboWindowData::BIND_IDX, 0)]]
ConstantBuffer<idk::UboWindowData, Std140DataLayout> UboWindow_;

[[vk::binding(idk::UboCameraData::BIND_IDX, 0)]]
ConstantBuffer<idk::UboCameraData, Std140DataLayout> uboCamera_;

[[vk::binding(idk::BIND_NBODY_IN, 0)]]
StructuredBuffer<idk::NBodyVertex, Std430DataLayout> vertIn;

[[vk::binding(idk::BIND_NBODY_OUT, 0)]]
StructuredBuffer<idk::NBodyVertex, Std430DataLayout> vertOut;

struct VertexOutput
{
    uint    vertId;
    float4  sv_position  : SV_Position;
    float2  screenCenter : TEXCOORD0;   // point centre in pixels
    float   pointSize    : TEXCOORD1;
};

[shader("vertex")]
VertexOutput vertmain(uint vertId : SV_VertexID, out float sv_pointsize : SV_PointSize)
{
    VertexOutput vsout;

    float4 pos  = float4(vertIn[vertId].pos.xyz, 1.0);
    float  mass = vertIn[vertId].pos.w;

    vsout.vertId = vertId;
    vsout.sv_position = mul(uboCamera_.P, mul(uboCamera_.V, pos));
    sv_pointsize = sqrt(0.25f * mass);

    float ps = sqrt(0.25f * mass);
    sv_pointsize    = ps;
    vsout.pointSize = ps;

    // convert NDC → pixels using viewport dimensions from your UBO
    float2 ndc = vsout.sv_position.xy / vsout.sv_position.w;
    vsout.screenCenter = (ndc * 0.5 + 0.5) * UboWindow_.winsz.xy;

    return vsout;
}


[shader("fragment")]
float4 fragmain(VertexOutput fsin) : SV_Target0
{
    // vec2 uv = 2.0 * g_PointCoord - 1.0;

    // if (dot(uv, uv) > 1.0)
    // {
    //     discard;
    // }

    float3 vel = vertIn[fsin.vertId].vel.xyz;
    float3 acc = vel - vertOut[fsin.vertId].vel.xyz;
           acc += float3(0.15);

    return float4(1.0);
}
