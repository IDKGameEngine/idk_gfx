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

static void image_load_test()
{
    SDL_Surface *img = IMG_Load("asset/noise/perlin.jpg");
    VLOG_INFO("perlin.jpg: {}x{}", img->w, img->h);
}


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
    nbodyComputeProg("asset/shader/nbody.comp"),
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

    for (uint32_t i=0; i<slang::NBodyVertex::MAX_BODIES; i++)
    {
        auto &vert = ssboNBody0.get()[i];

        vert.pos = glm::vec4(
            idk::randf(-1.0f, +1.0f) * 4.0f,
            idk::randf(-1.0f, +1.0f) * 4.0f,
            idk::randf(-1.0f, +1.0f) * 4.0f,
            idk::randf(+1.0f, +32.0f)
        );

        vert.vel = glm::vec4(
            idk::randf(-1.0f, +1.0f) * 0.5f,
            idk::randf(-1.0f, +1.0f) * 0.5f,
            idk::randf(-1.0f, +1.0f) * 0.5f,
            0.0f
        );
    }
    ssboNBody0.sendToGpu();

    image_load_test();

    // gl::CreateVertexArrays(1, &nbodyVao_);
    gl::CreateVertexArrays(1, &mDummyVao);

    // gl::UseProgram(clearProg.mId);
    // automataTexPtr[0]->bindImageTexture(8);
    // automataTexPtr[1]->bindImageTexture(9);
    // gl::DispatchCompute(slang::AUTOMATA_WIDTH/8, slang::AUTOMATA_WIDTH/8, 1);
    // gl::MemoryBarrier(GL_ALL_BARRIER_BITS);
    // std::swap(automataTexPtr[0], automataTexPtr[1]);

}

RenderEngine::~RenderEngine()
{
    gl::DeleteVertexArrays(1, &mDummyVao);
}

#include "idk/gfx/controller.hpp"

void RenderEngine::update(idk::IEngine *E)
{
    SDL_Event e;
    while (SDL_PollEvent(&e))
    {
        if (e.type == SDL_EVENT_QUIT)
        {
            VLOG_INFO("SDL_EVENT_QUIT");
            E->shutdown();
        }
    }


    static idk::TestCharacterController ctl;
    ctl.update();

    glm::vec3 move, look;
    ctl.getMotion(move, look);

    auto &T = cam_.getTransform();
    T.translate(move.x * T.getRight());
    T.translate(move.y * T.getUp());
    T.translate(move.z * T.getFront());
    T.rotate(look.x, T.getUp()); 
    T.rotate(look.y, T.getRight()); 

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

            case GfxReqType::GetCamera:
                req.as_GetCamera.dst = cam_;
                break;

            case GfxReqType::SetCamera:
                cam_ = req.as_SetCamera.src;
                // {
                //     auto p = cam_.getTransform().getPos();
                //     VLOG_INFO("pos: {}, {}, {}", p.x, p.y, p.z);
                // }
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

    if (cam_.isDirty())
    {
        // auto p = cam_.getTransform().to_mat4() * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
        // VLOG_INFO("cam_.isDirty() == true, pos == {}, {}, {}", p.x, p.y, p.z);

        uboCamera_->V = cam_.getView();
        uboCamera_->P = cam_.getProj();
        uboCamera_->PV = cam_.getProjView();
        uboCamera_.sendToGpu();

        cam_.unDirty();
    }

    gl::UseProgram(automataProg.mId);
    automataTexPtr[0]->bindImageTexture(8);
    automataTexPtr[1]->bindImageTexture(9);
    gl::DispatchCompute(slang::AUTOMATA_WIDTH/8, slang::AUTOMATA_WIDTH/8, 1);
    gl::MemoryBarrier(GL_ALL_BARRIER_BITS);
    std::swap(automataTexPtr[0], automataTexPtr[1]);


    gl::UseProgram(nbodyComputeProg.mId);
    ssboNBodyIn->bind(slang::BIND_NBODY_IN);
    ssboNBodyOut->bind(slang::BIND_NBODY_OUT);
    gl::DispatchCompute(slang::NBodyVertex::MAX_BODIES / slang::NBodyVertex::GROUP_SIZE, 1, 1);
    gl::MemoryBarrier(GL_ALL_BARRIER_BITS);


    gl::UseProgram(nbodyRenderProg.mId);
    ssboNBodyOut->bind(slang::BIND_NBODY_IN);
    uboCamera_.bind(uboCamera_->BIND_IDX);

    gl::Enable(GL_PROGRAM_POINT_SIZE);
    gl::Enable(GL_BLEND); // Additive blending looks great for particles
    gl::BlendFunc(GL_SRC_ALPHA, GL_ONE); // Additive

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


