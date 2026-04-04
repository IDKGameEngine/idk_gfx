#include "idk/gfx/slang.hpp"


struct YeahBoi
{
    [[vk::binding(idk::BINDING_SSBO_0, 0)]]
    StructuredBuffer<idk::SSBO_0, Std430DataLayout> dataIn;

    [[vk::binding(idk::BINDING_SSBO_1, 0)]]
    RWStructuredBuffer<idk::SSBO_1, Std430DataLayout> dataOut;
};

YeahBoi boi;



[shader("compute")]
[numthreads(1,1,1)]
void compmain(uint3 threadId : SV_DispatchThreadID)
{
    uint i = threadId.x;

    vec4 pos = boi.dataIn[i].pos;
    vec4 vel = boi.dataIn[i].vel;

    boi.dataOut[i].pos = vec4((pos+vel).xyz, 0.0);
}


// [[vk::binding(idk::BINDING_UBO_0, 0)]]
// StructuredBuffer<DescriptorHandle<Texture2D>> textureHandles;

// vec4 sampleBindless(DescriptorHandle<Texture2D> handle, vec2 uv)
// {
//     handle.Load()
//     handle.Sample(SamplerState uv);
//     textureHandles[0].
//     Sampler2D tex = Sampler2D(handle);
//     return tex.Sample(uv);
// }

// void testfn()
// {
//     vec4 rgba = sampleBindless(textureHandles[0])
// }