#include "idk/gfxapi.hpp"
#include "idk/gfx/renderer.hpp"


idk::GfxApi::GfxApi(idk::gfx::RenderEngine *ren)
:   writer_(ren->getGfxRequestWriter())
{

}
