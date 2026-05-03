#pragma once

#include <atomic>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <glm/glm.hpp>

#include "idk/core/metric.hpp"


namespace idk::gfx
{
    class RenderEngine;
    class ComputeProgram;
    class RenderProgram;

    #define IDK_GFXREQ_LIST \
        IDK_XMACRO(DebugOutputEnable) \
        IDK_XMACRO(AddComputeProgram) \
        IDK_XMACRO(AddRenderProgram) \
        IDK_XMACRO(GetComputeProgram) \
        IDK_XMACRO(GetRenderProgram) \
        IDK_XMACRO(BgColorSet) \
        IDK_XMACRO(BgColorAdd)

    enum class GfxReqType: uint64_t
    {
        Invalid = 0,

        #define IDK_XMACRO(Name) Name,
        IDK_GFXREQ_LIST
        #undef IDK_XMACRO

        NumTypes
    };


    struct GfxRequest;

    struct GfxResponse
    {
    private:
        friend class idk::gfx::RenderEngine;
        std::atomic_bool ready_;
        void make_ready() { ready_.notify_all(); }        

    public:
        GfxResponse(): ready_(false) {  };

        bool is_ready()
        {
            return ready_.load();
        }

        void await_ready()
        {
            while (!is_ready()) {  }
        }
    };


    template <GfxReqType R>
    struct GfxRequestImpl;

    template <GfxReqType R>
    struct GfxResponseImpl;


    template<>
    struct GfxRequestImpl<GfxReqType::DebugOutputEnable>
    {
        bool enabled;
        GfxRequestImpl(bool e): enabled(e) {  }
    };
    template<>
    struct GfxResponseImpl<GfxReqType::DebugOutputEnable>: public GfxResponse
    {
        bool enabled;
        GfxResponseImpl(): enabled(false) {  }
    };


    template<>
    struct GfxRequestImpl<GfxReqType::AddComputeProgram>
    {
        const char *comp_path;
        GfxRequestImpl(const char *cpath): comp_path(cpath) {  }
    };
    template<>
    struct GfxResponseImpl<GfxReqType::AddComputeProgram>: public GfxResponse
    {
        int64_t id;
        GfxResponseImpl(): id(-1) { }
    };


    template<>
    struct GfxRequestImpl<GfxReqType::AddRenderProgram>
    {
        const char *vert_path;
        const char *frag_path;
        GfxRequestImpl(const char *v, const char *f): vert_path(v), frag_path(f) {  }
    };
    template<>
    struct GfxResponseImpl<GfxReqType::AddRenderProgram>: public GfxResponse
    {
        int64_t id;
        GfxResponseImpl(): id(-1) {  }
    };


    template<>
    struct GfxRequestImpl<GfxReqType::GetComputeProgram>
    {
        int64_t id;
        GfxRequestImpl(int64_t i): id(i) {  }
    };
    template<>
    struct GfxResponseImpl<GfxReqType::GetComputeProgram>: public GfxResponse
    {
        gfx::ComputeProgram *prog;
        GfxResponseImpl(): prog(nullptr) {  }
    };


    template<>
    struct GfxRequestImpl<GfxReqType::GetRenderProgram>
    {
        int64_t id;
        GfxRequestImpl(int64_t i): id(i) {  }
    };
    template<>
    struct GfxResponseImpl<GfxReqType::GetRenderProgram>: public GfxResponse
    {
        gfx::RenderProgram *prog;
        GfxResponseImpl(): prog(nullptr) {  }
    };


    template<>
    struct GfxRequestImpl<GfxReqType::BgColorSet>
    {
        glm::vec4 value;
        GfxRequestImpl(const glm::vec4 &v): value(v) {  }
    };
    template<>
    struct GfxResponseImpl<GfxReqType::BgColorSet>: public GfxResponse
    {
        glm::vec4 value;
        GfxResponseImpl(): value(0.0f) {  }
    };


    template<>
    struct GfxRequestImpl<GfxReqType::BgColorAdd>
    {
        glm::vec4 value;
        GfxRequestImpl(const glm::vec4 &v): value(v) {  }
    };
    template<>
    struct GfxResponseImpl<GfxReqType::BgColorAdd>: public GfxResponse
    {
        glm::vec4 value;
        GfxResponseImpl(): value(0.0f) {  }
    };


    #define IDK_XMACRO(Name) using Name ## Request = GfxRequestImpl<GfxReqType::Name>;
    IDK_GFXREQ_LIST
    #undef IDK_XMACRO

    #define IDK_XMACRO(Name) using Name ## Response = GfxResponseImpl<GfxReqType::Name>;
    IDK_GFXREQ_LIST
    #undef IDK_XMACRO

}



struct idk::gfx::GfxRequest
{
public:
    GfxReqType  type;
    GfxResponse *res;

    union {
        char as_bytes[0];

        #define IDK_XMACRO(Name) Name ## Request as_##Name;
        IDK_GFXREQ_LIST
        #undef IDK_XMACRO
    };

    GfxRequest(): type(GfxReqType::Invalid), res(nullptr) {  }

    template <GfxReqType R>
    static GfxRequest makeGfxRequest(const GfxRequestImpl<R> &req_impl, GfxResponseImpl<R> *res_impl)
    {
        GfxRequest req;

        req.type = R;
        req.res  = res_impl;
        std::memcpy(req.as_bytes, &req_impl, sizeof(gfx::GfxRequestImpl<R>));

        return req;
    }
};


