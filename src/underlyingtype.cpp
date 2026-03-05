#include "idk/gfx/underlyingtype.hpp"

using namespace idk::gfx;


GLenum idk::gfx::toUnderlyingType(TextureFormat from)
{
    switch (from)
    {
        case TextureFormat::RED_U8:   return GL_RED;
        case TextureFormat::RGB_U8:   return GL_RGB8;
        case TextureFormat::RGBA_U8:  return GL_RGBA8;
        case TextureFormat::RGBA_U32: return GL_RGBA32UI;
        case TextureFormat::RGBA_F16: return GL_RGBA16F;
        case TextureFormat::RGBA_F32: return GL_RGBA32F;
        default:
            VLOG_ERROR("toUnderlyingType<GLenum, gfx::TextureFormat> unknown conversion");
            return GL_INVALID_ENUM;
    }
}


TextureFormat idk::gfx::fromUnderlyingType(GLenum from)
{
    switch (from)
    {
        case GL_RED:      return TextureFormat::RED_U8;
        case GL_RGB8:     return TextureFormat::RGB_U8;
        case GL_RGBA8:    return TextureFormat::RGBA_U8;
        case GL_RGBA32UI: return TextureFormat::RGBA_U32;
        case GL_RGBA16F:  return TextureFormat::RGBA_F16;
        case GL_RGBA32F:  return TextureFormat::RGBA_F32;
        default:
            VLOG_ERROR("toUnderlyingType<GLenum, gfx::TextureFormat> unknown conversion");
            return TextureFormat::RED_U8;
    }
}




