#pragma once

#include "idk/gfx/fwd.hpp"
#include "idk/core/double_buffer.hpp"

// static     gfxbuffer;
// static core::DblBufferReader<idk::gfx::CmdData> gfxread(gfxbuffer);
// static core::DblBufferWriter<idk::gfx::CmdData> gfxwrite(gfxbuffer);

#include <glm/glm.hpp>
#include <SDL3/SDL.h>

namespace idk::gfx
{
    class RenderEngine;

    enum class GfxCmdType: int
    {
        Invalid = 0,
        DebugOutputEnable,
        BgColorSet,
        BgColorAdd,
        FgColorSet,
        FgColorAdd,
        SomethingElse
    };

    struct GfxCmd
    {
        GfxCmdType type;

        union
        {
            bool as_debugOutputEnable;
            glm::vec4 as_rgba;
        };
    };

}

namespace idk::gfx__
{
    class MeshBuffer;
}


class idk::gfx::RenderEngine: public idk::core::Service
{
public:
    RenderEngine(const idk::core::WindowDesc&);
    virtual ~RenderEngine();
    virtual void onUpdate(idk::IEngine*) final;
    virtual void onShutdown(idk::IEngine*) final;

    void debugOutputEnable();
    void debugOutputDisable();

    core::DblBufferWriter<GfxCmd> getQueueWriter();

private:
    gfx::WindowSDL3 *win_;
    core::DoubleBuffer<GfxCmd> gfxqueue_;
    core::DblBufferReader<GfxCmd> gfxread_;

    GLuint mDummyVao;
    gfx__::MeshBuffer *meshbuf_;

};

