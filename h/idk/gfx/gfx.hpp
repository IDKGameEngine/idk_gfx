#pragma once

#include "idk/core/renderer.hpp"
#include <memory>

namespace idk::gfx
{
    idk::core::IRendererPtr create_renderer(const idk::core::IWindowPtr&);
}

