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
    mSizef(glm::vec2(mSizei))
{
    if (false == SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_CAMERA))
    {
        VLOG_FATAL("{}", SDL_GetError());
    }
    std::filesystem::current_path(std::filesystem::path(SDL_GetBasePath()));

    if (!SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE))
        VLOG_ERROR("{}", SDL_GetError());

    if (!SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4))
        VLOG_ERROR("{}", SDL_GetError());

    if (!SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 5))
        VLOG_ERROR("{}", SDL_GetError());

    if (!SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1))
        VLOG_ERROR("{}", SDL_GetError());

    if (!SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE,  24))
        VLOG_ERROR("{}", SDL_GetError());

    if (!SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8))
        VLOG_ERROR("{}", SDL_GetError());

    mSdlWin = SDL_CreateWindow(mTitle, mSizei.x, mSizei.y, SDL_WINDOW_OPENGL);
    if (mSdlWin == NULL)
        VLOG_FATAL("SDL_CreateWindow: {}", SDL_GetError());

    mGlCtx = SDL_GL_CreateContext(mSdlWin);
    if (mGlCtx == nullptr)
        VLOG_FATAL("SDL_GL_CreateContext: {}", SDL_GetError());

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


