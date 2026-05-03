#pragma once

#include "idk/core/double_buffer.hpp"
#include "idk/gfx/render_interface.hpp"


namespace idk
{
    namespace gfx { class RenderEngine; }

    class GfxApi
    {
    private:
        using Request = idk::gfx::GfxReqType;

        core::DblBufferWriter<gfx::GfxRequest> writer_;
        // void *table_[size_t(Request::NumTypes)];

        template <Request R>
        void _send_request(const gfx::GfxRequestImpl<R> &req_impl, gfx::GfxResponseImpl<R> *res_impl)
        {
            writer_->push( gfx::GfxRequest::makeGfxRequest<R>(req_impl, res_impl) );
        }

    public:
        GfxApi(idk::gfx::RenderEngine*);

        #define IDK_XMACRO(Name) \
        void Name(const gfx::Name ## Request &req, gfx::Name ## Response *res) \
        { \
            _send_request<Request::Name>(req, res); \
        }
        IDK_GFXREQ_LIST
        #undef IDK_XMACRO

    };
}

