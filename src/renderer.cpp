#include "idk/gfx/gfx.hpp"
#include "idk/gfx/renderer.hpp"
#include "idk/gfx/shader.hpp"
#include "idk/gfx/window.hpp"
#include "idk_gfx/mesh.hpp"
#include "idk/core/metric.hpp"


using namespace idk::gfx;

static RenderProgram *m_winprg;


RenderEngine::RenderEngine(const idk::core::WindowDesc &windesc)
:   win_(new WindowSDL3(windesc))
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
}


RenderEngine::~RenderEngine()
{
    gl::DeleteVertexArrays(1, &mDummyVao);
}


void RenderEngine::beginFrame()
{
    // meshbuf_->loadMesh()
}


void RenderEngine::endFrame()
{
    win_->makeCurrent();

    // gl::ClearColor(0.0f, 0.75f, 0.25f, 1.0f);
    // gl::Clear(GL_COLOR_BUFFER_BIT);

    gl::UseProgram(m_winprg->mId);

    gl::BindVertexArray(mDummyVao);
    gl::DrawArrays(GL_TRIANGLES, 0, 3);


    win_->swapWindow();
}

