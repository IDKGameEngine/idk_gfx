#include "idk/gfx_service.hpp"

#include "idk/gfx/renderer.hpp"
#include "idk/gfx/window.hpp"


idk::GfxService::GfxService(const idk::core::WindowDesc &windesc)
:   Service(idk::PeriodicTimer(1000.0 / 60.0)),
    win_(new gfx::WindowSDL3(windesc)),
    ren_(new gfx::RenderEngine(*win_))
{

}


void idk::GfxService::_startup(idk::IEngine*)
{
    VLOG_INFO("[idk::GfxService::_startup]");
}

void idk::GfxService::_update(idk::IEngine *E)
{
    // win_.makeCurrent();
    // ren_.renderToWindow(win_);
    // win_.swapWindow();

    ren_->update(E);
}

void idk::GfxService::_shutdown(idk::IEngine*)
{
    VLOG_INFO("[idk::GfxService::_shutdown]");
    ren_->shutdown();
}

