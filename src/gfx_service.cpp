#include "idk/gfx_service.hpp"
#include "idk/gfx/renderer.hpp"
#include "idk/gfx/window.hpp"


idk::GfxService::GfxService()
:   Service(idk_typeid<GfxService>()),
    mCfg("asset/GfxService.cfg")
{
    const char *winName = mCfg["WINDOW_NAME"].getValue();
    int32_t winWidth = mCfg["WINDOW_WIDTH"].getValueI32();
    int32_t winHeight = mCfg["WINDOW_HEIGHT"].getValueI32();
    mWin = new gfx::WindowSDL3(core::WindowDesc(winName, winWidth, winHeight));
    mRen = new gfx::RenderEngine(*mWin);

    uint64_t refreshRateHz = mCfg["REFRESH_RATE"].getValueU64();
    mRen->setRefreshRateHz(refreshRateHz);
    VLOG_INFO("[GfxService::GfxService] refreshRateHz={}", refreshRateHz);
}

void idk::GfxService::startup(idk::IEngine*)
{
    VLOG_INFO("[GfxService::startup]");
}

void idk::GfxService::update(idk::IEngine *E)
{
    mRen->update(E);
}

void idk::GfxService::shutdown(idk::IEngine*)
{
    VLOG_INFO("[GfxService::shutdown]");
    mRen->shutdown();
}
