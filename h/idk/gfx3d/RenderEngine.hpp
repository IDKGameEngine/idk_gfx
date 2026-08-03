#pragma once

#include <glm/glm.hpp>
#include <SDL3/SDL.h>

#include "idk/gfx3d/fwd.hpp"
#include "idk/gfx3d/buffer.hpp"
#include "idk/gfx3d/shader.hpp"
#include "idk/gfx3d/texture.hpp"
#include "idk/gfx3d/mesh.hpp"
#include "idk/gfx3d/render_interface.hpp"

#include "idk/slang.hpp"

#include "libidk/camera.hpp"
#include "libidk/double_buffer.hpp"
#include "libidk/raii.hpp"
#include "libidk/Timer.hpp"
#include "libidk/Service.hpp"

#include <map>


namespace idk::gfx
{
    class RenderEngine;
}


class idk::gfx::RenderEngine
{
public:
    std::atomic_bool alive_;

    RenderEngine(idk::Platform&);
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
    Platform         &mPlat;
    platform::Window           &mWin;
    idk::Camera                 camPrev_, camCurr_, camNext_;
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
};
