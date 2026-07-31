#include "idk/gfx2d/RenderEngine.hpp"


idk::gfx2d::RenderEngine::RenderEngine(idk::core::IPlatformService *plat)
{
    int winWidth, winHeight;
    plat->getWindowSize(winWidth, winHeight);

    mCamera = idk::Camera(float(winWidth)/winHeight, 80.0f, 0.1f, 8000.0f),
    mCamera.getTransform().SetPosition(glm::vec3(0.0f, 16.0f, 32.0f));
}


idk::gfx2d::RenderEngine::~RenderEngine()
{

}


void idk::gfx2d::RenderEngine::update()
{

}


void idk::gfx2d::RenderEngine::shutdown()
{

}


void idk::gfx2d::RenderEngine::drawRect(const glm::vec2 &xy, const glm::vec2 &wh)
{
    (void)xy;
    (void)wh;
}


void idk::gfx2d::RenderEngine::drawEllipse(const glm::vec2 &xy, const glm::vec2 &wh)
{
    (void)xy;
    (void)wh;
}


