#include "idk/GfxService.hpp"
#include "idk/gfx/renderer.hpp"
#include "idk/gfx/window.hpp"


idk::GfxService::GfxService()
:   Service("GfxService", idk_typeid<GfxService>())
{
    const char *winName = mCfg["WINDOW_NAME"].getValue();
    int32_t winWidth = mCfg["WINDOW_WIDTH"].getValueI32();
    int32_t winHeight = mCfg["WINDOW_HEIGHT"].getValueI32();
    mWin = new gfx::WindowSDL3(core::WindowDesc(winName, winWidth, winHeight));
    mRen = new gfx::RenderEngine(*mWin);

    uint64_t tickRateHz = mCfg["TICKRATE_HZ"].getValueU64();
    mRen->setRefreshRateHz(tickRateHz);
    VLOG_INFO("[GfxService::GfxService] tickRateHz={}", tickRateHz);
}

idk::GfxService::~GfxService()
{
    delete mWin;
    delete mRen;
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
