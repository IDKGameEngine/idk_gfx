#include "idk/gfx/slang.hpp"

layout(binding = 8) RWTexture2D<vec4> inputImage;
layout(binding = 9) RWTexture2D<vec4> outputImage;

[shader("compute")]
[numthreads(8, 8, 1)]
void compmain(uvec3 threadId : SV_DispatchThreadID)
{
    uvec2 texel = uvec2(threadId.xy);
    vec4 prev = inputImage.Load(texel);
    vec4 curr = sin(prev);
    outputImage.Store(texel, curr);
}
