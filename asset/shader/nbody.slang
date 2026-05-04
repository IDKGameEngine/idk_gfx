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

    vec4 inPos = dataIn[i].pos;
    vec4 inVel = dataIn[i].vel;

    vec3  pos_i  = inPos.xyz;
    float mass_i = inPos.w;
    vec3  acc    = vec3(0.0);

    // O(N²) all-pairs gravity
    for (uint j=0; j<idk::NBodyVertex::MAX_BODIES; j++)
    {
        if (i == j)
        {
            continue;
        }

        vec3  diff     = dataIn[j].pos.xyz - pos_i;
        float distSq   = dot(diff, diff) + softening_*softening_;
        float invDist3 = rsqrt(distSq * distSq); // 1/dist³
        float mass_j   = dataIn[j].pos.w;

        acc += mass_j * diff * invDist3; // G=1 for simplicity
    }

    // Leapfrog / symplectic Euler integration
    vec3 newVel = inVel.xyz + acc * dt_;
    vec3 newPos = pos_i + newVel * dt_;

    dataOut[i].vel.xyz = newVel;
    dataOut[i].pos.xyz = newPos;

    // dataOut[i].pos.xyz = inPos.xyz + dt_*inVel.xyz;
    // dataOut[i].pos.w   = inPos.w;
    // dataOut[i].vel     = inVel;
}

