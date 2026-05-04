#include "idk/gfx/renderer.hpp"
#include "idk/gfx/camera.hpp"
#include "idk/gfx/framebuffer.hpp"
#include "idk/gfx/texture.hpp"
#include "idk/gfx/window.hpp"
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


static void *initAutomataTexture()
{
    static uint8_t *base = nullptr;
    if (base == nullptr)
    {
        base = new uint8_t[4*slang::AUTOMATA_WIDTH*slang::AUTOMATA_WIDTH];

        for (size_t i=0; i<4*slang::AUTOMATA_WIDTH*slang::AUTOMATA_WIDTH; i+=4)
        {
            if (rand()%100 >= 25)
            {
                base[i+0] = rand() % 255;
                base[i+1] = rand() % 255;
                base[i+2] = rand() % 255;
                base[i+3] = rand() % 255;
            }
        }
    }
    return base;
}


static idk::gfx::MeshDescriptor mesh_desc;

RenderEngine::RenderEngine(idk::gfx::WindowSDL3 &win)
:   win_(win),
    cam_(90.0f, win.mAspect, 0.01f, 1000.0f),
    raii_(gfxDebugOutputEnable, true),
    cmd_read_(cmd_queue_),
    uboWindow_(),
    uboCamera_(),
    ssboNBody0(),
    ssboNBody1(),
    ssboNBodyIn(&ssboNBody0),
    ssboNBodyOut(&ssboNBody1),
    automataFmt(GL_RGBA8, GL_RGBA, GL_UNSIGNED_BYTE, GL_LINEAR, GL_LINEAR),
    automataTexA(slang::AUTOMATA_WIDTH, slang::AUTOMATA_WIDTH, initAutomataTexture(), automataFmt),
    automataTexB(slang::AUTOMATA_WIDTH, slang::AUTOMATA_WIDTH, initAutomataTexture(), automataFmt),
    automataProg("asset/shader/automata.comp"),
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

    automataTexPtr[0] = &automataTexA;
    automataTexPtr[1] = &automataTexB;
    VLOG_INFO("gfx::Renderer Initialized");

    GLint mGlVersionMajor, mGlVersionMinor;
    gl::GetIntegerv(GL_MAJOR_VERSION, &mGlVersionMajor);
    gl::GetIntegerv(GL_MINOR_VERSION, &mGlVersionMinor);
    VLOG_INFO("Context supports OpenGL {}.{}", mGlVersionMajor, mGlVersionMinor);

    gl::CreateVertexArrays(1, &mDummyVao);
    gl::Enable(GL_MULTISAMPLE);


    glm::vec3 centerPos  = glm::vec3(0.0f);
    float     centerMass = 512.0f; // slang::NBodyVertex::MAX_MASS_HI;

    ssboNBody0.get()[0].pos = glm::vec4(centerPos, centerMass);
    ssboNBody0.get()[0].vel = glm::vec4(0.0f);
    ssboNBody0.get()[0].acc = glm::vec4(0.0f);

    for (uint32_t i=1; i<slang::NBodyVertex::MAX_BODIES; i++)
    {
        auto &vert = ssboNBody0.get()[i];

        float radius = 128.0f;
        glm::vec3 pos = radius * glm::vec3(idk::randf(-1, +1), idk::randf(-1, +1), idk::randf(-1, +1));
                  pos = glm::clamp(pos, -radius, +radius);
        // while (glm::dot(pos, pos) > radius)
        // {
        //     pos = radius * glm::vec3(idk::randf(-1, +1), idk::randf(-1, +1), idk::randf(-1, +1));
        // }

        vert.pos = glm::vec4(pos, idk::randf(+1.0f, slang::NBodyVertex::MAX_MASS_LO));
    
        // glm::vec3 disp  = centerPos - glm::vec3(vert.pos);
        // glm::vec3 dir   = glm::normalize(disp);
        // float     dist  = glm::length(disp);

        // // glm::vec3 noise = 0.1f * glm::vec3(0.0f, idk::randf(-1, +1), 0.0f);
        // glm::vec3 odir  = glm::normalize(glm::cross(dir, coordinate_system::UP));
        // float     omag  = (0.5f * centerMass) / (dist*dist*dist);

        // vert.vel = glm::vec4(omag*odir, 0.0f);
        // vert.vel = glm::vec4(0.0f);
        idk::randvec4(-32.0f, +32.0f, &(vert.vel[0]));
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

            case GfxReqType::BgColorSet:
                uboWindow_->gamepos = req.as_BgColorSet.value;
                break;

            case GfxReqType::BgColorAdd:
                uboWindow_->gamepos += req.as_BgColorAdd.value;
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

    uboWindow_->winsz = glm::vec4(win_.mSizef, 0.0f, 0.0f);
    // if (ubo3_->mDirty)
    {
        uboWindow_.sendToGpu();
    }

    {
        auto getter = getCameraLock();
        auto &cam = getter();

        if (cam.isDirty())
        {
            uboCamera_->V = cam.getView();
            uboCamera_->P = cam.getProj();
            uboCamera_->PV = cam.getProjView();
            uboCamera_.sendToGpu();

            cam.unDirty();
        }
    }

    gl::UseProgram(automataProg.mId);
    automataTexPtr[0]->bindImageTexture(8);
    automataTexPtr[1]->bindImageTexture(9);
    gl::DispatchCompute(slang::AUTOMATA_WIDTH/8, slang::AUTOMATA_WIDTH/8, 1);
    gl::MemoryBarrier(GL_ALL_BARRIER_BITS);
    std::swap(automataTexPtr[0], automataTexPtr[1]);


    gl::UseProgram(nbodyPositionProg.mId);
    ssboNBodyIn->bind(slang::BIND_NBODY_IN);
    ssboNBodyOut->bind(slang::BIND_NBODY_OUT);
    gl::DispatchCompute(slang::NBodyVertex::MAX_BODIES / slang::NBodyVertex::GROUP_SIZE, 1, 1);
    gl::MemoryBarrier(GL_ALL_BARRIER_BITS);
    std::swap(ssboNBodyIn, ssboNBodyOut);


    static uint64_t count = 0;
    count += 1;

    if (count < 99)
        gl::UseProgram(nbodyExpansionProg.mId);
    else
        gl::UseProgram(nbodyGravityProg.mId);

    ssboNBodyIn->bind(slang::BIND_NBODY_IN);
    ssboNBodyOut->bind(slang::BIND_NBODY_OUT);
    gl::DispatchCompute(slang::NBodyVertex::MAX_BODIES / slang::NBodyVertex::GROUP_SIZE, 1, 1);
    gl::MemoryBarrier(GL_ALL_BARRIER_BITS);


    gl::UseProgram(nbodyRenderProg.mId);
    ssboNBodyIn->bind(slang::BIND_NBODY_IN);
    ssboNBodyOut->bind(slang::BIND_NBODY_OUT);
    uboCamera_.bind(uboCamera_->BIND_IDX);
    gl::Enable(GL_PROGRAM_POINT_SIZE);
    gl::Enable(GL_BLEND);
    gl::BlendFunc(GL_SRC_ALPHA, GL_ONE);

    gl::BindVertexArray(mDummyVao);
    gl::DrawArrays(GL_POINTS, 0, slang::NBodyVertex::MAX_BODIES);

    std::swap(ssboNBodyIn, ssboNBodyOut);

    // gl::UseProgram(winProg.mId);
    // automataTexPtr[0]->bindImageTexture(8);
    // uboWindow_.bind(uboWindow_->BIND_IDX);
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

idk::ThreadSafeAccess<idk::gfx::Camera> RenderEngine::getCameraLock()
{
    return idk::ThreadSafeAccess<gfx::Camera>(cam_);
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


