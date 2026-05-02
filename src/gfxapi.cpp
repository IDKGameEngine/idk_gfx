#include "idk/gfxapi.hpp"
#include "idk/gfx/renderer.hpp"

using namespace idk;
using enum gfx::GfxReqType;


idk::GfxApi::GfxApi(idk::gfx::RenderEngine *ren)
:   writer_(ren->getGfxRequestWriter())
{

}


void GfxApi::debugOutputEnable(gfx::GfxResponse *res, bool value)
{
    req_ = {
        .type = cmdtype::DebugOutputEnable,
        .res  = res,
        .as_DebugOutputEnable = {
            .enabled = value
        }
    };
    _push();
}


void GfxApi::addComputeProgram(gfx::GfxResponse *res, const char *compute_filepath)
{
    req_ = {
        .type = cmdtype::AddComputeProgram,
        .res  = res,
        .as_AddComputeProgram = {
            .comp_path = compute_filepath
        },
    }; _push();
}

void GfxApi::addRenderProgram(gfx::GfxResponse *res, const char *vert_filepath, const char *frag_filepath)
{
    req_ = {
        .type = cmdtype::AddRenderProgram,
        .res  = res,
        .as_AddRenderProgram = {
            .vert_path = vert_filepath,
            .frag_path = frag_filepath
        },
    }; _push();
}

// uint64_t GfxApi::addRenderProgram(std::unique_ptr<gfx::RenderProgram> prog)
// {
//     uint64_t key = renderPrograms_.size();
//     renderPrograms_[key] = std::move(prog);
// }

// idk::gfx::ComputeProgram *GfxApi::getComputeProgram(uint64_t key)
// {
//     if (computePrograms_.contains(key))
//         return computePrograms_[key].get();
//     return nullptr;
// }

// idk::gfx::RenderProgram *GfxApi::getRenderProgram(uint64_t key)
// {
//     if (renderPrograms_.contains(key))
//         return renderPrograms_[key].get();
//     return nullptr;
// }




void GfxApi::bgColorAdd(gfx::GfxResponse *res, const glm::vec4 &v)
{
    req_ = {
        .type = cmdtype::BgColorAdd,
        .res  = res,
        .as_BgColorAdd = { v }
    };
    _push();
}

void GfxApi::bgColorSet(gfx::GfxResponse *res, const glm::vec4 &v)
{
    req_ = {
        .type = cmdtype::BgColorSet,
        .res  = res,
        .as_BgColorSet = { v }
    };
    _push();
}



