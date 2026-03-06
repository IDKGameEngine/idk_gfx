#include "h/storage.slang"

[shader("compute")]
[numthreads(1,1,1)]
void compmain(uint3 threadId : SV_DispatchThreadID)
{
    uint index = threadId.x;

    float4 pos = dataIn[0].pos[index];
    float4 vel = dataIn[0].vel[index];

    dataOut[0].pos[index] = float4((pos+vel).xyz, 0.0);
}


