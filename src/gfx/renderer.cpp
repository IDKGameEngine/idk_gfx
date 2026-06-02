#include "idk/gfx/renderer.hpp"
#include "idk/gfx/framebuffer.hpp"
#include "idk/gfx/texture.hpp"
#include "idk/gfx/window.hpp"

#include "idk/core/basis.hpp"
#include "idk/core/camera.hpp"
#include "idk/core/file.hpp"
#include "idk/core/metric.hpp"
#include "idk/core/random.hpp"

#include <SDL3_image/SDL_image.h>

using namespace idk::gfx;

extern void gfxDebugOutputEnable(bool);

// static void image_load_test()
// {
//     SDL_Surface *img = IMG_Load("asset/noise/perlin.jpg");
//     VLOG_INFO("perlin.jpg: {}x{}", img->w, img->h);
// }


static glm::vec3 rand_vec3(float lo, float hi)
{
    return glm::vec3(idk::randf(lo, hi), idk::randf(lo, hi), idk::randf(lo, hi));
}

static glm::vec3 rand_vec3(float m)
{
    return rand_vec3(-m, +m);
}



RenderEngine::RenderEngine(idk::gfx::WindowSDL3 &win)
:   win_(win),
    cam_(win.mAspect, 80.0f, 0.1f, 8000.0f),
    raii_(gfxDebugOutputEnable, true),
    cmd_read_(cmd_queue_),
    perFrame_(),
    perCamera_(),
    ssboNBody0(),
    ssboNBody1(),
    ssboNBodyIn(&ssboNBody0),
    ssboNBodyOut(&ssboNBody1),
    clearProg("asset/shader/clear.comp"),
    winProg("asset/shader/screenquad.vert", "asset/shader/screenquad.frag"),
    nbodyPositionProg("asset/shader/nbody_position.comp"),
    nbodyExpansionProg("asset/shader/nbody_expansion.comp"),
    nbodyGravityProg("asset/shader/nbody_gravity.comp"),
    nbodyRenderProg("asset/shader/nbody_render.vert", "asset/shader/nbody_render.frag"),
    meshbuf_(gfx::MeshBuffer(64*idk::MEGA, 64*idk::MEGA))
{
    alive_.store(true);
    flush_.store(false);
    cam_.getTransform().SetPosition(glm::vec3(0.0f, 16.0f, 32.0f));

    VLOG_INFO("gfx::Renderer Initialized");

    GLint mGlVersionMajor, mGlVersionMinor;
    gl::GetIntegerv(GL_MAJOR_VERSION, &mGlVersionMajor);
    gl::GetIntegerv(GL_MINOR_VERSION, &mGlVersionMinor);
    VLOG_INFO("Context supports OpenGL {}.{}", mGlVersionMajor, mGlVersionMinor);

    gl::CreateVertexArrays(1, &mDummyVao);
    gl::Enable(GL_MULTISAMPLE);

    perFrame_->prevTime  = float(OsAdapter::GetSysTimeMs()) / 1000.0;
    perFrame_->currTime  = perFrame_->currTime;
    perFrame_->timescale = 1.0f;



    static constexpr float radius = 1024.0f;

    // glm::vec3 centerPos  = glm::vec3(0.0f);
    // float     centerMass = 128.0f*1024.0f; // slang::NBodyVertex::MAX_MASS;

    // ssboNBody0.get()[0].pos = glm::vec4(centerPos, centerMass);
    // ssboNBody0.get()[0].vel = glm::vec4(0.0f);
    // ssboNBody0.get()[0].acc = glm::vec4(0.0f);

    for (uint32_t i=0; i<slang::NBodyVertex::MAX_BODIES; i++)
    {
        auto &vert = ssboNBody0.get()[i];
    
        glm::vec3 pos = rand_vec3(radius) * glm::vec3(1.0, 0.25, 1.0);
        // glm::vec3 pos_disp = (centerPos - pos);
        // if (glm::length(pos_disp) > radius)
        // {
        //     pos += rand_vec3(0.0f, 0.25f) * pos_disp;
        // }

        // glm::vec3 disp  = centerPos - glm::vec3(pos);
        // glm::vec3 dir   = glm::normalize(disp);
        // float     dist  = glm::length(disp);
        // float     omag  = sqrt(centerMass / dist);
        // glm::vec3 ovel  = omag*glm::normalize(glm::cross(dir, world_axis::UP));


        vert.pos = glm::vec4(pos, idk::randf(slang::NBodyVertex::MIN_MASS, slang::NBodyVertex::MAX_MASS));
        // vert.vel = glm::vec4(0.0f);
        vert.vel = glm::vec4(rand_vec3(16.0f), 0.0f);
        vert.acc = glm::vec4(0.0f);
    }

    ssboNBody0.sendToGpu();

    // image_load_test();
}

RenderEngine::~RenderEngine()
{
    gl::DeleteVertexArrays(1, &mDummyVao);
}


void RenderEngine::update(idk::IEngine *E)
{
    (void)E;

    SDL_Event e;
    while (SDL_PollEvent(&e))
    {
        if (e.type == SDL_EVENT_QUIT)
        {
            VLOG_INFO("SDL_EVENT_QUIT");
            E->shutdown();
        }

        if (e.type == SDL_EVENT_KEY_UP)
        {
            if (e.key.scancode == SDL_SCANCODE_ESCAPE)
            {
                SDL_SetWindowRelativeMouseMode(win_.mSdlWin, false);
            }
        }
    }

    while (!cmd_read_->empty())
    {
        auto &req = cmd_read_->front();
        auto *res = req.res;

        switch (req.type)
        {
            case GfxReqType::DebugOutputEnable:
                debugOutputEnable(req.as_DebugOutputEnable, static_cast<DebugOutputEnableResponse*>(res));
                break;

            case GfxReqType::AddComputeProgram:
                addComputeProgram(req.as_AddComputeProgram, static_cast<AddComputeProgramResponse*>(res));
                break;

            case GfxReqType::AddRenderProgram:
                addRenderProgram(req.as_AddRenderProgram, static_cast<AddRenderProgramResponse*>(res));
                break;

            case GfxReqType::GetComputeProgram:
                ((GetComputeProgramResponse*)res)->prog = &computePrograms_[req.as_GetComputeProgram.id];
                break;

            case GfxReqType::GetRenderProgram:
                ((GetRenderProgramResponse*)res)->prog = &renderPrograms_[req.as_GetRenderProgram.id];
                break;

            default:
                VLOG_FATAL("gfx::GfxReqType is Invalid!");
                break;
        }

        res->make_ready();
        cmd_read_->pop();
    }

    _update_image();

    if (flush_.load() == true)
    {
        cmd_queue_.swapBuffers();
        flush_.store(false);
    }
}

void RenderEngine::_update_image()
{
    win_.makeCurrent();
    gl::Clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    {
        perFrame_->prevTime  = perFrame_->currTime;
        perFrame_->currTime  = float(OsAdapter::GetSysTimeMs()) / 1000.0f;
        perFrame_->deltaTime = (perFrame_->currTime - perFrame_->prevTime);
        perFrame_->winSize   = glm::vec4(win_.mSizef, 0.0f, 0.0f);
        perFrame_.sendToGpu();
    }
    {
        auto getter = getCameraLock();
        auto &cam = getter();

        perCamera_->View = cam.getView();
        perCamera_->Proj = cam.getProj();
        perCamera_.sendToGpu();
    }

    gl::UseProgram(nbodyPositionProg.mId);
    ssboNBodyIn->bind(slang::BIND_NBODY_IN);
    ssboNBodyOut->bind(slang::BIND_NBODY_OUT);
    gl::DispatchCompute(slang::NBodyVertex::MAX_BODIES / slang::NBodyVertex::GROUP_SIZE, 1, 1);
    gl::MemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);

    static uint64_t count = 0;
    count += 1;

    // if (count < 25)
    //     gl::UseProgram(nbodyExpansionProg.mId);
    // else
        // gl::UseProgram(nbodyGravityProg.mId);

    gl::UseProgram(nbodyGravityProg.mId);
    ssboNBodyOut->bind(slang::BIND_NBODY_IN);
    ssboNBodyIn->bind(slang::BIND_NBODY_OUT);
    gl::DispatchCompute(slang::NBodyVertex::MAX_BODIES / slang::NBodyVertex::GROUP_SIZE, 1, 1);
    gl::MemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
    std::swap(ssboNBodyIn, ssboNBodyOut);

    gl::UseProgram(nbodyRenderProg.mId);
    ssboNBodyIn->bind(slang::BIND_NBODY_IN);
    ssboNBodyOut->bind(slang::BIND_NBODY_OUT);
    perCamera_.bind(perCamera_->BIND_IDX);
    gl::Enable(GL_PROGRAM_POINT_SIZE);
    gl::Enable(GL_BLEND);
    gl::BlendFunc(GL_ONE, GL_ONE);

    gl::BindVertexArray(mDummyVao);
    gl::DrawArrays(GL_POINTS, 0, slang::NBodyVertex::MAX_BODIES);

    std::swap(ssboNBodyIn, ssboNBodyOut);

    // gl::UseProgram(winProg.mId);
    // gl::BindVertexArray(mDummyVao);
    // gl::DrawArrays(GL_TRIANGLES, 0, 3);

    win_.swapWindow();
}


void RenderEngine::shutdown()
{
    alive_.store(false);
}

std::mutex &RenderEngine::getMutex()
{
    return mutex_;
}

idk::DblBufferWriter<GfxRequest> RenderEngine::getGfxRequestWriter()
{
    return idk::DblBufferWriter<GfxRequest>(cmd_queue_);
}

idk::ThreadSafeAccess<idk::Camera> RenderEngine::getCameraLock()
{
    return idk::ThreadSafeAccess<idk::Camera>(cam_);
}

void RenderEngine::addComputeProgram(const AddComputeProgramRequest &req, AddComputeProgramResponse *res)
{
    uint64_t key = computePrograms_.size();
    computePrograms_.emplace_back(req.comp_path);
    res->id = static_cast<int64_t>(key);
}

void RenderEngine::addRenderProgram(const AddRenderProgramRequest &req, AddRenderProgramResponse *res)
{
    uint64_t key = renderPrograms_.size();
    renderPrograms_.emplace_back(req.vert_path, req.frag_path);
    res->id = static_cast<int64_t>(key);
}

// idk::gfx::ComputeProgram *RenderEngine::getComputeProgram(uint64_t key)
// {
//     if (computePrograms_.contains(key))
//         return computePrograms_[key].get();
//     return nullptr;
// }

// idk::gfx::RenderProgram *RenderEngine::getRenderProgram(uint64_t key)
// {
//     if (renderPrograms_.contains(key))
//         return renderPrograms_[key].get();
//     return nullptr;
// }


