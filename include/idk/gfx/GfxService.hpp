#pragma once

#include "idk/gfx/fwd.hpp"
#include "idk/engine/Engine.hpp"


namespace idk::gfx
{
    class GfxService: public idk::EngineService
    {
    public:
        GfxService(idk::EngineContext&);
        ~GfxService();
        virtual void init(idk::Engine&) final;
        virtual void update(idk::Engine&) final;

    private:
        GraphicsDevice *mGfxDevice;

    };
}
