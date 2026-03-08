#pragma once

#include "idk/gfx/fwd.hpp"
#include <SDL3/SDL.h>
#include <glm/glm.hpp>


class idk::gfx::WindowSDL3: public idk::NonCopyable, public idk::NonMovable
{
public:
    const char   *mTitle;
    SDL_Window   *mSdlWin;
    SDL_GLContext mGlCtx;
    glm::ivec2    mSizei;
    glm::vec2     mSizef;

    WindowSDL3(const idk::core::WindowDesc&);
    ~WindowSDL3();

    void makeCurrent();
    void swapWindow();
    // virtual int   getWidth() const final;
    // virtual int   getHeight() const final;
    // virtual void *getNativeHandle() const final;
    // virtual void *getGpuContext() const final;
    // virtual void  pollEvents() final;

private:

};

