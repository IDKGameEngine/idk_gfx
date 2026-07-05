#pragma once

#include "idk_core/Engine.hpp"
#include "idk_core/Service.hpp"
#include "idk_gfx/renderer.hpp"

namespace idk
{
    class GfxService;
}

class idk::GfxService: public idk::core::Service
{
private:
    idk::gfx::RenderEngine *mRen;

public:
    GfxService(idk::core::IPlatformService*);
    ~GfxService();
    virtual void update(idk::IEngine*) final;
    virtual void shutdown(idk::IEngine*) final;

    idk::gfx::RenderEngine &getRenderer() { return *mRen; }

};
