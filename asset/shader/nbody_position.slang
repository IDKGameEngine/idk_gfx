#include "idk/slang.hpp"

#define dt_ 0.001f
#define softening_ 0.1f // prevents singularity when distance → 0
#define num_bodies_ idk::NBodyVertex::MAX_BODIES


[[vk::binding(idk::BIND_NBODY_IN, 0)]]
StructuredBuffer<idk::NBodyVertex, Std430DataLayout> dataIn;

[[vk::binding(idk::BIND_NBODY_OUT, 0)]]
RWStructuredBuffer<idk::NBodyVertex, Std430DataLayout> dataOut;


[shader("compute")]
[numthreads(idk::NBodyVertex::GROUP_SIZE, 1, 1)]
void compmain(uvec3 threadId : SV_DispatchThreadID)
{
    uint i = threadId.x;
    if (i >= num_bodies_)
    {
        return;
    }

    vec3  pos     = dataIn[i].pos.xyz;
    float mass    = dataIn[i].pos.w;
    vec3  vel     = dataIn[i].vel.xyz;
    vec3  acc_old = dataIn[i].acc.xyz;

    // Verlet position step
    vec3 newPos = pos + vel * dt_ + 0.5f * acc_old * dt_ * dt_;

    // Pass everything through; vel and acc are updated in pass 2
    dataOut[i].pos = vec4(newPos, mass);
    dataOut[i].vel = dataIn[i].vel;
    dataOut[i].acc = dataIn[i].acc;
}

