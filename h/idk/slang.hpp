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
    static constexpr const uint32_t UBO_BIND_BASE  = 0;
    static constexpr const uint32_t SSBO_BIND_BASE = 16;
    static constexpr const uint32_t IMG_BIND_BASE  = 32;

    UBO_STRUCT(0,
        vec4 rgba;
        vec4 xyzw;
    );

    UBO_STRUCT(3,
        vec4 winsz;
        vec4 bgtint;
        vec4 fgtint;
        vec4 gamepos;
    );


    SSBO_STRUCT(0,
        vec4 color[512][512];
    );

    SSBO_STRUCT(1,
        vec4 pos[256];
        vec4 vel[256];
    );

    SSBO_STRUCT(2,
        vec4 junk[1024];
    );

    SSBO_STRUCT(3,
        vec4 data0[512];
        vec4 data1[512];
    );

    static constexpr const int AUTOMATA_WIDTH = 1024;

} // namespace IDK_SLANG_NAMESPACE

#endif // IDK_GFX_SLANG_HPP
