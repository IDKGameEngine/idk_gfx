#include "idk/gfxtools/gfxtools.hpp"
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>


idk::gfxtools::RawTexture::RawTexture(const char *filepath)
:   data(nullptr),
    width(0),
    height(0),
    format(0)
{
    auto *tmp = IMG_Load(filepath);
    auto *img = SDL_ConvertSurface(tmp, SDL_PIXELFORMAT_RGBA32);

    

    SDL_DestroySurface(tmp);
    SDL_DestroySurface(img);
}



