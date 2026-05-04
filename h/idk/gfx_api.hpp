#pragma once

#include "idk/core/log.hpp"
#include "idk/core/double_buffer.hpp"

#include "idk/gfx/fwd.hpp"
#include "idk/gfx/render_interface.hpp"


namespace idk
{
    class GfxService;


    class GfxApi
    {
    private:
        using Request = idk::gfx::GfxReqType;

        idk::GfxService                 *srv_;
        DblBufferWriter<gfx::GfxRequest> writer_;

        template <Request R>
        void _send_request(const gfx::GfxRequestImpl<R> &req_impl, gfx::GfxResponseImpl<R> *res_impl)
        {
            writer_->push( gfx::GfxRequest::makeGfxRequest<R>(req_impl, res_impl) );
        }

    public:
        GfxApi(idk::GfxService*);

        #define IDK_XMACRO(Name) \
        void Name(const gfx::Name ## Request &req, gfx::Name ## Response *res) \
        { \
            _send_request<Request::Name>(req, res); \
        }
        IDK_GFXREQ_LIST
        #undef IDK_XMACRO

    
        idk::ThreadSafeAccess<idk::gfx::Camera> GetCameraLock();
        void FlushCommandQueue();

    };
}

