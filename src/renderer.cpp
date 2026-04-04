#include "idk/gfx/renderer.hpp"
#include "idk/gfx/framebuffer.hpp"
#include "idk/gfx/texture.hpp"
#include "idk/gfx/window.hpp"
#include "idk/core/metric.hpp"

using namespace idk::gfx;

extern void gfxDebugOutputEnable(bool);
static glm::vec4 bgtint_ = glm::vec4(1.0f);
static glm::vec4 fgtint_ = glm::vec4(1.0f);


RenderEngine::RenderEngine(const idk::core::WindowDesc &windesc)
:   win_(new WindowSDL3(windesc)),
    raii_(gfxDebugOutputEnable, true),
    gfxread_(gfxqueue_),
    uboWt3(),
    automataFmt(GL_R8, GL_RGBA, GL_UNSIGNED_BYTE),
    automataTexA(slang::AUTOMATA_WIDTH, slang::AUTOMATA_WIDTH, nullptr, automataFmt),
    automataTexB(slang::AUTOMATA_WIDTH, slang::AUTOMATA_WIDTH, nullptr, automataFmt),
    automataProg("assets/shader/automata.comp"),
    clearProg("assets/shader/clear.comp"),
    winProg("assets/shader/screenquad.vert", "assets/shader/screenquad.frag")
{
    automataTexPtr[0] = &automataTexA;
    automataTexPtr[1] = &automataTexB;
    VLOG_INFO("gfx::Renderer Initialized");

    GLint mGlVersionMajor, mGlVersionMinor;
    gl::GetIntegerv(GL_MAJOR_VERSION, &mGlVersionMajor);
    gl::GetIntegerv(GL_MINOR_VERSION, &mGlVersionMinor);
    VLOG_INFO("Context supports OpenGL {}.{}", mGlVersionMajor, mGlVersionMinor);

    gl::CreateVertexArrays(1, &mDummyVao);
    meshbuf_ = new gfx::MeshBuffer(64*idk::MEGA, 64*idk::MEGA);

    gl::UseProgram(clearProg.mId);
    automataTexPtr[0]->bindImageTexture(8);
    automataTexPtr[1]->bindImageTexture(9);
    gl::DispatchCompute(slang::AUTOMATA_WIDTH/8, slang::AUTOMATA_WIDTH/8, 1);
    gl::MemoryBarrier(GL_ALL_BARRIER_BITS);
    std::swap(automataTexPtr[0], automataTexPtr[1]);

}


RenderEngine::~RenderEngine()
{
    gl::DeleteVertexArrays(1, &mDummyVao);
}


void RenderEngine::onUpdate(idk::IEngine *engine)
{
    SDL_Event e;
    while (SDL_PollEvent(&e))
    {
        if (e.type == SDL_EVENT_QUIT)
        {
            VLOG_INFO("SDL_EVENT_QUIT");
            engine->shutdown();
        }
    }

    while (!gfxread_->empty())
    {
        auto &cmd = gfxread_->front();

        switch (cmd.type)
        {
            case GfxCmdType::DebugOutputEnable:
                gfxDebugOutputEnable(cmd.as_debugOutputEnable);
                break;

            case GfxCmdType::BgColorSet:
                uboWt3->gamepos = cmd.as_rgba;
                break;

            case GfxCmdType::BgColorAdd:
                uboWt3->gamepos += cmd.as_rgba;
                break;

            case GfxCmdType::FgColorSet:
                uboWt3->fgtint = cmd.as_rgba;
                break;

            case GfxCmdType::FgColorAdd:
                uboWt3->fgtint += cmd.as_rgba;
                break;

            default:
                VLOG_FATAL("gfx::GfxCmdType is Invalid!");
                break;
        }

        gfxread_->pop();
    }

    win_->makeCurrent();
    uboWt3->winsz = glm::vec4(win_->mSizef, 0.0f, 0.0f);

    // if (ubo3_->mDirty)
    {
        uboWt3.sendToGpu();
    }

    gl::UseProgram(automataProg.mId);
    automataTexPtr[0]->bindImageTexture(8);
    automataTexPtr[1]->bindImageTexture(9);
    gl::DispatchCompute(slang::AUTOMATA_WIDTH/8, slang::AUTOMATA_WIDTH/8, 1);
    gl::MemoryBarrier(GL_ALL_BARRIER_BITS);
    std::swap(automataTexPtr[0], automataTexPtr[1]);


    gl::UseProgram(winProg.mId);
    automataTexPtr[0]->bindImageTexture(8);
    uboWt3.bind();
    gl::BindVertexArray(mDummyVao);
    gl::DrawArrays(GL_TRIANGLES, 0, 3);

    win_->swapWindow();

    gfxqueue_.swapBuffers();
}


void RenderEngine::onShutdown(idk::IEngine*)
{
    VLOG_INFO("RenderEngine::onShutdown");
}


idk::core::DblBufferWriter<GfxCmd> RenderEngine::getQueueWriter()
{
    return idk::core::DblBufferWriter<GfxCmd>(gfxqueue_);
}

