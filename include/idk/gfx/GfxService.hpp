#pragma once

#include "idk/gfx/fwd.hpp"
#include "idk/engine/Engine.hpp"


namespace idk::gfx
{
    class GfxService: public idk::EngineComponent
    {
    public:
        GfxService(idk::EngineContext&);
        ~GfxService();
        virtual void update() final;

    private:
        GraphicsDevice *mGfxDevice;

    };
}
