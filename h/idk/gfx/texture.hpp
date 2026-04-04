#pragma once

#include "idk/gfx/fwd.hpp"


namespace idk::gfx
{
    struct TextureFormatDesc
    {
        uint32_t gpuInternalFormat;
        uint32_t cpuPixelLayout;
        uint32_t cpuPixelDataType;

        TextureFormatDesc(uint32_t gpuFormat, uint32_t cpuFormat, uint32_t dataType)
        : gpuInternalFormat(gpuFormat), cpuPixelLayout(cpuFormat), cpuPixelDataType(dataType) {  }

        static TextureFormatDesc DefaultFormat;
    };
}


class idk::gfx::Texture: public GfxResource
{
public:
    const int mWidth;
    const int mHeight;
    TextureFormatDesc mDesc;

    Texture(int w, int h, const void *data, const TextureFormatDesc &desc = TextureFormatDesc::DefaultFormat);
    ~Texture();

    void bindImageTexture(uint32_t unit)
    {
        gl::BindImageTexture(unit, mId, 0, GL_FALSE, 0, GL_READ_WRITE, mDesc.gpuInternalFormat);
    }
};

