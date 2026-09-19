#pragma once

#include "idk/gfx/fwd.hpp"


namespace idk::gfx
{
    class RenderEngine
    {
    public:
        RenderEngine();
        ~RenderEngine();
        void update();

    private:
        GraphicsDevice *mGfxDevice;

    };
}
