#include "idk/GfxService.hpp"

namespace idk
{
    GfxService::GfxService() = default;

    GfxService::~GfxService() = default;

    void GfxService::update(void *engine)
    {
        mRenderEngine.update(engine);
    }

    void GfxService::shutdown(void *engine)
    {
        (void)engine;
        mRenderEngine.shutdown();
    }

    void GfxService::setRefreshRateHz(uint64_t hz)
    {
        mRenderEngine.setRefreshRateHz(hz);
    }

    idk::gfx::RenderEngine &GfxService::getRenderEngine()
    {
        return mRenderEngine;
    }
}
