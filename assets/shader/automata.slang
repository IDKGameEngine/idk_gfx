#include "idk/gfx/slang.hpp"

idk::StorageReaderB0 ssbo0;
idk::StorageWriterB1 ssbo1;



[shader("compute")]
[numthreads(1,1,1)]
void compmain(uint3 threadId : SV_DispatchThreadID)
{
    uint i = threadId.x;

    vec4 pos = ssbo0.dataIn[i].pos;
    vec4 vel = ssbo0.dataIn[i].vel;

    ssbo1.dataOut[i].pos = vec4((pos+vel).xyz, 0.0);
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