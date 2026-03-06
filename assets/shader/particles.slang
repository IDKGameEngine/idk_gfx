#define SSBO_PARTICLE_COUNT 128*128

struct ssbo_particles_t
{
    float4 pos[SSBO_PARTICLE_COUNT];
    float4 vel[SSBO_PARTICLE_COUNT];
};

// ConstantBuffer<ssbo_particles_t, Std430DataLayout> dataIn;
StructuredBuffer<ssbo_particles_t, Std430DataLayout> dataIn;  // read only ssbo
RWStructuredBuffer<ssbo_particles_t, Std430DataLayout> dataOut; // read write ssbo


[shader("compute")]
[numthreads(1,1,1)]
void compmain(uint3 threadId : SV_DispatchThreadID)
{
    uint index = threadId.x;

    float4 pos = dataIn[0].pos[index];
    float4 vel = dataIn[0].vel[index];

    dataOut[0].pos[index] = float4((pos+vel).xyz, 0.0);
}


