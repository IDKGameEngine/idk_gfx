#include "idk/gfx/renderer_opengl.hpp"
#include "idk/gfx/gfx.hpp"

using namespace idk::gfx;


RendererOpenGL::RendererOpenGL(idk::core::IWindow *win)
:   mWin(win),
    mSdlWin(static_cast<SDL_Window*>(win->getNativeHandle())),
    mGlCtx(static_cast<SDL_GLContext>(win->getGpuContext()))
{
    if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress))
        VLOG_FATAL("gladLoadGLLoader failure");

    if (!SDL_GL_MakeCurrent(mSdlWin, mGlCtx))
        VLOG_ERROR("SDL_GL_MakeCurrent: {}", SDL_GetError());

    GLint mGlVersionMajor, mGlVersionMinor;
    gl::GetIntegerv(GL_MAJOR_VERSION, &mGlVersionMajor);
    gl::GetIntegerv(GL_MINOR_VERSION, &mGlVersionMinor);

    VLOG_INFO("Context supports OpenGL {}.{}", mGlVersionMajor, mGlVersionMinor);
    VLOG_INFO("Context supports OpenGL {}.{}", GLVersion.major, GLVersion.minor);

    this->debugOutputEnable();

    gl::CreateVertexArrays(1, &mDummyVao);
}


RendererOpenGL::~RendererOpenGL()
{
    gl::DeleteVertexArrays(1, &mDummyVao);
}


void RendererOpenGL::beginFrame()
{
    SDL_GL_MakeCurrent(mSdlWin, mGlCtx);
}

void RendererOpenGL::endFrame()
{
    gl::ClearColor(0.0f, 0.75f, 0.25f, 1.0f);
    gl::Clear(GL_COLOR_BUFFER_BIT);

    SDL_GL_SwapWindow(mSdlWin);
}

void RendererOpenGL::onShutdown()
{

}
