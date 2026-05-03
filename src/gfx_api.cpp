#include "idk/gfx_api.hpp"
#include "idk/gfx_service.hpp"
#include "idk/gfx/renderer.hpp"


idk::GfxApi::GfxApi(idk::GfxService *srv)
:   srv_(srv),
    writer_(srv->getRenderer().getGfxRequestWriter())
{

}


void idk::GfxApi::FlushCommandQueue()
{
    auto &ren = srv_->getRenderer();
    std::atomic_bool &alive = ren.alive_;
    std::atomic_bool &flush = ren.flush_;

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