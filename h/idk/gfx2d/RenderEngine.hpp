#pragma once

#include <glm/glm.hpp>

#include "libidk/platform/IPlatform.hpp"
#include "libidk/Types.hpp"
#include "libidk/Engine.hpp"
#include "libidk/camera.hpp"
#include "libidk/Service.hpp"

namespace idk::gfx
{
    class RenderEngine: public idk::Immobile
    {
    public:
        RenderEngine(idk::Platform&);
        ~RenderEngine();

        void update(idk::IEngine*);
        void shutdown();
        void drawRect(const glm::vec2 &xy, const glm::vec2 &wh);
        void drawEllipse(const glm::vec2 &xy, const glm::vec2 &wh);

        void setRefreshRateHz(uint64_t);

        idk::Camera &getCamera();
        void swapCamera();

    private:
        idk::Platform &mPlat;
        idk::platform::Window   &mWin;
        idk::Camera mCamera;

    };
}
