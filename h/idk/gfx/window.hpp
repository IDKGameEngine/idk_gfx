#pragma once

#include "idk/gfx/fwd.hpp"
#include <SDL3/SDL.h>
#include <glm/glm.hpp>


class idk::gfx::Window
{
public:
    Window(const core::WindowDesc&);
    ~Window();

    virtual int   getWidth() const final;
    virtual int   getHeight() const final;
    virtual void *getNativeHandle() const final;
    virtual void *getGpuContext() const final;
    virtual void  pollEvents() final;

private:
    const char *title_;
    SDL_Window *sdlWin_;
    SDL_GLContext glCtx_;

    glm::vec2 size_;
};

