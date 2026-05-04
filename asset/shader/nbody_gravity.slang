#include "idk/slang.hpp"

#define dt_ 0.001f
#define softening_ 0.4f // prevents singularity when distance → 0
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

    vec3  pos_i   = dataIn[i].pos.xyz;
    float mass_i  = dataIn[i].pos.w;
    vec3  vel     = dataIn[i].vel.xyz;
    vec3  acc_old = dataIn[i].acc.xyz;

    vec3 acc_new = vec3(0.0f);
    for (uint j=0; j<num_bodies_; j++)
    {
        if (i == j) continue;

        vec3  diff     = dataIn[j].pos.xyz - pos_i;
        float distSq   = dot(diff, diff) + softening_ * softening_;
        float invDist3 = rsqrt(distSq * distSq * distSq);
        float mass_j   = dataIn[j].pos.w;

        acc_new += mass_j * diff * invDist3;
    }

    vec3 newVel = vel + 0.5f * (acc_old + acc_new) * dt_;

    dataOut[i].pos = dataIn[i].pos;
    dataOut[i].vel = vec4(newVel, 0.0f);
    dataOut[i].acc = vec4(acc_new, 0.0f);
}

