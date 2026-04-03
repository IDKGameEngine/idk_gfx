// https://shader-slang.org/slang/user-guide/glsl-target-specific

#ifndef IDK_GFX_SLANG_HPP
#define IDK_GFX_SLANG_HPP

#ifdef __cplusplus
    #define IDK_SLANG_NAMESPACE idk::gfx::slang
    namespace IDK_SLANG_NAMESPACE
    {
        using vec2 = glm::vec2;
        using vec3 = glm::vec3;
        using vec4 = glm::vec4;
    }
#else
    #define IDK_SLANG_NAMESPACE idk
    typedef float2 vec2;
    typedef float3 vec3;
    typedef float4 vec4;
#endif


namespace IDK_SLANG_NAMESPACE
{
    static constexpr const uint32_t BINDING_UBO_0 = 0;
    static constexpr const uint32_t BINDING_UBO_1 = 1;
    static constexpr const uint32_t BINDING_UBO_2 = 2;
    static constexpr const uint32_t BINDING_UBO_3 = 3;

    static constexpr const uint32_t BINDING_SSBO_0 = 4;
    static constexpr const uint32_t BINDING_SSBO_1 = 5;
    static constexpr const uint32_t BINDING_SSBO_2 = 6;
    static constexpr const uint32_t BINDING_SSBO_3 = 7;

    struct UBO3_t
    {
        static constexpr const uint32_t BINDING_IDX = BINDING_UBO_3;

        vec4 winsz;
        vec4 bgtint;
        vec4 fgtint;
        vec4 gamepos;
    };

    struct SSBO_0
    {
        vec4 pos;
        vec4 vel;
    };

    struct SSBO_1
    {
        vec4 pos;
        vec4 vel;
    };

    #ifdef __cplusplus


    #endif

    // #ifdef __cplusplus
    //     template <uint32_t B> struct SSBO_Type { static constexpr uint32_t BINDING_IDX = B; };

    //     #define SSBO_TYPE(Idx_) \
            
    //         struct SSBO_Type<uint32_t Idx_> \
    //         { \
    //             static constexpr const uint32_t BINDING_IDX = Idx_; \
    //         };

    //     struct SSBO_0 SSBO_TYPE()

    //     using vec2 = glm::vec2;
    //     using vec3 = glm::vec3;
    //     using vec4 = glm::vec4;
    // #else
    //     typedef float2 vec2;
    //     typedef float3 vec3;
    //     typedef float4 vec4;
    // #endif

    // struct SSBO_Type<uint32_t Idx_>
    // {
    //     static constexpr const uint32_t BINDING_IDX = Idx_;
    // };

    // struct SSBO_0: SSBO_Type<BINDING_SSBO_0>
    // {
    //     vec4 particles;
    // };

    // struct SSBO_1: SSBO_Type<BINDING_SSBO_1>
    // {
    //     vec4 particles;
    // };

    // #ifdef __cplusplus
    //     template <uint32_t BindIdx>
    //     struct SSBO_Type
    //     {
    //         static constexpr const uint32_t BINDING_IDX = BindIdx;
    //     };

    //     struct SSBO0_t: public SSBO_Type<BINDING_SSBO0>
    //     {
            
    //     };

    //     struct SSBO1_t: public SSBO_Type<BINDING_SSBO1>
    //     {
    //         vec4 winsz;
    //         vec4 bgtint;
    //         vec4 fgtint;
    //         vec4 gamepos;
    //     };
    // #endif
}

#undef IDK_SLANG_NAMESPACE

#endif // IDK_GFX_SLANG_HPP
