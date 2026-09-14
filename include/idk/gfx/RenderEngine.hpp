#pragma once

#if defined(IDK_GFX_MODEL_2D)
    #include "idk/gfx2d/RenderEngine.hpp"
#elif defined(IDK_GFX_MODEL_3D)
    #include "idk/gfx3d/RenderEngine.hpp"
#else
    #include "idk/gfx3d/RenderEngine.hpp"
#endif
