#pragma once

#define IDK_GFX_OPENGL


#ifdef IDK_GFX_OPENGL
    #include "idk/gfx/renderer_opengl.hpp"
    namespace idk::gfx
    {
        using RenderEngine = RendererOpenGL;
    }

#elif defined(IDK_GFX_VULKAN)
    #include "idk/gfx/renderer_vulkan.hpp"
    namespace idk::gfx
    {
        using RenderEngine = RendererOpenVK;
    }

#endif

