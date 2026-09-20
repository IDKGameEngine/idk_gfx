#include "idk/gfx/GfxService.hpp"
#include "vk/GraphicsDevice.hpp"
#include "libidk/New.hpp"


idk::gfx::GfxService::GfxService(idk::EngineContext &ctx)
:   EngineService(ctx),
    mGfxDevice(idk::New<GraphicsDevice>())
{

}

idk::gfx::GfxService::~GfxService()
{

}

void idk::gfx::GfxService::init(idk::Engine &E)
{
    (void)E;
}

void idk::gfx::GfxService::update(idk::Engine &E)
{
    (void)E;
}
