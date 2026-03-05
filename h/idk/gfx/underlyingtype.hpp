#pragma once

#include "idk/gfx/gfx.hpp"


namespace idk::gfx
{
    // template<typename T>
    // struct as_underlying { using type = T; };

    // template<typename T>
    // struct as_underlying<T>::type toUnderlyingType(T);

    // template<typename T>
    // T fromUnderlyingType(as_underlying<T>::type);

    GLenum toUnderlyingType(TextureFormat from);
    TextureFormat fromUnderlyingType(GLenum from);

}

