#include "idk/gfx/slang.hpp"

StorageBufferRO(idk::StorageBuffer00) ssboRO;
StorageBufferRW(idk::StorageBuffer01) ssboRW;


[shader("compute")]
[numthreads(1,1,1)]
void compmain(uint3 threadId : SV_DispatchThreadID)
{
    let dataIn = ssboRO[0];
    uint i = threadId.x;

    vec4 pos = dataIn.pos[i];
    vec4 vel = dataIn.vel[i];

    ssboRW[0].pos[i] = vec4((pos+vel).xyz, 0.0);
    // dataOut.pos[i] = vec4((pos+vel).xyz, 0.0);
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