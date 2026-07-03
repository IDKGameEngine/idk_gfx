#pragma once

#include <glm/glm.hpp>
#include <SDL3/SDL.h>

#include "idk/gfx/fwd.hpp"
#include "idk/gfx/buffer.hpp"
#include "idk/gfx/shader.hpp"
#include "idk/gfx/texture.hpp"
#include "idk/gfx/mesh.hpp"
#include "idk/gfx/render_interface.hpp"

#include "idk/slang.hpp"

#include "idk/core/camera.hpp"
#include "idk/core/double_buffer.hpp"
#include "idk/core/raii.hpp"
#include "idk/core/periodic_timer.hpp"

#include <map>


namespace idk::gfx
{
    class RenderEngine;
}


class idk::gfx::RenderEngine
{
public:
    std::atomic_bool alive_;

    RenderEngine(idk::gfx::WindowSDL3&);
    ~RenderEngine();
    void update(idk::IEngine*);
    void shutdown();

    void setRefreshRateHz(uint64_t hz);
    std::mutex &getMutex();
    idk::Camera &getCamera();
    void setLerpAlpha(float alpha) { lerpAlpha_ = alpha; }
    void swapCamera();

    static void debugOutputEnable(const DebugOutputEnableRequest&, DebugOutputEnableResponse*);
    void addComputeProgram(const AddComputeProgramRequest&, AddComputeProgramResponse*);
    void addRenderProgram(const AddRenderProgramRequest&, AddRenderProgramResponse*);

private:
    idk::PeriodicTimer          timer_;
    std::mutex                  mutex_;
    idk::gfx::WindowSDL3        &win_;
    idk::Camera                 camPrev_;
    idk::Camera                 camCurr_;
    idk::Camera                 camNext_;
    RaiiFunc<void(bool)>        raii_;

    UboWriter<slang::PerFrameData>  perFrame_;
    UboWriter<slang::PerCameraData> perCamera_;

    SsboWriter<slang::NBodyVertex[slang::NBodyVertex::MAX_BODIES]> ssboNBody0, ssboNBody1;
    SsboWriter<slang::NBodyVertex[slang::NBodyVertex::MAX_BODIES]> *ssboNBodyIn, *ssboNBodyOut;

    gfx::ComputeProgram clearProg;
    gfx::RenderProgram  winProg;
    gfx::ComputeProgram nbodyPositionProg;
    gfx::ComputeProgram nbodyExpansionProg;
    gfx::ComputeProgram nbodyGravityProg;
    gfx::RenderProgram  nbodyRenderProg;

    GLuint nbodyVao_;
    GLuint mDummyVao;

    gfx::MeshBuffer meshbuf_;

    std::vector<gfx::ComputeProgram> computePrograms_;
    std::vector<gfx::RenderProgram> renderPrograms_;

    float lerpAlpha_ = 0.0f;

    void _update_image();
};

