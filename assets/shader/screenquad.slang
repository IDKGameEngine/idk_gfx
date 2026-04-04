#include "idk/gfx/slang.hpp"

struct VertexShaderInput
{
    uint id : SV_VertexID;
};

struct VertexShaderOutput
{
    float2 uv;
    float4 sv_position : SV_Position;
};

idk::UniformReaderB3 ubo3;



[shader("vertex")]
VertexShaderOutput vertmain(VertexShaderInput vsin)
{
    VertexShaderOutput vsout;

    float2 pos = float2(vsin.id % 2, vsin.id / 2) * 4.0 - 1.0;
    float2 tex = (pos + 1.0) * 0.5;

    vsout.uv = tex;
    vsout.sv_position = float4(pos, 1.0, 1.0);

    return vsout;
}


[shader("fragment")]
float4 fragmain(VertexShaderOutput fsin) : SV_Target0
{
    float3 rgb = float3(fsin.uv, 0.0);
    float2 sz = ubo3.dataIn.winsz.xy;

    float d = distance(sz*ubo3.dataIn.gamepos.xy, sz*fsin.uv);
    if (d < 16.0f)
    {
        rgb = float3(1.0f);
    }

    return float4(rgb, 1.0);
}

