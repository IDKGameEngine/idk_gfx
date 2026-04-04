#include "idk/gfx/slang.hpp"


layout(binding = 8) RWTexture2D<vec4> inputImage;
layout(binding = 9) RWTexture2D<vec4> outputImage;


int countNeighbours(ivec2 texel)
{
    uint count = 0;
    count += uint(255.0f * inputImage.Load(texel + ivec2(-1, -1)).x);
    count += uint(255.0f * inputImage.Load(texel + ivec2(-1,  0)).x);
    count += uint(255.0f * inputImage.Load(texel + ivec2(-1, +1)).x);
    count += uint(255.0f * inputImage.Load(texel + ivec2( 0, -1)).x);
    count += uint(255.0f * inputImage.Load(texel + ivec2( 0,  0)).x);
    count += uint(255.0f * inputImage.Load(texel + ivec2( 0, +1)).x);
    count += uint(255.0f * inputImage.Load(texel + ivec2(+1, -1)).x);
    count += uint(255.0f * inputImage.Load(texel + ivec2(+1,  0)).x);
    count += uint(255.0f * inputImage.Load(texel + ivec2(+1, +1)).x);
    return count / 255;
}
// Any live cell with fewer than two live neighbours dies, as if by underpopulation.
// Any live cell with two or three live neighbours lives on to the next generation.
// Any live cell with more than three live neighbours dies, as if by overpopulation.
// Any dead cell with exactly three live neighbours becomes a live cell, as if by reproduction.

[shader("compute")]
[numthreads(8, 8, 1)]
void compmain(uvec3 threadId : SV_DispatchThreadID)
{
    ivec2 texel = ivec2(threadId.xy);
    uint count = countNeighbours(texel);
    uint value = 0;

    if (count==2 || count==3)
    {
        value = 1;
    }

    outputImage.Store(texel, vec4(value, 0, 0, 1));
}
