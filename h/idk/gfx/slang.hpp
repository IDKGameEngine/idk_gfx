// https://shader-slang.org/slang/user-guide/glsl-target-specific

#ifndef IDK_GFX_SLANG_HPP
#define IDK_GFX_SLANG_HPP

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


#ifdef __cplusplus
    #include <cstdint>
    #include <glm/glm.hpp>
    namespace idk::gfx::slang
    {
        using vec2 = glm::vec2;
        using vec3 = glm::vec3;
        using vec4 = glm::vec4;
    }

#else
    typedef float2 vec2;
    typedef float3 vec3;
    typedef float4 vec4;

    #define UniformBufferRO(T) [[vk::binding(T::BIND_IDX, 0)]] ConstantBuffer<T, Std140DataLayout>
    #define StorageBufferRO(T) [[vk::binding(T::BIND_IDX, 0)]] StructuredBuffer<T, Std430DataLayout>
    #define StorageBufferRW(T) [[vk::binding(T::BIND_IDX, 0)]] RWStructuredBuffer<T, Std430DataLayout>
#endif


namespace idk
{
#ifdef __cplusplus
    namespace gfx::slang
    {
#endif

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
        vec4 pos[256];
        vec4 vel[256];
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


#ifdef __cplusplus
    } // namespace gfx::slang
#endif 

} // namespace idk

#endif // IDK_GFX_SLANG_HPP
