#include "idk/idk_gfxapi.hpp"


void idk::GfxApi::debugOutputEnable()
{
    cmd_ = {
        .type = cmdtype::DebugOutputEnable,
        .as_debugOutputEnable = true
    };
    _push();
}

void idk::GfxApi::debugOutputDisable()
{
    cmd_ = {
        .type = cmdtype::DebugOutputEnable,
        .as_debugOutputEnable = false
    };
    _push();
}

void idk::GfxApi::bgColorAdd(const glm::vec4 &v)
{
    cmd_ = {
        .type = cmdtype::BgColorAdd,
        .as_rgba = v
    };
    _push();
}

void idk::GfxApi::bgColorSet(const glm::vec4 &v)
{
    cmd_ = {
        .type = cmdtype::BgColorSet,
        .as_rgba = v
    };
    _push();
}
