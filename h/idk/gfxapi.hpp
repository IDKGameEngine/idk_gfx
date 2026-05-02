#pragma once

#include "idk/core/double_buffer.hpp"
#include "idk/gfx/render_interface.hpp"


namespace idk
{
    namespace gfx { class RenderEngine; }

    class GfxApi
    {
    private:
        using cmdtype = idk::gfx::GfxReqType;

        core::DblBufferWriter<gfx::GfxRequest> writer_;
        idk::gfx::GfxRequest req_;

        void _push()
        {
            req_.res->ready = false;
            writer_->push(req_);
        }

    public:
        GfxApi(idk::gfx::RenderEngine*);

        void debugOutputEnable(gfx::GfxResponse*, bool);

        void addComputeProgram(gfx::GfxResponse*, const char *compute_filepath);
        void addRenderProgram(gfx::GfxResponse*, const char *vert_filepath, const char *frag_filepath);
        // ComputeProgram *getComputeProgram(uint64_t key);
        // RenderProgram *getRenderProgram(uint64_t key);

        void bgColorAdd(gfx::GfxResponse*, const glm::vec4&);
        void bgColorSet(gfx::GfxResponse*, const glm::vec4&);

    };
}

