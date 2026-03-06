#pragma once

#include "idk/core/type.hpp"
#include "idk/core/log.hpp"
#include "idk/gfx/gl_bindings.hpp"

#include <memory>
#include <vector>


namespace idk::gfx
{
    class Camera;

    class GfxResource;
    class Framebuffer;
    class Texture;
    class ShaderProgram;
    class RenderProgram;
    class ComputeProgram;

    using FramebufferPtr    = std::shared_ptr<Framebuffer>;
    using TexturePtr        = std::shared_ptr<Texture>;
    using RenderProgramPtr  = std::shared_ptr<RenderProgram>;
    using ComputeProgramPtr = std::shared_ptr<ComputeProgram>;

    enum class TextureFormat
    {
        RED_U8,
        RGB_U8,
        RGBA_U8,
        RGBA_U32,
        RGBA_F16,
        RGBA_F32
    };
}



class idk::gfx::GfxResource: idk::NonCopyable
{
public:
    GLuint mId;
    GfxResource(GLuint id): mId(id) {  }
};


