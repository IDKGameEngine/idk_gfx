#pragma once

#include "idk/core/log.hpp"
#include "idk/core/type.hpp"
#include "idk/core/renderer.hpp"
#include "idk/gfx/gl_bindings.hpp"

#include <memory>
#include <vector>


namespace idk::gfx
{
    class Camera;
    class WindowSDL3;

    class GfxResource;
    class Framebuffer;
    class Texture;

    class BaseRaiiShader;
    class VertexShader;
    class FragmentShader;
    class ComputeShader;

    class BaseRaiiProgram;
    class RenderProgram;
    class ComputeProgram;

    using FramebufferPtr    = std::shared_ptr<Framebuffer>;
    using TexturePtr        = std::shared_ptr<Texture>;

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
private:

public:
    GLuint mId;
    GfxResource(uint32_t id): mId(id) {  }
};


