
// idk/gfx/slang.hpp
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

// ── Binding base offsets (available to both C++ and Slang) ───────────────────
#define UBO_BINDING_BASE    0
#define SSBO_BINDING_BASE  16
#define IMG_BINDING_BASE   32

// ── Resource-struct macros ────────────────────────────────────────────────────
// Idx_      : local slot index (0-based within its resource class)
// BIND_IDX  : the actual SPIR-V / OpenGL binding point (base + local)
// LOCAL_IDX : the local index, useful when you need the relative slot

#define UBO_STRUCT(Idx_, Content_)                          \
    struct UniformBuffer##Idx_                              \
    {                                                       \
        static const int BIND_IDX  = UBO_BINDING_BASE  + Idx_; \
        static const int LOCAL_IDX = Idx_;                 \
        Content_                                            \
    }

#define SSBO_STRUCT(Idx_, Content_)                         \
    struct StorageBuffer##Idx_                              \
    {                                                       \
        static const int BIND_IDX  = SSBO_BINDING_BASE + Idx_; \
        static const int LOCAL_IDX = Idx_;                 \
        Content_                                            \
    }

// ── Platform-specific block ───────────────────────────────────────────────────
#ifdef __cplusplus
// ── C++ side ─────────────────────────────────────────────────────────────────
    #include <cstdint>
    #include <glm/glm.hpp>

    namespace idk::gfx::slang
    {
        using vec2  = glm::vec2;  using vec3  = glm::vec3;  using vec4  = glm::vec4;
        using ivec2 = glm::ivec2; using ivec3 = glm::ivec3; using ivec4 = glm::ivec4;
        using uvec2 = glm::uvec2; using uvec3 = glm::uvec3; using uvec4 = glm::uvec4;
        using mat4  = glm::mat4;
    }

#else
// ── Slang shader side ─────────────────────────────────────────────────────────
    typedef float2   vec2;  typedef float3   vec3;  typedef float4   vec4;
    typedef int2     ivec2; typedef int3     ivec3; typedef int4     ivec4;
    typedef uint2    uvec2; typedef uint3    uvec3; typedef uint4    uvec4;
    typedef float4x4 mat4;

    // UBO  — read-only, std140
    #define UniformBufferRO(T) \
        [[vk::binding(T::BIND_IDX, 0)]] ConstantBuffer<T, Std140DataLayout>

    // SSBO — read-only, std430
    #define StorageBufferRO(T) \
        [[vk::binding(T::BIND_IDX, 0)]] StructuredBuffer<T, Std430DataLayout>

    // SSBO — read-write, std430
    #define StorageBufferRW(T) \
        [[vk::binding(T::BIND_IDX, 0)]] RWStructuredBuffer<T, Std430DataLayout>

    // Image load/store — Idx_ is the LOCAL image slot (0-based)
    // The actual binding resolves to IMG_BINDING_BASE + Idx_ at compile time.
    // Format_ is a Slang scalar/vector type, e.g. float4, int4, uint.
    //
    // Example:
    //   ImageRW(float4, 0) uOutputImage;   // binding = 32
    //   ImageRO(float4, 1) uInputImage;    // binding = 33
    #define ImageRO(Format_, Idx_) \
        [[vk::binding(IMG_BINDING_BASE + Idx_, 0)]] Texture2D<Format_>

    #define ImageRW(Format_, Idx_) \
        [[vk::binding(IMG_BINDING_BASE + Idx_, 0)]] RWTexture2D<Format_>

    // Convenience aliases for common image formats
    #define Image2D_rgba8_RW(Idx_)  ImageRW(float4,  Idx_)
    #define Image2D_rgba32f_RW(Idx_) ImageRW(float4, Idx_)
    #define Image2D_r32ui_RW(Idx_)  ImageRW(uint,    Idx_)

#endif // __cplusplus

// ── Resource definitions ──────────────────────────────────────────────────────
namespace idk
{
#ifdef __cplusplus
    namespace gfx::slang
    {
#endif

    //
    // UBOs — SPIR-V bindings [UBO_BINDING_BASE … UBO_BINDING_BASE+15]
    //        OpenGL: glBindBufferBase(GL_UNIFORM_BUFFER, T::BIND_IDX, buf)
    //
    UBO_STRUCT(0,           // binding = 0
        vec4 rgba;
        vec4 xyzw;
    );
    UBO_STRUCT(3,           // binding = 3
        vec4 winsz;
        vec4 bgtint;
        vec4 fgtint;
        vec4 gamepos;
    );

    //
    // SSBOs — SPIR-V bindings [SSBO_BINDING_BASE … SSBO_BINDING_BASE+15]
    //         OpenGL: glBindBufferBase(GL_SHADER_STORAGE_BUFFER, T::BIND_IDX, buf)
    //
    SSBO_STRUCT(0,          // binding = 16
        vec4 color[512][512];
    );
    SSBO_STRUCT(1,          // binding = 17
        vec4 pos[256];
        vec4 vel[256];
    );
    SSBO_STRUCT(2,          // binding = 18
        vec4 junk[1024];
    );
    SSBO_STRUCT(3,          // binding = 19
        vec4 data0[512];
        vec4 data1[512];
    );

    //
    // Images — SPIR-V bindings [IMG_BINDING_BASE … IMG_BINDING_BASE+15]
    //          OpenGL: glBindImageTexture(IMG_BINDING_BASE + Idx_, tex, ...)
    //          Defined inline at usage site via ImageRO / ImageRW macros.
    //

    static const int AUTOMATA_WIDTH = 1024;

#ifdef __cplusplus
    } // namespace gfx::slang
#endif
} // namespace idk

#endif // IDK_GFX_SLANG_HPP


/*
    Slang usage:
        #include "idk/gfx/slang.hpp"

        // UBO at binding 0
        UniformBufferRO(UniformBuffer0) uFrame;

        // SSBO at binding 16 (read-write)
        StorageBufferRW(StorageBuffer0) uCells;

        // Image at binding 32
        ImageRW(float4, 0) uOutputImage;

    C++ usage:
        // UBO
        glBindBufferBase(GL_UNIFORM_BUFFER, idk::gfx::slang::UniformBuffer0::BIND_IDX, ubo);
    
        // SSBO
        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, idk::gfx::slang::StorageBuffer0::BIND_IDX, ssbo);
    
        // Image
        glBindImageTexture(IMG_BINDING_BASE + 0, tex, 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA8);

    The BIND_IDX value is the single source of truth on both sides — no manual offset arithmetic at call sites.
*/

