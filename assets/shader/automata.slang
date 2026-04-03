#include "idk/gfx/slang.hpp"

[[vk::binding(idk::BINDING_SSBO_0, 0)]]
StructuredBuffer<idk::SSBO_0, Std430DataLayout> dataIn;

[[vk::binding(idk::BINDING_SSBO_1, 0)]]
RWStructuredBuffer<idk::SSBO_1, Std430DataLayout> dataOut;


[shader("compute")]
[numthreads(1,1,1)]
void compmain(uint3 threadId : SV_DispatchThreadID)
{
    uint i = threadId.x;

    vec4 pos = dataIn[i].pos;
    vec4 vel = dataIn[i].vel;

    dataOut[i].pos = vec4((pos+vel).xyz, 0.0);
}

