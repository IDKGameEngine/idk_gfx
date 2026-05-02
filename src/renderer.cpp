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
:   win_(new WindowSDL3(windesc)),
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


void RenderEngine::onUpdate(idk::IEngine *engine)
{
    SDL_Event e;
    while (SDL_PollEvent(&e))
    {
        if (e.type == SDL_EVENT_QUIT)
        {
            VLOG_INFO("SDL_EVENT_QUIT");
            engine->shutdown();
        }
    }

    while (!gfxread_->empty())
    {
        auto &req = gfxread_->front();
        auto *res = req.res;

        switch (req.type)
        {
            case GfxReqType::DebugOutputEnable:
                debugOutputEnable(req.as_DebugOutputEnable, &res->as_DebugOutputEnable);
                break;
            case GfxReqType::AddComputeProgram:
                addComputeProgram(req.as_AddComputeProgram, &res->as_AddComputeProgram);
                break;
            case GfxReqType::AddRenderProgram:
                addRenderProgram(req.as_AddRenderProgram, &res->as_AddRenderProgram);
                break;
            case GfxReqType::BgColorSet:
                uboWt3->gamepos = req.as_BgColorSet.value;
                break;
            case GfxReqType::BgColorAdd:
                uboWt3->gamepos += req.as_BgColorAdd.value;
                break;
            case GfxReqType::FgColorSet:
                uboWt3->fgtint = req.as_FgColorSet.value;
                break;
            case GfxReqType::FgColorAdd:
                uboWt3->fgtint += req.as_FgColorAdd.value;
                break;
            default:
                VLOG_FATAL("gfx::GfxReqType is Invalid!");
                break;
        }

        res->ready = true;
        gfxread_->pop();
    }

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

    gfxreqs_.swapBuffers();
}


void RenderEngine::onShutdown(idk::IEngine*)
{
    VLOG_INFO("RenderEngine::onShutdown");
}

idk::core::DblBufferWriter<GfxRequest> RenderEngine::getGfxRequestWriter()
{
    return idk::core::DblBufferWriter<GfxRequest>(gfxreqs_);
}


void RenderEngine::addComputeProgram(const AddComputeProgramRequest &req, AddComputeProgramResponse *res)
{
    uint64_t key = computePrograms_.size();
    computePrograms_.emplace_back(gfx::ComputeProgram(req.comp_path));
    res->id = static_cast<int64_t>(key);
}

void RenderEngine::addRenderProgram(const AddRenderProgramRequest &req, AddRenderProgramResponse *res)
{
    uint64_t key = renderPrograms_.size();
    renderPrograms_.emplace_back(gfx::RenderProgram(req.vert_path, req.frag_path));
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


