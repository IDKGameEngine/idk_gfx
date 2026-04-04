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

layout(binding = 8) RWTexture2D<vec3> inputImage;
UniformBufferRO(idk::UniformBuffer03) ubo3RO;


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
    uvec2 texel = uvec2(fsin.uv * vec2(idk::AUTOMATA_WIDTH));
    vec3 rgb = inputImage.Load(texel);

    return vec4(rgb, 1.0f);

    // float3 rgb = float3(fsin.uv, 0.0);
    // float2 sz = ubo3RO.winsz.xy;

    // float d = distance(sz*ubo3RO.gamepos.xy, sz*fsin.uv);
    // if (d < 16.0f)
    // {
    //     rgb = float3(1.0f);
    // }

    // return float4(rgb, 1.0);
}


