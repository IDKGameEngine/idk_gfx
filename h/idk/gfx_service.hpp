#pragma once

#include "idk/core/service.hpp"
#include "idk/gfx/fwd.hpp"

namespace idk
{
    class GfxService;
}


class idk::GfxService: public idk::core::Service
{
private:
    idk::gfx::WindowSDL3   *win_;
    idk::gfx::RenderEngine *ren_;

public:
    GfxService(const idk::core::WindowDesc&);
    virtual void _startup(idk::IEngine*) final;
    virtual void _update(idk::IEngine*) final;
    virtual void _shutdown(idk::IEngine*) final;

    idk::gfx::RenderEngine &getRenderer() { return *ren_; }

};
