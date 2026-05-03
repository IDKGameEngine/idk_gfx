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


namespace idk::gfx
{
    class RenderEngine;
}


class idk::gfx::RenderEngine
{
public:
    std::atomic_bool alive_;
    std::atomic_bool flush_;

    RenderEngine(idk::gfx::WindowSDL3&);
    ~RenderEngine();
    void update(idk::IEngine*);
    void shutdown();
    
    std::mutex &getMutex();
    DblBufferWriter<GfxRequest> getGfxRequestWriter();

    static void debugOutputEnable(const DebugOutputEnableRequest&, DebugOutputEnableResponse*);
    void addComputeProgram(const AddComputeProgramRequest&, AddComputeProgramResponse*);
    void addRenderProgram(const AddRenderProgramRequest&, AddRenderProgramResponse*);

private:
    std::mutex                  mutex_;
    idk::gfx::WindowSDL3        &win_;
    RaiiFunc<void(bool)>        raii_;
    DoubleBuffer<GfxRequest>    cmd_queue_;
    DblBufferReader<GfxRequest> cmd_read_;

    UniformBufferWriter<slang::UniformBuffer03> uboWt3;

    gfx::TextureFormatDesc automataFmt;
    gfx::Texture  automataTexA;
    gfx::Texture  automataTexB;
    gfx::Texture *automataTexPtr[2];
    gfx::ComputeProgram automataProg;
    gfx::ComputeProgram clearProg;
    gfx::RenderProgram  winProg;
    GLuint mDummyVao;
    gfx::MeshBuffer meshbuf_;

    std::vector<gfx::ComputeProgram> computePrograms_;
    std::vector<gfx::RenderProgram> renderPrograms_;

    void _update_image();
};

