#include "idk/gfx/window.hpp"
#include "idk/core/log.hpp"
#include <filesystem>

using namespace idk::gfx;


// static void file_dialog_callback(void *userdata, const char *const *filelist, int filter)
// {
//     (void)userdata;
//     (void)filelist;
//     (void)filter;
//     printf("[file_dialog_callback] WOOP\n");
// }


WindowSDL3::WindowSDL3(const idk::core::WindowDesc& desc)
:   mTitle(desc.title),
    mSdlWin(nullptr),
    mGlCtx(nullptr),
    mSizei(desc.width, desc.height),
    mSizef(glm::vec2(mSizei)),
    mAspect(mSizef.x / mSizef.y)
{
    VLOG_INFO("[WindowSDL3::WindowSDL3]");

    mSdlWin = SDL_CreateWindow(mTitle, mSizei.x, mSizei.y, SDL_WINDOW_OPENGL);
    if (mSdlWin == NULL)
        VLOG_FATAL("SDL_CreateWindow: {}", SDL_GetError());

    mGlCtx = SDL_GL_CreateContext(mSdlWin);
    if (mGlCtx == nullptr)
        VLOG_FATAL("SDL_GL_CreateContext: {}", SDL_GetError());

    if (!SDL_GL_SetSwapInterval(1))
        VLOG_WARN("SDL_GL_SetSwapInterval: {}", SDL_GetError());

    int interval;
    if (!SDL_GL_GetSwapInterval(&interval))
        VLOG_WARN("SDL_GL_GetSwapInterval: {}", SDL_GetError());
    else
        VLOG_INFO("SDL_GL_GetSwapInterval: interval={}", interval);

    if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress))
        VLOG_FATAL("gladLoadGLLoader failure");

    if (!SDL_GL_MakeCurrent(mSdlWin, mGlCtx))
        VLOG_ERROR("SDL_GL_MakeCurrent: {}", SDL_GetError());
    
    SDL_SetWindowRelativeMouseMode(mSdlWin, true);
    // SDL_ShowOpenFileDialog(file_dialog_callback, nullptr, mSdlWin, NULL, 0, NULL, true);
}

WindowSDL3::~WindowSDL3()
{
    SDL_DestroyWindow(mSdlWin);
    SDL_Quit();
}


void WindowSDL3::makeCurrent()
{
    SDL_GL_MakeCurrent(mSdlWin, mGlCtx);
}

void WindowSDL3::swapWindow()
{
    SDL_GL_SwapWindow(mSdlWin);
}


// int WindowSDL3::getWidth() const
// {
//     return mWidth;
// }

// int WindowSDL3::getHeight() const
// {
//     return mHeight;
// }

// void *WindowSDL3::getNativeHandle() const
// {
//     return mSdlWin;
// }

// void *WindowSDL3::getGpuContext() const
// {
//     return mGlCtx;
// }

// void WindowSDL3::pollEvents()
// {

// }


