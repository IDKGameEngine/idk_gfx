#include "idk/gfx/slang.hpp"

layout(binding = 4) RWTexture2D<vec3> outputImage;

[shader("compute")]
[numthreads(idk::AUTOMATA_WIDTH, idk::AUTOMATA_WIDTH, 1)]
void compmain(uvec3 threadId : SV_DispatchThreadID)
{
    uvec2 texel = uvec2(threadId.xy);
    outputImage.Store(texel, curr);
}
