#pragma once

#include <glm/glm.hpp>
#include <SDL3/SDL.h>

#include "idk/gfx/fwd.hpp"
#include "idk/gfx/buffer.hpp"
#include "idk/gfx/shader.hpp"
#include "idk/gfx/slang.hpp"
#include "idk/gfx/texture.hpp"

#include "idk/core/raii.hpp"
#include "idk/core/double_buffer.hpp"


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
    core::DblBufferWriter<GfxCmd> getQueueWriter();

private:
    gfx::WindowSDL3 *win_;
    core::RaiiFunc<void(bool)> raii_;
    core::DoubleBuffer<GfxCmd> gfxqueue_;
    core::DblBufferReader<GfxCmd> gfxread_;
    UniformBufferWriter<slang::UniformBuffer03> uboWt3;

    gfx::TextureFormatDesc automataFmt;
    gfx::Texture automataTexA;
    gfx::Texture automataTexB;
    gfx::ComputeProgram automataProg;
    gfx::ComputeProgram clearProg;
    gfx::RenderProgram  winProg;
    GLuint mDummyVao;
    gfx__::MeshBuffer *meshbuf_;

};

