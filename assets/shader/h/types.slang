// https://shader-slang.org/slang/user-guide/glsl-target-specific

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
