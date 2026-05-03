#pragma once

#include "idk/core/log.hpp"
#include "idk/core/double_buffer.hpp"
#include "idk/gfx/render_interface.hpp"

namespace idk::gfx
{
    class RenderEngine;
}

namespace idk
{
    class GfxApi
    {
    private:
        using Request = idk::gfx::GfxReqType;
        idk::gfx::RenderEngine *ren_;
        DblBufferWriter<gfx::GfxRequest> writer_;

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
            VLOG_INFO("[GfxApi::" #Name "] A"); \
            _send_request<Request::Name>(req, res); \
            VLOG_INFO("[GfxApi::" #Name "] B"); \
        }
        IDK_GFXREQ_LIST
        #undef IDK_XMACRO

        void FlushCommandQueue();

    };
}

