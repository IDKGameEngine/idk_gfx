#pragma once

#include "idk/gfx/renderer.hpp"


namespace idk
{
    class GfxApi
    {
    private:
        using cmdtype = idk::gfx::GfxCmdType;

        core::DblBufferWriter<gfx::GfxCmd> writer_;
        idk::gfx::GfxCmd cmd_;
        void _push() { writer_->push(cmd_); }

    public:
        GfxApi(idk::gfx::RenderEngine *ren)
        :   writer_(ren->getQueueWriter()) {  }

        void debugOutputEnable();
        void debugOutputDisable();
        
        void bgColorAdd(const glm::vec4&);
        void bgColorSet(const glm::vec4&);
        
    };
}

