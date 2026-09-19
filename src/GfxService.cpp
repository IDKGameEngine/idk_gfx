#include "idk/gfx/GfxService.hpp"
#include "vk/GraphicsDevice.hpp"
#include "libidk/New.hpp"


idk::gfx::GfxService::GfxService(idk::EngineContext &ctx)
:   EngineComponent(ctx),
    mGfxDevice(idk::New<GraphicsDevice>())
{

}

idk::gfx::GfxService::~GfxService()
{

}

void idk::gfx::GfxService::update()
{

}
