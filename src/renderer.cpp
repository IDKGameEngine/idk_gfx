#include "idk/gfx/gfx.hpp"
#include "idk/gfx/renderer.hpp"
#include "idk/gfx/buffer.hpp"
#include "idk/gfx/shader.hpp"
#include "idk/gfx/window.hpp"
#include "idk/gfx/slang.hpp"
#include "idk_gfx/mesh.hpp"
#include "idk/core/metric.hpp"


using namespace idk::gfx;

static RenderProgram *m_winprg;
static ComputeProgram *m_automata_prg;

static glm::vec4 bgtint_ = glm::vec4(1.0f);
static glm::vec4 fgtint_ = glm::vec4(1.0f);

static UboWrapperT<slang::UBO3_t> *ubo3_;



RenderEngine::RenderEngine( const idk::core::WindowDesc &windesc,
                            core::DblBufferReader<CmdData> &cmdbuf )
:   win_(new WindowSDL3(windesc)),
    cmdbuf_(cmdbuf)
{
    if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress))
        VLOG_FATAL("gladLoadGLLoader failure");

    if (!SDL_GL_MakeCurrent(win_->mSdlWin, win_->mGlCtx))
        VLOG_ERROR("SDL_GL_MakeCurrent: {}", SDL_GetError());

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

    ubo3_ = new UboWrapperT<slang::UBO3_t>();
}


RenderEngine::~RenderEngine()
{
    gl::DeleteVertexArrays(1, &mDummyVao);
}


void RenderEngine::beginFrame()
{
    auto &ubo3 = *ubo3_;

    while (!cmdbuf_->empty())
    {
        auto &cmd = cmdbuf_->front();

        switch (cmd.type)
        {
            case CmdType::BgColorSet:
                ubo3_->mDirty = true;
                ubo3->gamepos = cmd.as_rgba;
                break;

            case CmdType::BgColorAdd:
                ubo3_->mDirty = true;
                ubo3->gamepos += cmd.as_rgba;
                break;

            case CmdType::FgColorSet:
                ubo3_->mDirty = true;
                ubo3->fgtint = cmd.as_rgba;
                break;

            case CmdType::FgColorAdd:
                ubo3_->mDirty = true;
                ubo3->fgtint += cmd.as_rgba;
                break;

            default:
                VLOG_FATAL("gfx::CmdType is Invalid!");
                break;
        }

        cmdbuf_->pop();
    }
}



void RenderEngine::endFrame()
{
    win_->makeCurrent();
    (*ubo3_)->winsz = glm::vec4(win_->mSizef, 0.0f, 0.0f);

    if (ubo3_->mDirty)
    {
        ubo3_->sendToGpu();
    }

    // gl::ClearColor(0.0f, 0.75f, 0.25f, 1.0f);
    // gl::Clear(GL_COLOR_BUFFER_BIT);
    gl::UseProgram(m_winprg->mId);

    ubo3_->bindToIndex(ubo3_->BINDING_IDX);

    gl::BindVertexArray(mDummyVao);
    gl::DrawArrays(GL_TRIANGLES, 0, 3);

    win_->swapWindow();
}

