#pragma once

#include <cstdint>

#include "idk/gfx/RenderEngine.hpp"

namespace idk
{
    class GfxService
    {
    public:
        GfxService();
        ~GfxService();

        template <typename PlatformT>
        explicit GfxService(PlatformT &platform)
        : mRenderEngine(platform)
        {
            (void)platform;
        }

        void update(void *engine = nullptr);
        void shutdown(void *engine = nullptr);
        void setRefreshRateHz(uint64_t hz);

        idk::gfx::RenderEngine &getRenderEngine();

    private:
        idk::gfx::RenderEngine mRenderEngine;
    };
}
