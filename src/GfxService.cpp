#include "idk/GfxService.hpp"
#include "idk/gfx/renderer.hpp"
#include "idk/gfx/window.hpp"
#include "idk_config/Memory.hpp"

idk::GfxService::GfxService(idk::core::IPlatformService *plat)
:   IDK_SERVICE_CTOR(GfxService),
    mRen(idk::New<gfx::RenderEngine>(plat))
{
    uint64_t tickRateHz = mCfg["TICKRATE_HZ"].toU64();
    mRen->setRefreshRateHz(tickRateHz);
    VLOG_INFO("[GfxService::GfxService] tickRateHz={}", tickRateHz);
}

idk::GfxService::~GfxService()
{

}

void idk::GfxService::update(idk::IEngine *E)
{
    mRen->update(E);

    // static constexpr uint64_t TIMESTEP_MS = 16;
    // static uint64_t accum = 0;
    // static uint64_t prevTime = Platform::getSysTimeMs();

    // uint64_t currTime = Platform::getSysTimeMs();
    // uint64_t frameTime = currTime - prevTime;
    // if (frameTime > maxFrameTime) { frameTime = maxFrameTime; }
    // accum += frameTime;

    // while (accum >= TIMESTEP_MS)
    // {
    //     prevState = currState;
    //     doStuff(currState);
    //     accum -= TIMESTEP_MS;
    // }

    // double alpha = double(accum) / double(TIMESTEP_MS);
    // lerpState = mix(prevState, currState, alpha);
    // render(lerpState);
}

void idk::GfxService::shutdown(idk::IEngine*)
{
    VLOG_INFO("[GfxService::shutdown]");
    mRen->shutdown();
}
