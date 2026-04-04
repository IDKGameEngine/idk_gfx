#include "idk/gfx/renderer.hpp"
#include "idk/gfx/shader.hpp"
#include "idk/gfx/window.hpp"
#include "idk_gfx/mesh.hpp"
#include "idk/core/metric.hpp"

using namespace idk::gfx;

static RenderProgram *m_winprg;
static ComputeProgram *m_automata_prg;

static glm::vec4 bgtint_ = glm::vec4(1.0f);
static glm::vec4 fgtint_ = glm::vec4(1.0f);


RenderEngine::RenderEngine(const idk::core::WindowDesc &windesc)
:   win_(new WindowSDL3(windesc)),
    gfxread_(gfxqueue_),
    uboWt3()
{
    VLOG_INFO("gfx::Renderer Initialized");

    GLint mGlVersionMajor, mGlVersionMinor;
    gl::GetIntegerv(GL_MAJOR_VERSION, &mGlVersionMajor);
    gl::GetIntegerv(GL_MINOR_VERSION, &mGlVersionMinor);

    VLOG_INFO("Context supports OpenGL {}.{}", mGlVersionMajor, mGlVersionMinor);
    VLOG_INFO("Context supports OpenGL {}.{}", GLVersion.major, GLVersion.minor);

    this->debugOutputEnable();

    gl::CreateVertexArrays(1, &mDummyVao);

    meshbuf_ = new gfx__::MeshBuffer(64*idk::MEGA, 64*idk::MEGA);
    m_winprg = new RenderProgram("assets/shader/screenquad.vert", "assets/shader/screenquad.frag");
    m_automata_prg = new ComputeProgram("assets/shader/automata.comp");

    // uboWt3 = new UniformBufferWriter<slang::UniformBufferB3>();
    // (*uboWt3)->winsz
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

    // gl::ClearColor(0.0f, 0.75f, 0.25f, 1.0f);
    // gl::Clear(GL_COLOR_BUFFER_BIT);
    gl::UseProgram(m_winprg->mId);
    uboWt3.bind();
    // ubo3_->bindToIndex(ubo3_->BIND_IDX);

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

