// https://shader-slang.org/slang/user-guide/glsl-target-specific
// https://docs.shader-slang.org/en/latest/coming-from-glsl.html
//
// Shared C++ / Slang header — GPU resource binding registry
//
// OpenGL 4.6 + SPIR-V: descriptor sets do not exist; all resources share
// one flat binding namespace per pipeline. We partition it explicitly:
//
//   Binding range  │ Resource class        │ Max slots
//   ───────────────┼───────────────────────┼──────────
//    [ 0 … 15]     │ Uniform buffers (UBO) │ 16
//    [16 … 31]     │ Storage buffers (SSBO)│ 16
//    [32 … 47]     │ Image load/store      │ 16
//    [48 … 63]     │ (reserved / samplers) │ 16
//
// C++  side: use BIND_IDX with glBindBufferBase / glBindImageTexture
// Slang side: use the UniformBufferRO / StorageBufferR[OW] / Image* macros
//

#ifndef IDK_GFX_SLANG_HPP
#define IDK_GFX_SLANG_HPP

#ifdef __cplusplus
    #include "slang/defs.hpp"
    #define IDK_SLANG_NAMESPACE idk::gfx::slang
#else
    #include "slang/defs.slang"
    #define IDK_SLANG_NAMESPACE idk
#endif


#define UBO_STRUCT(Idx_, Content_) \
struct UniformBuffer0##Idx_ \
{ \
    static constexpr const uint32_t BIND_IDX = Idx_; \
    Content_ \
}

#define SSBO_STRUCT(Idx_, Content_) \
struct StorageBuffer0##Idx_ \
{ \
    static constexpr const uint32_t BIND_IDX = Idx_; \
    Content_ \
}


namespace IDK_SLANG_NAMESPACE
{
    static constexpr const uint32_t UBO_BIND_BASE  = 8;
    static constexpr const uint32_t SSBO_BIND_BASE = 16;
    static constexpr const uint32_t IMG_BIND_BASE  = 32;

    struct GlobalData
    {
        static constexpr const uint32_t BIND_IDX = UBO_BIND_BASE + 0;
    };

    struct PerFrameData
    {
        static constexpr const uint32_t BIND_IDX = UBO_BIND_BASE + 1;
        float prevTime;
        float currTime;
        float deltaTime;
        float timescale;
        vec2  winSize;
        vec2  padding1;
    };

    struct PerCameraData
    {
        static constexpr const uint32_t BIND_IDX = UBO_BIND_BASE + 2;
        mat4 View;
        mat4 Proj;
    };


    static constexpr const uint32_t BIND_NBODY_IN  = SSBO_BIND_BASE + 4;
    static constexpr const uint32_t BIND_NBODY_OUT = SSBO_BIND_BASE + 5;

    struct NBodyVertex
    {
        static constexpr const float    MIN_MASS   = 16.0f;
        static constexpr const float    MAX_MASS   = 512.0f;
        static constexpr const uint32_t MAX_BODIES = 1*1024;
        static constexpr const uint32_t GROUP_SIZE = 32;
        
        vec4 pos;
        vec4 vel;
        vec4 acc;
    };

    // struct NBodyVertexArray
    // {
    //     NBodyVertex verts[NBodyVertex::MAX_BODIES];
    // };

    static constexpr const int AUTOMATA_WIDTH = 1024;

} // namespace IDK_SLANG_NAMESPACE

#endif // IDK_GFX_SLANG_HPP
