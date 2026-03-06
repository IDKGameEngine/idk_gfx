#define SSBO_PARTICLE_COUNT 128*128

struct ssbo_particles_t
{
    float4 pos[SSBO_PARTICLE_COUNT];
    float4 vel[SSBO_PARTICLE_COUNT];
};

// ConstantBuffer<ssbo_particles_t, Std430DataLayout> dataIn;
StructuredBuffer<ssbo_particles_t, Std430DataLayout> dataIn;  // read only ssbo
RWStructuredBuffer<ssbo_particles_t, Std430DataLayout> dataOut; // read write ssbo
