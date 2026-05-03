#pragma once

#include "idk/core/service.hpp"

namespace idk::gfx
{
    class GfxService: public idk::core::Service
    {
    private:
    public:
        GfxService(): Service(0.0) {  }
        virtual void _update(idk::IEngine*) final;
        virtual void _shutdown(idk::IEngine*) final;

    };

}
