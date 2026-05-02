#pragma once

#include <cstddef>
#include <cstdint>
#include <glm/glm.hpp>

#include "idk/core/metric.hpp"


namespace idk::gfx
{
    #define IDK_GFXREQ_LIST \
        IDK_XMACRO(DebugOutputEnable) \
        IDK_XMACRO(AddComputeProgram) \
        IDK_XMACRO(AddRenderProgram) \
        IDK_XMACRO(BgColorSet) \
        IDK_XMACRO(BgColorAdd) \
        IDK_XMACRO(FgColorSet) \
        IDK_XMACRO(FgColorAdd)

    enum class GfxReqType: int
    {
        Invalid = 0,
        #define IDK_XMACRO(Name) Name,
        IDK_GFXREQ_LIST
        #undef IDK_XMACRO
    };

    struct GfxRequest;
    struct GfxResponse;

    struct DebugOutputEnableRequest
    {
        bool enabled;
    };
    struct DebugOutputEnableResponse
    {
        bool enabled;
    };

    struct AddComputeProgramRequest
    {
        const char *comp_path;
    };
    struct AddComputeProgramResponse
    {
        int64_t id;
    };

    struct AddRenderProgramRequest
    {
        const char *vert_path;
        const char *frag_path;
    };
    struct AddRenderProgramResponse
    {
        int64_t id;
    };

    struct BgColorSetRequest
    {
        glm::vec4 value;
    };
    struct BgColorSetResponse
    {
        glm::vec4 value;
    };

    struct BgColorAddRequest
    {
        glm::vec4 value;
    };
    struct BgColorAddResponse
    {
        glm::vec4 value;
    };

    struct FgColorSetRequest
    {
        glm::vec4 value;
    };
    struct FgColorSetResponse
    {
        glm::vec4 value;
    };

    struct FgColorAddRequest
    {
        glm::vec4 value;
    };
    struct FgColorAddResponse
    {
        glm::vec4 value;
    };
}



// #define GFXREQ_MEMBER(Obj, Name) \
//     reinterpret_cast<idk::gfx::detail::gfx_request<idk::gfx::GfxReqType::##Name>*>( \
//         reinterpret_cast<uint8_t*>(&Obj) + offsetof(idk::gfx::GfxRequest, as_##Name) \
//     )

struct idk::gfx::GfxRequest
{
    GfxReqType  type;
    GfxResponse *res;

    union
    {
        #define IDK_XMACRO(Name) Name ## Request as_##Name;
        IDK_GFXREQ_LIST
        #undef IDK_XMACRO
    };
};


struct idk::gfx::GfxResponse
{
    GfxReqType type;
    bool ready;

    union
    {
        #define IDK_XMACRO(Name) Name ## Response as_##Name;
        IDK_GFXREQ_LIST
        #undef IDK_XMACRO
    };
};
