#include "idk/gfx/gfx.hpp"
#include "idk/gfx/renderer.hpp"


void idk::gfx::main(idk::core::IWindow *ptr)
{
    idk::gfx::RenderEngine ren(ptr);


    ren.onUpdate();

}

