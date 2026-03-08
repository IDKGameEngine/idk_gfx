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

    enum class CmdType: int
    {
        Invalid = 0,
        BgColorSet,
        BgColorAdd,
        FgColorSet,
        FgColorAdd,
        SomethingElse
    };

    struct CmdData
    {
        idk::gfx::CmdType type;

        union
        {
            glm::vec4 as_rgba;
        };
    };

}

namespace idk::gfx__
{
    class MeshBuffer;
}


class idk::gfx::RenderEngine: public idk::core::IRenderer
{
public:
    RenderEngine(const idk::core::WindowDesc&, core::DblBufferReader<CmdData>&);
    virtual ~RenderEngine();
    virtual void beginFrame() final;
    virtual void endFrame() final;

    void debugOutputEnable();
    void debugOutputDisable();

private:
    gfx::WindowSDL3 *win_;
    core::DblBufferReader<CmdData> &cmdbuf_;

    GLuint mDummyVao;
    gfx__::MeshBuffer *meshbuf_;

};

