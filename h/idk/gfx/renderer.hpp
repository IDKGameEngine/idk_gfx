#pragma once

#include "idk/gfx/fwd.hpp"
#include <SDL3/SDL.h>


namespace idk::gfx
{
    class RenderEngine;
}


class idk::gfx::RenderEngine: public idk::core::IRenderer
{
public:
    RenderEngine(const idk::core::IWindowPtr &win);
    virtual ~RenderEngine();
    virtual void beginFrame() final;
    virtual void endFrame() final;

    virtual void onUpdate() final;
    virtual void onEnable() final;
    virtual void onDisable() final;
    virtual void onRestart() final;
    virtual void onShutdown() final;

    void debugOutputEnable();
    void debugOutputDisable();

private:
    idk::core::IWindowPtr mWin;
    SDL_Window *mSdlWin;
    SDL_GLContext mGlCtx;
    GLuint mDummyVao;

};

