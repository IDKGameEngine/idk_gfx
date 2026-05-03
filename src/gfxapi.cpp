#include "idk/gfxapi.hpp"
#include "idk/gfx/renderer.hpp"


idk::GfxApi::GfxApi(idk::gfx::RenderEngine *ren)
:   ren_(ren),
    writer_(ren->getGfxRequestWriter())
{

}


void idk::GfxApi::FlushCommandQueue()
{
    std::atomic_bool &alive = ren_->port_.alive;
    std::atomic_bool &flush = ren_->port_.flush;

    while (true)
    {
        if (alive.load() == false)
            return;
        if (flush.load() == false)
            break;
    }

    flush.store(true);

    while (true)
    {
        if (alive.load() == false)
            return;
        if (flush.load() == false)
            break;
    }
}