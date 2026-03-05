#include "idk/gfx/texture.hpp"
#include "idk/gfx/underlyingtype.hpp"

using namespace idk::gfx;


Texture::Texture(int w, int h, const void *data, TextureFormat fmt)
:   GfxResource(0),
    mWidth(w), mHeight(h),
    mTextureFormat(fmt)
{
    GLenum internalformat = toUnderlyingType(mTextureFormat);

    gl::CreateTextures(GL_TEXTURE_2D, 1, &mId);
    gl::TextureParameteri(mId, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
    gl::TextureParameteri(mId, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
    gl::TextureParameteri(mId, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    gl::TextureParameteri(mId, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    gl::TextureStorage2D(mId, 1, internalformat, w, h);
    gl::TextureSubImage2D(mId, 0, 0, 0, w, h, GL_RGBA, GL_UNSIGNED_BYTE, data);

    // gl::TextureStorage2D(mId, 1, GL_DEPTH_COMPONENT24, w, h);
    // gl::TextureSubImage2D(mId, 0, 0, 0, w, h, GL_DEPTH_COMPONENT, GL_FLOAT, data);

}


Texture::Texture(Texture &&tex)
:   GfxResource(tex.mId),
    mWidth(tex.mWidth), mHeight(tex.mHeight),
    mTextureFormat(tex.mTextureFormat)
{

}


Texture &Texture::operator=(Texture &&tex)
{
    mId = tex.mId;
    mTextureFormat = tex.mTextureFormat;
    return *this;
}


Texture::~Texture()
{
    gl::DeleteTextures(1, &mId);
}


TextureFormat Texture::getTextureFormat() { return mTextureFormat; }
void Texture::setTextureFormat(TextureFormat fmt) { mTextureFormat = fmt; }


