#include "idk/gfx/gfx.hpp"
#include "idk/gfx/renderer.hpp"

idk::core::IRendererPtr idk::gfx::create_renderer(const idk::core::IWindowPtr &win)
{
    return std::make_shared<idk::gfx::RenderEngine>(win);
}

