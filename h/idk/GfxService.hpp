#pragma once

#include "idk/core/engine.hpp"
#include "idk/core/cfgparser.hpp"
#include "idk/gfx/renderer.hpp"

namespace idk
{
    class GfxService;
}

class idk::GfxService: public idk::core::Service
{
private:
    idk::gfx::WindowSDL3 *mWin;
    idk::gfx::RenderEngine *mRen;

public:
    GfxService();
    ~GfxService();
    virtual void startup(idk::IEngine*) final;
    virtual void update(idk::IEngine*) final;
    virtual void shutdown(idk::IEngine*) final;

    idk::gfx::RenderEngine &getRenderer() { return *mRen; }

};
