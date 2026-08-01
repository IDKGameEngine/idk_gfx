#pragma once

#ifdef IDK_GFX_MODEL_2D
    #include "idk/gfx2d/RenderEngine.hpp"
#elif defined(IDK_GFX_MODEL_3D)
    #include "idk/gfx3d/RenderEngine.hpp"
#else
    #error Must define either IDK_GFX_MODEL_2D or IDK_GFX_MODEL_3D
#endif
