#pragma once

#ifdef IDK_GFX_MODEL_2D
    #include "idk/gfx2d/Platform.hpp"
#elif defined(IDK_GFX_MODEL_3D)
    #include "idk/gfx3d/Platform.hpp"
#else
    #error Must define either IDK_GFX_MODEL_2D or IDK_GFX_MODEL_3D
#endif
