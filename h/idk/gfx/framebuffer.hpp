#pragma once

#include "idk/gfx/gfx.hpp"


class idk::gfx::Framebuffer: public GfxResource
{
public:
    TexturePtr mTexture;
    void _check_status();

    Framebuffer();
    Framebuffer(Texture&&);
    ~Framebuffer();
    void setTextureDst(const TexturePtr&);
};
