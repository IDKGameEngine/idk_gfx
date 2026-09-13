#include "idk/gfx3d/RenderEngine.hpp"

#include <utility>

namespace idk::gfx
{
    RenderEngine::RenderEngine()
    : mCamera(1.0f, 80.0f, 0.1f, 8000.0f)
    {
        mCamera.getTransform().SetPosition(glm::vec3(0.0f, 16.0f, 32.0f));
    }

    RenderEngine::~RenderEngine() = default;

    void RenderEngine::update(void *engine)
    {
        (void)engine;
    }

    void RenderEngine::shutdown()
    {
        mAlive = false;
    }

    void RenderEngine::setRefreshRateHz(uint64_t hz)
    {
        mRefreshRateHz = hz;
    }

    std::mutex &RenderEngine::getMutex()
    {
        return mMutex;
    }

    idk::Camera &RenderEngine::getCamera()
    {
        return mCamera;
    }

    void RenderEngine::swapCamera()
    {
    }
}
