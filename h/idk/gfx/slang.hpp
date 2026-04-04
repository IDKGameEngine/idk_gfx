// https://shader-slang.org/slang/user-guide/glsl-target-specific

#ifndef IDK_GFX_SLANG_HPP
#define IDK_GFX_SLANG_HPP

#ifdef __SLANG__
    typedef float2 vec2;
    typedef float3 vec3;
    typedef float4 vec4;
#endif


namespace idk
{
#ifdef __cplusplus
    namespace gfx::slang
    {
        using vec2 = glm::vec2;
        using vec3 = glm::vec3;
        using vec4 = glm::vec4;
#endif


#ifdef __cplusplus
    #define UBO_TYPE_BEGIN(Idx_) \
        struct UniformBufferB##Idx_ \
        { \
            static constexpr const uint32_t BIND_IDX = Idx_;
    #define UBO_TYPE_END(Idx_) \
        }; \
        struct UniformReaderB##Idx_ { UniformBufferB##Idx_ dataIn; }; \
        struct UniformWriterB##Idx_ { UniformBufferB##Idx_ dataOut; };

    #define SSBO_TYPE_BEGIN(Idx_) \
        struct StorageBufferB##Idx_ \
        { \
            static constexpr const uint32_t BIND_IDX = Idx_;
    #define SSBO_TYPE_END(Idx_) \
        }; \
        struct StorageReaderB##Idx_ { StorageBufferB##Idx_ dataIn; }; \
        struct StorageWriterB##Idx_ { StorageBufferB##Idx_ dataOut; };

#else
    #define UBO_TYPE_BEGIN(Idx_) \
        struct UniformBufferB##Idx_ \
        { \
            static constexpr const uint32_t BIND_IDX = Idx_;
    #define UBO_TYPE_END(Idx_) \
        }; \
        struct UniformReaderB##Idx_ \
        { \
            [[vk::binding(Idx_, 0)]] \
            ConstantBuffer<UniformBufferB##Idx_> dataIn; \
        }; \

    #define SSBO_TYPE_BEGIN(Idx_) \
        struct StorageBufferB##Idx_ \
        { \
            static constexpr const uint32_t BIND_IDX = Idx_;
    #define SSBO_TYPE_END(Idx_) \
        }; \
        struct StorageReaderB##Idx_ \
        { \
            [[vk::binding(Idx_, 0)]] \
            StructuredBuffer<StorageBufferB##Idx_, Std430DataLayout> dataIn; \
        }; \
        struct StorageWriterB##Idx_ \
        { \
            [[vk::binding(Idx_, 0)]] \
            RWStructuredBuffer<StorageBufferB##Idx_, Std430DataLayout> dataOut; \
        };

#endif

    UBO_TYPE_BEGIN(0)
        vec4 rgba;
        vec4 xyzw;
    UBO_TYPE_END(0)

    UBO_TYPE_BEGIN(3)
        vec4 winsz;
        vec4 bgtint;
        vec4 fgtint;
        vec4 gamepos;
    UBO_TYPE_END(3)


    SSBO_TYPE_BEGIN(0)
        vec4 pos;
        vec4 vel;
    SSBO_TYPE_END(0)

    SSBO_TYPE_BEGIN(1)
        vec4 pos;
        vec4 vel;
    SSBO_TYPE_END(1)

    SSBO_TYPE_BEGIN(2)
        vec4 junk[1024];
    SSBO_TYPE_END(2)

    SSBO_TYPE_BEGIN(3)
        vec4 data0[512];
        vec4 data1[512];
    SSBO_TYPE_END(3)


#ifdef __cplusplus
    } // namespace gfx::slang
#endif 

} // namespace idk

#endif // IDK_GFX_SLANG_HPP
