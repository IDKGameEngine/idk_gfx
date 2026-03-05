#pragma once

#include "idk/core/renderer.hpp"
#include "idk/core/window.hpp"
#include "idk/gfx/gl_bindings.hpp"
#include <SDL3/SDL.h>


namespace idk::gfx
{
    class RendererOpenGL;
}


class idk::gfx::RendererOpenGL: public idk::core::IRenderer
{
public:
    RendererOpenGL(idk::core::IWindow *win);
    virtual ~RendererOpenGL();
    virtual void beginFrame() final;
    virtual void endFrame() final;
    virtual void onShutdown() final;

    void debugOutputEnable();
    void debugOutputDisable();

private:
    idk::core::IWindow *mWin;
    SDL_Window *mSdlWin;
    SDL_GLContext mGlCtx;
    GLuint mDummyVao;

};

