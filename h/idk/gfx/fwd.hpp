#pragma once

#include "idk/core/log.hpp"
#include "idk/core/window.hpp"
#include "idk/core/service.hpp"
#include "idk/gfx/gl_bindings.hpp"

#include <memory>
#include <vector>


namespace idk::gfx
{
    class Camera;
    class WindowSDL3;
    class RenderEngine;

    using MakeObjFunc_ = void(*)(int32_t n, uint32_t *buf);
    using KillObjFunc_ = void(*)(int32_t n, uint32_t *buf);
    template <MakeObjFunc_ MakeObj, KillObjFunc_ KillObj>
    class GfxResourceBase;
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

    using FramebufferPtr = std::shared_ptr<Framebuffer>;
    using TexturePtr     = std::shared_ptr<Texture>;

}


template <idk::gfx::MakeObjFunc_ MakeObj, idk::gfx::KillObjFunc_ KillObj>
class idk::gfx::GfxResourceBase: idk::NonCopyable
{
private:

public:
    GLuint mId;
    GLuint getId() { return mId; }
    GfxResourceBase() { MakeObj(1, &mId); }
    ~GfxResourceBase() { KillObj(1, &mId); }
};


class idk::gfx::GfxResource: public idk::NonCopyable
{
private:

public:
    GLuint mId;
    GfxResource(uint32_t id): mId(id) {  }
};


