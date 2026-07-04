#pragma once

#include "idk/core/Engine.hpp"
#include "idk/core/Service.hpp"
#include "idk/gfx/renderer.hpp"

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
