#pragma once

#include "libidk/platform/Platform.hpp"
#include "idk/gfx/RenderEngine.hpp"
#include "libidk/Engine.hpp"
#include "libidk/Service.hpp"

namespace idk
{
    class GfxService: public idk::core::Service
    {
    private:
        idk::Platform &mPlat;
        idk::gfx::RenderEngine mRen;

    public:
        GfxService(idk::Platform&);
        ~GfxService();
        virtual void update(idk::IEngine*) final;
        virtual void shutdown(idk::IEngine*) final;

        idk::gfx::RenderEngine &getRenderer() { return mRen; }

    };
}
