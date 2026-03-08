// https://shader-slang.org/slang/user-guide/glsl-target-specific

namespace idk::gfx::binding
{
    static constexpr uint32_t SSBO_0 = 0;
    static constexpr uint32_t SSBO_1 = 1;
    static constexpr uint32_t SSBO_2 = 2;
    static constexpr uint32_t SSBO_3 = 3;

    static constexpr uint32_t UBO_0 = 4;
    static constexpr uint32_t UBO_1 = 5;
    static constexpr uint32_t UBO_2 = 6;
    static constexpr uint32_t UBO_3 = 7;
}

#ifndef __cplusplus
    struct VertexShaderInput
    {
        uint id : SV_VertexID;
    };
    
    struct VertexShaderOutput
    {
        float2 uv;
        float4 sv_position : SV_Position;
    };

#endif
