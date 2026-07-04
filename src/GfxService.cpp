#include "idk/GfxService.hpp"
#include "idk/gfx/renderer.hpp"
#include "idk/gfx/window.hpp"


idk::GfxService::GfxService(idk::core::IPlatformService *plat)
:   IDK_SERVICE_CTOR(GfxService),
    mRen(new gfx::RenderEngine(plat))
{
    uint64_t tickRateHz = mCfg["TICKRATE_HZ"].toU64();
    mRen->setRefreshRateHz(tickRateHz);
    VLOG_INFO("[GfxService::GfxService] tickRateHz={}", tickRateHz);
}

idk::GfxService::~GfxService()
{
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
