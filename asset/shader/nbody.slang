#include "idk/slang.hpp"

#define dt_ 0.001f
#define softening_ 0.1f // prevents singularity when distance → 0
#define num_bodies_ idk::SsboNBody::MAX_BODIES

[[vk::binding(idk::SSBO_BIND_NBODY_PREV, 0)]]
StructuredBuffer<idk::SsboNBody, Std430DataLayout> prev_;

[[vk::binding(idk::SSBO_BIND_NBODY_CURR, 0)]]
RWStructuredBuffer<idk::SsboNBody, Std430DataLayout> curr_;


[shader("compute")]
[numthreads(idk::SsboNBody::GROUP_SIZE, 1, 1)]
void compmain(uvec3 threadId : SV_DispatchThreadID)
{
    uint i = threadId.x;
    if (i >= num_bodies_)
    {
        return;
    }

    vec3  pos_i  = prev_[0].pos[i].xyz;
    float mass_i = prev_[0].pos[i].w;
    vec3  acc    = vec3(0.0);

    // O(N²) all-pairs gravity
    for (uint j=0; j<idk::SsboNBody::MAX_BODIES; j++)
    {
        if (i == j)
        {
            continue;
        }

        vec3  diff     = prev_[0].pos[j].xyz - pos_i;
        float distSq   = dot(diff, diff) + softening_*softening_;
        float invDist3 = rsqrt(distSq * distSq * distSq); // 1/dist³
        float mass_j   = prev_[0].pos[j].w;

        acc += mass_j * diff * invDist3; // G=1 for simplicity
    }

    // Leapfrog / symplectic Euler integration
    vec3 newVel = prev_[0].vel[i].xyz + acc * dt_;
    vec3 newPos = pos_i + newVel * dt_;

    curr_[0].vel[i].xyz = newVel;
    curr_[0].pos[i].xyz = newPos;
}

