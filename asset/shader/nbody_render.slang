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
    float4  position;
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
    vsout.position = pos;
    vsout.sv_position = mul(uboCamera_.P, mul(uboCamera_.V, pos));

    float maxMassHi = idk::NBodyVertex::MAX_MASS_HI;
    float maxMassLo = idk::NBodyVertex::MAX_MASS_LO;
    sv_pointsize = (mass <= maxMassLo) ? 1.25 + (mass/maxMassLo) : 4.5*(mass/maxMassHi);
    vsout.pointSize = sv_pointsize;

    // convert NDC → pixels using viewport dimensions from your UBO
    float2 ndc = vsout.sv_position.xy / vsout.sv_position.w;
    vsout.screenCenter = (ndc * 0.5 + 0.5) * UboWindow_.winsz.xy;

    return vsout;
}


[shader("fragment")]
float4 fragmain(VertexOutput fsin) : SV_Target0
{
    // vec2 centerNDC = fsin.sv_position.xy / fsin.sv_position.w;
    // vec2 centerWindow = centerNDC * 0.5 + 0.5;
    // vec2 centerPixel  = centerWindow * UboWindow_.winsz.xy;
    // vec2 myPointCoord = (fsin.position.xy - centerPixel) / fsin.pointSize + 0.5;

    // return vec4(distance(myPointCoord, centerPixel, 0.0, 1.0));

    // if (dot(myPointCoord, myPointCoord) > 1.0)
    // {
    //     discard;
    // }

    float maxMassHi = idk::NBodyVertex::MAX_MASS_HI;

    float mass  = vertIn[fsin.vertId].pos.w;
    float alpha = clamp(mass*mass*mass*mass, 0.0, maxMassHi) / maxMassHi;

    return float4(0.95, 0.7, 0.96, alpha);
}
