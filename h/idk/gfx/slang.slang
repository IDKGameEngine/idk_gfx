// https://shader-slang.org/slang/user-guide/glsl-target-specific

#ifndef IDK_GFX_SLANG_HPP
#define IDK_GFX_SLANG_HPP

typedef float2 vec2;
typedef float3 vec3;
typedef float4 vec4;
typedef float3x3 mat3;
typedef float4x4 mat4;

namespace idk
{
    static const uint32_t BINDING_UBO_0 = 0;
    static const uint32_t BINDING_UBO_1 = 1;
    static const uint32_t BINDING_UBO_2 = 2;
    static const uint32_t BINDING_UBO_3 = 3;

    interface IUniformBuffer
    {
        static const uint32_t BINDING_IDX;
    };

    struct UniformBufferB0: IUniformBuffer
    {
        static const uint32_t BINDING_IDX = BINDING_UBO_0;
        vec4 mouse;
        vec4 rgba;
        mat4 T;
        mat4 V;
        mat4 P;
    };

    struct UniformBufferB3: IUniformBuffer
    {
        static const uint32_t BINDING_IDX = BINDING_UBO_3;

        vec4 winsz;
        vec4 bgtint;
        vec4 fgtint;
        vec4 gamepos;
    };


    static const uint32_t BINDING_SSBO_0 = 0;
    static const uint32_t BINDING_SSBO_1 = 1;
    static const uint32_t BINDING_SSBO_2 = 2;
    static const uint32_t BINDING_SSBO_3 = 3;
    static const uint32_t BINDING_SSBO_4 = 4;
    static const uint32_t BINDING_SSBO_5 = 5;
    static const uint32_t BINDING_SSBO_6 = 6;
    static const uint32_t BINDING_SSBO_7 = 7;

    interface IStorageBuffer
    {
        static const uint32_t BINDING_IDX;
    };

    struct StorageBufferB0: IStorageBuffer
    {
        static const uint32_t BINDING_IDX = BINDING_SSBO_0;
        vec4 pos;
        vec4 vel;
    };

    struct StorageBufferB1: IStorageBuffer
    {
        static const uint32_t BINDING_IDX = BINDING_SSBO_1;
        vec4 pos;
        vec4 vel;
    };


    #define SSBO_TYPE_BEGIN(Name_, Idx_) \
        struct Name_: IStorageBuffer \
        { \
            static const uint32_t BINDING_IDX = Idx_;

    #define SSBO_TYPE_END };

    SSBO_TYPE_BEGIN(StorageBufferB2, 2)
        vec4 xxx;
        vec4 yyy;
        vec4 zzz;
        vec4 www;
    SSBO_TYPE_END

    SSBO_TYPE_BEGIN(StorageBufferB3, 3);

    SSBO_TYPE_END
}

#endif // IDK_GFX_SLANG_HPP
