#include "idk/gfx/texture.hpp"

using namespace idk::gfx;

TextureFormatDesc TextureFormatDesc::DefaultFormat(GL_RGBA16F, GL_RGBA, GL_UNSIGNED_BYTE);


Texture::Texture(int w, int h, const void *data, const TextureFormatDesc &desc)
:   GfxResource(0U),
    mWidth(w),
    mHeight(h),
    mDesc(desc)
{
    GLenum texInternalformat = mDesc.gpuInternalFormat;
    GLenum texFormat = mDesc.cpuPixelLayout;
    GLenum texType = mDesc.cpuPixelDataType;

    gl::CreateTextures(GL_TEXTURE_2D, 1, &mId);
    gl::TextureParameteri(mId, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
    gl::TextureParameteri(mId, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
    gl::TextureParameteri(mId, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    gl::TextureParameteri(mId, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    gl::TextureStorage2D(mId, 1, texInternalformat, w, h);
    gl::TextureSubImage2D(mId, 0, 0, 0, w, h, texFormat, texType, data);

    // gl::TextureStorage2D(mId, 1, GL_DEPTH_COMPONENT24, w, h);
    // gl::TextureSubImage2D(mId, 0, 0, 0, w, h, GL_DEPTH_COMPONENT, GL_FLOAT, data);

}


// Texture::Texture(Texture &&tex)
// :   GfxResource(tex.mId),
//     mWidth(tex.mWidth), mHeight(tex.mHeight),
//     mTextureFormat(tex.mTextureFormat)
// {

// }


// Texture &Texture::operator=(Texture &&tex)
// {
//     mId = tex.mId;
//     mTextureFormat = tex.mTextureFormat;
//     return *this;
// }


Texture::~Texture()
{
    gl::DeleteTextures(1, &mId);
}



