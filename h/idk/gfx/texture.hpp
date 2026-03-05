#pragma once

#include "idk/gfx/gfx.hpp"


class idk::gfx::Texture: public GfxResource
{
public:
    const int mWidth;
    const int mHeight;
    TextureFormat mTextureFormat;

    Texture(int w, int h, const void *data, TextureFormat fmt=TextureFormat::RGBA_F16);
    Texture(Texture&&);
    Texture &operator=(Texture&&);
    ~Texture();

    TextureFormat getTextureFormat();
    void setTextureFormat(TextureFormat);

};

