#pragma once

#include <glm/glm.hpp>
#include <SDL3/SDL.h>

#include "idk/gfx/fwd.hpp"
#include "idk/gfx/buffer.hpp"
#include "idk/gfx/shader.hpp"
#include "idk/gfx/texture.hpp"
#include "idk/gfx/mesh.hpp"
#include "idk/gfx/render_interface.hpp"

#include "idk/slang.hpp"

#include "idk/core/raii.hpp"
#include "idk/core/double_buffer.hpp"

#include <map>


namespace idk
{
    namespace gfx
    {
        class RenderEngine;
    }

    class MeshBuffer;
}


class idk::gfx::RenderEngine: public idk::core::Service
{
public:
    RenderEngine(const idk::core::WindowDesc&);
    virtual ~RenderEngine();
    virtual void onUpdate(idk::IEngine*) final;
    virtual void onShutdown(idk::IEngine*) final;
    core::DblBufferWriter<GfxRequest> getGfxRequestWriter();

    static void debugOutputEnable(const DebugOutputEnableRequest&, DebugOutputEnableResponse*);
    void addComputeProgram(const AddComputeProgramRequest&, AddComputeProgramResponse*);
    void addRenderProgram(const AddRenderProgramRequest&, AddRenderProgramResponse*);
    // uint64_t addComputeProgram(const char *comp_path);
    // uint64_t addRenderProgram(const char *vert_path, const char *frag_path);

    // gfx::ComputeProgram *getComputeProgram(uint64_t key);
    // gfx::RenderProgram *getRenderProgram(uint64_t key);

private:
    gfx::WindowSDL3                  *win_;
    core::RaiiFunc<void(bool)>        raii_;
    core::DoubleBuffer<GfxRequest>    gfxreqs_;
    core::DblBufferReader<GfxRequest> gfxread_;

    UniformBufferWriter<slang::UniformBuffer03> uboWt3;

    gfx::TextureFormatDesc automataFmt;
    gfx::Texture  automataTexA;
    gfx::Texture  automataTexB;
    gfx::Texture *automataTexPtr[2];
    gfx::ComputeProgram automataProg;
    gfx::ComputeProgram clearProg;
    gfx::RenderProgram  winProg;
    GLuint mDummyVao;
    gfx::MeshBuffer *meshbuf_;

    std::vector<gfx::ComputeProgram> computePrograms_;
    std::vector<gfx::RenderProgram> renderPrograms_;

};

