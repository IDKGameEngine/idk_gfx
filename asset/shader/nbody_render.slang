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
    float4  worldpos;
    float4  sv_position  : SV_Position;
    float   pointSize;    // : TEXCOORD1;
    float2  screenCenter; // : TEXCOORD0;   // point centre in pixels
};

[shader("vertex")]
VertexOutput vertmain(uint vertId: SV_VertexID, out float sv_pointsize: SV_PointSize)
{
    VertexOutput vsout;

    float4 wpos = float4(vertIn[vertId].pos.xyz, 1.0);
    float  mass = vertIn[vertId].pos.w;

    vsout.vertId   = vertId;
    vsout.worldpos = wpos;
    vsout.sv_position = mul(uboCamera_.P, mul(uboCamera_.V, wpos));
    vsout.pointSize = 64.0 / vsout.sv_position.w;
    float2 ndc = vsout.sv_position.xy / vsout.sv_position.w;
    vsout.screenCenter = (ndc * 0.5 + 0.5) * UboWindow_.winsz.xy;

    sv_pointsize = vsout.pointSize;

    return vsout;
}


[shader("fragment")]
float4 fragmain(VertexOutput fsin) : SV_Target0
{
    vec2 centerNDC = fsin.sv_position.xy / fsin.sv_position.w;
    vec2 centerWindow = centerNDC * 0.5 + 0.5;
    vec2 centerPixel  = centerWindow * UboWindow_.winsz.xy;
    vec2 myPointCoord = (fsin.screenCenter.xy - centerPixel) / fsin.pointSize + 0.5;

    return vec4(myPointCoord, 0.0, 1.0);

    // if (dot(myPointCoord, myPointCoord) > 1.0)
    // {
    //     discard;
    // }

    // float maxMassHi = idk::NBodyVertex::MAX_MASS_HI;

    // float mass  = vertIn[fsin.vertId].pos.w;
    // float alpha = clamp(mass*mass*mass*mass, 0.0, maxMassHi) / maxMassHi;

    // return float4(0.95, 0.7, 0.96, alpha);
}
