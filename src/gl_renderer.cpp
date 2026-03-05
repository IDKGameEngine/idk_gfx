#include "idk/gfx/gl_renderer.hpp"
#include "libidk/log.hpp"

using namespace idk::gfx;


RendererOpenGL::RendererOpenGL(idk::core::IWindow *win)
:   mWin(win)
{
    auto *sdlWin = static_cast<SDL_Window*>(win->getNativeHandle());
    auto *glCtx  = static_cast<SDL_GLContext>(win->getGpuContext());

    if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress))
        VLOG_FATAL("gladLoadGLLoader failure");

    if (!SDL_GL_MakeCurrent(sdlWin, glCtx))
        VLOG_ERROR("SDL_GL_MakeCurrent: {}", SDL_GetError());

    GLint mGlVersionMajor, mGlVersionMinor;
    gl::GetIntegerv(GL_MAJOR_VERSION, &mGlVersionMajor);
    gl::GetIntegerv(GL_MINOR_VERSION, &mGlVersionMinor);

    VLOG_INFO("Context supports OpenGL {}.{}", mGlVersionMajor, mGlVersionMinor);
    VLOG_INFO("Context supports OpenGL {}.{}", GLVersion.major, GLVersion.minor);

    gl::CreateVertexArrays(1, &mVAO);
}


RendererOpenGL::~RendererOpenGL()
{
    gl::DeleteVertexArrays(1, &mVAO);
}



void RendererOpenGL::beginFrame()
{
    SDL_Event e;
    while (SDL_PollEvent(&e))
    {
        if (e.type == SDL_EVENT_QUIT)
        {
            return;
        }

        // if (SDL_GetWindowFromEvent(&e))
        // {
        //     // if (auto *gfx = getService<gfxapi::RenderEngine>())
        //     // {
        //     //     gfx.on
        //     // }
        // }
    }

}


void RendererOpenGL::endFrame()
{
    auto *sdlWin = static_cast<SDL_Window*>(mWin->getNativeHandle());
    SDL_GL_SwapWindow(sdlWin);
}

