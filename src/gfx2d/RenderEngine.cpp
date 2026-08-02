#include "idk/gfx2d/RenderEngine.hpp"


idk::gfx::RenderEngine::RenderEngine(idk::platform::Platform &plat)
:   mPlat(plat),
    mWin(plat.getWindow())
{
    int winWidth = mWin.getWidth();
    int winHeight = mWin.getHeight();
    mCamera = idk::Camera(float(winWidth)/winHeight, 80.0f, 0.1f, 8000.0f),
    mCamera.getTransform().SetPosition(glm::vec3(0.0f, 16.0f, 32.0f));
}


idk::gfx::RenderEngine::~RenderEngine()
{

}


void idk::gfx::RenderEngine::update(idk::IEngine *E)
{
    (void)E;

}


void idk::gfx::RenderEngine::shutdown()
{

}


void idk::gfx::RenderEngine::drawRect(const glm::vec2 &xy, const glm::vec2 &wh)
{
    (void)xy;
    (void)wh;
}


void idk::gfx::RenderEngine::drawEllipse(const glm::vec2 &xy, const glm::vec2 &wh)
{
    (void)xy;
    (void)wh;
}


void idk::gfx::RenderEngine::setRefreshRateHz(uint64_t hz)
{
    (void)hz;
}


idk::Camera &idk::gfx::RenderEngine::getCamera()
{
    return mCamera;
}


void idk::gfx::RenderEngine::swapCamera()
{

}
