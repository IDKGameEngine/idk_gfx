
float4 sampleBindless(uint64_t handle, float2 uv)
{
    __intrinsic_asm "texture(sampler2D(unpackUint2x32($0)), $1)";
}

