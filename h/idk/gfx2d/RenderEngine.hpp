#pragma once

#include "libidk/Types.hpp"
#include <glm/glm.hpp>
#include "libidk/camera.hpp"
#include "libidk/Service.hpp"

namespace idk::gfx2d
{
    class RenderEngine;
}


class idk::gfx2d::RenderEngine: public idk::Immobile
{
public:
    RenderEngine(idk::core::IPlatformService*);
    ~RenderEngine();

    void update();
    void shutdown();

    void drawRect(const glm::vec2 &xy, const glm::vec2 &wh);
    void drawEllipse(const glm::vec2 &xy, const glm::vec2 &wh);

    idk::Camera &getCamera();

private:
    idk::Camera mCamera;

};
