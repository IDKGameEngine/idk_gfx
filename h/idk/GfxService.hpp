#pragma once

#include "libidk/Engine.hpp"
#include "libidk/Service.hpp"
#include "idk/gfx/Platform.hpp"
#include "idk/gfx/RenderEngine.hpp"

namespace idk
{
    class GfxService;
}

class idk::GfxService: public idk::core::Service
{
private:
    idk::gfx::Platform     *mPlat;
    idk::gfx::RenderEngine *mRen;

public:
    GfxService();
    ~GfxService();
    virtual void update(idk::IEngine*) final;
    virtual void shutdown(idk::IEngine*) final;

    idk::gfx::RenderEngine &getRenderer() { return *mRen; }

};
