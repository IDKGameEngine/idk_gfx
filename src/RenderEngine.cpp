#include "idk/gfx/RenderEngine.hpp"
#include "vk/GraphicsDevice.hpp"
#include "libidk/New.hpp"


idk::gfx::RenderEngine::RenderEngine()
:   mGfxDevice(idk::New<GraphicsDevice>())
{

}

idk::gfx::RenderEngine::~RenderEngine()
{

}

void idk::gfx::RenderEngine::update()
{

}
