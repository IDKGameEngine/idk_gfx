// https://shader-slang.org/slang/user-guide/glsl-target-specific

#ifdef __cplusplus
    #pragma once
    #include <cstdint>
    #include <glm/glm.hpp>
    #define IDK_SLANG_NAMESPACE idk::gfx::slang
    #define GFX_VEC2_TYPE glm::vec2
    #define GFX_VEC3_TYPE glm::vec3
    #define GFX_VEC4_TYPE glm::vec4

#else
    #define IDK_SLANG_NAMESPACE idk
    #define GFX_VEC2_TYPE float2
    #define GFX_VEC3_TYPE float3
    #define GFX_VEC4_TYPE float4

#endif


namespace IDK_SLANG_NAMESPACE
{
    static constexpr const uint32_t BINDING_SSBO0 = 0;
    static constexpr const uint32_t BINDING_SSBO1 = 1;
    static constexpr const uint32_t BINDING_SSBO2 = 2;
    static constexpr const uint32_t BINDING_SSBO3 = 3;

    static constexpr const uint32_t BINDING_UBO0 = 4;
    static constexpr const uint32_t BINDING_UBO1 = 5;
    static constexpr const uint32_t BINDING_UBO2 = 6;
    static constexpr const uint32_t BINDING_UBO3 = 7;

    struct UBO3_t
    {
        static constexpr const uint32_t BINDING_IDX = BINDING_UBO3;

        GFX_VEC4_TYPE winsz;
        GFX_VEC4_TYPE bgtint;
        GFX_VEC4_TYPE fgtint;
        GFX_VEC4_TYPE gamepos;
    };


    #ifdef __cplusplus
        template <uint32_t Idx_>
        struct SSBO_Type
        {
            static constexpr const uint32_t BINDING_IDX = Idx_;
        };

        struct SSBO0_t: public SSBO_Type<BINDING_SSBO0>
        {
            
        };

        struct SSBO1_t: public SSBO_Type<BINDING_SSBO1>
        {
            GFX_VEC4_TYPE winsz;
            GFX_VEC4_TYPE bgtint;
            GFX_VEC4_TYPE fgtint;
            GFX_VEC4_TYPE gamepos;
        };
    #endif
}


#undef IDK_SLANG_NAMESPACE
