#pragma once

#ifdef IDK_GFX_MODEL_2D
    #include "libidk/platform/PlatformSDL3.hpp"

#elif defined(IDK_GFX_MODEL_3D)
    #include "libidk/platform/PlatformSDL3GL.hpp"

#else
    #error Must define either IDK_GFX_MODEL_2D or IDK_GFX_MODEL_3D

#endif
