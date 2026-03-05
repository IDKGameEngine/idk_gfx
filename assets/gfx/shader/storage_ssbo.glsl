#define SSBO_PARTICLE_COUNT 128*128

struct ssbo_particles_t
{
    vec4 pos[SSBO_PARTICLE_COUNT];
    vec4 vel[SSBO_PARTICLE_COUNT];
};

layout (binding = 2, std430) readonly
buffer SSBO_2
{
    ssbo_particles_t ssIn;
};

layout (binding = 3, std430) writeonly
buffer SSBO_3
{
    ssbo_particles_t ssOut;
};
