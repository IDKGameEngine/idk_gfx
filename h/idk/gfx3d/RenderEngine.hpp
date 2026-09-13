#pragma once

#include <cstdint>
#include <mutex>

#include "libidk/camera.hpp"

namespace idk::gfx
{
    class RenderEngine
    {
    public:
        RenderEngine();

        template <typename PlatformT>
        explicit RenderEngine(PlatformT &)
        : RenderEngine()
        {
        }

        ~RenderEngine();

        void update(void *engine = nullptr);
        void shutdown();
        void setRefreshRateHz(uint64_t hz);

        std::mutex &getMutex();
        idk::Camera &getCamera();
        void swapCamera();

        bool isAlive() const { return mAlive; }

    private:
        std::mutex mMutex;
        idk::Camera mCamera;
        uint64_t mRefreshRateHz = 60;
        bool mAlive = true;
    };
}
