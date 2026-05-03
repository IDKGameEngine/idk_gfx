#include "idk/gfx/renderer.hpp"
#include "idk/gfx/framebuffer.hpp"
#include "idk/gfx/texture.hpp"
#include "idk/gfx/window.hpp"
#include "idk/core/file.hpp"
#include "idk/core/metric.hpp"

#include <SDL3_image/SDL_image.h>

extern void gfxDebugOutputEnable(bool);

using namespace idk::gfx;

#include <SDL3_image/SDL_image.h>
static void image_load_test()
{
    SDL_Surface *img = IMG_Load("asset/noise/perlin.jpg");
    VLOG_INFO("perlin.jpg: {}x{}", img->w, img->h);
}


RenderEngine::RenderEngine(const idk::core::WindowDesc &windesc)
:   Service(idk::PeriodicTimer(1000.0 / 60.0)),
    win_(new WindowSDL3(windesc)),
    raii_(gfxDebugOutputEnable, true),
    gfxread_(gfxreqs_),
    uboWt3(),
    automataFmt(GL_R8, GL_RGBA, GL_UNSIGNED_BYTE),
    automataTexA(slang::AUTOMATA_WIDTH, slang::AUTOMATA_WIDTH, nullptr, automataFmt),
    automataTexB(slang::AUTOMATA_WIDTH, slang::AUTOMATA_WIDTH, nullptr, automataFmt),
    automataProg("asset/shader/automata.comp"),
    clearProg("asset/shader/clear.comp"),
    winProg("asset/shader/screenquad.vert", "asset/shader/screenquad.frag")
{
    port_.alive.store(true);
    port_.flush.store(false);

    automataTexPtr[0] = &automataTexA;
    automataTexPtr[1] = &automataTexB;
    VLOG_INFO("gfx::Renderer Initialized");

    GLint mGlVersionMajor, mGlVersionMinor;
    gl::GetIntegerv(GL_MAJOR_VERSION, &mGlVersionMajor);
    gl::GetIntegerv(GL_MINOR_VERSION, &mGlVersionMinor);
    VLOG_INFO("Context supports OpenGL {}.{}", mGlVersionMajor, mGlVersionMinor);

    image_load_test();

    gl::CreateVertexArrays(1, &mDummyVao);
    meshbuf_ = new gfx::MeshBuffer(64*idk::MEGA, 64*idk::MEGA);

    gl::UseProgram(clearProg.mId);
    automataTexPtr[0]->bindImageTexture(8);
    automataTexPtr[1]->bindImageTexture(9);
    gl::DispatchCompute(slang::AUTOMATA_WIDTH/8, slang::AUTOMATA_WIDTH/8, 1);
    gl::MemoryBarrier(GL_ALL_BARRIER_BITS);
    std::swap(automataTexPtr[0], automataTexPtr[1]);

}

RenderEngine::~RenderEngine()
{
    gl::DeleteVertexArrays(1, &mDummyVao);
}


void RenderEngine::_startup(idk::IEngine*)
{
    VLOG_INFO("[idk::RenderEngine::_startup]");
}


void RenderEngine::_update(idk::IEngine *E)
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

    while (!gfxread_->empty())
    {
        GfxRequest req = gfxread_->front();
                         gfxread_->pop();
        GfxResponse *res = req.res;

        switch (req.type)
        {
            case GfxReqType::DebugOutputEnable:
                VLOG_INFO("[RenderEngine::_update] DebugOutputEnable");
                debugOutputEnable(req.as_DebugOutputEnable, static_cast<DebugOutputEnableResponse*>(res));
                break;

            case GfxReqType::AddComputeProgram:
                VLOG_INFO("[RenderEngine::_update] AddComputeProgram");
                addComputeProgram(req.as_AddComputeProgram, static_cast<AddComputeProgramResponse*>(res));
                break;

            case GfxReqType::AddRenderProgram:
                VLOG_INFO("[RenderEngine::_update] AddRenderProgram");
                addRenderProgram(req.as_AddRenderProgram, static_cast<AddRenderProgramResponse*>(res));
                break;

            case GfxReqType::GetComputeProgram:
                VLOG_INFO("[RenderEngine::_update] GetComputeProgram");
                ((GetComputeProgramResponse*)res)->prog = &computePrograms_[req.as_GetComputeProgram.id];
                break;

            case GfxReqType::GetRenderProgram:
                VLOG_INFO("[RenderEngine::_update] GetRenderProgram");
                ((GetRenderProgramResponse*)res)->prog = &renderPrograms_[req.as_GetRenderProgram.id];
                break;

            case GfxReqType::BgColorSet:
                VLOG_INFO("[RenderEngine::_update] BgColorSet");
                uboWt3->gamepos = req.as_BgColorSet.value;
                break;

            case GfxReqType::BgColorAdd:
                VLOG_INFO("[RenderEngine::_update] BgColorAdd");
                uboWt3->gamepos += req.as_BgColorAdd.value;
                break;

            default:
                VLOG_FATAL("gfx::GfxReqType is Invalid!");
                break;
        }

        res->make_ready();
    }

    if (port_.flush.load() == true)
    {
        _flush();
        gfxreqs_.swapBuffers();
        port_.flush.store(false);
    }
}

void RenderEngine::_flush()
{
    win_->makeCurrent();
    uboWt3->winsz = glm::vec4(win_->mSizef, 0.0f, 0.0f);
    // if (ubo3_->mDirty)
    {
        uboWt3.sendToGpu();
    }

    gl::UseProgram(automataProg.mId);
    automataTexPtr[0]->bindImageTexture(8);
    automataTexPtr[1]->bindImageTexture(9);
    gl::DispatchCompute(slang::AUTOMATA_WIDTH/8, slang::AUTOMATA_WIDTH/8, 1);
    gl::MemoryBarrier(GL_ALL_BARRIER_BITS);
    std::swap(automataTexPtr[0], automataTexPtr[1]);

    gl::UseProgram(winProg.mId);
    automataTexPtr[0]->bindImageTexture(8);
    uboWt3.bind();
    gl::BindVertexArray(mDummyVao);
    gl::DrawArrays(GL_TRIANGLES, 0, 3);

    win_->swapWindow();
}


void RenderEngine::_shutdown(idk::IEngine*)
{
    VLOG_INFO("RenderEngine::_shutdown");
    port_.alive.store(false);
}

std::mutex &RenderEngine::getMutex()
{
    return mutex_;
}

idk::DblBufferWriter<GfxRequest> RenderEngine::getGfxRequestWriter()
{
    return idk::DblBufferWriter<GfxRequest>(gfxreqs_);
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


