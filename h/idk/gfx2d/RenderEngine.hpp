#pragma once

#include <glm/glm.hpp>

#include "libidk/Types.hpp"
#include "libidk/Engine.hpp"
#include "libidk/camera.hpp"
#include "libidk/Service.hpp"


namespace idk::gfx
{
    class RenderEngine;
}


class idk::gfx::RenderEngine: public idk::Immobile
{
public:
    RenderEngine(idk::core::IPlatformService*);
    ~RenderEngine();

    void update(idk::IEngine*);
    void shutdown();
    void drawRect(const glm::vec2 &xy, const glm::vec2 &wh);
    void drawEllipse(const glm::vec2 &xy, const glm::vec2 &wh);

    void setRefreshRateHz(uint64_t);

    idk::Camera &getCamera();
    void swapCamera();

private:
    idk::Camera mCamera;

};
