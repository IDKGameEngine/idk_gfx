#pragma once

#include "idk/gfx/fwd.hpp"
#include <SDL3/SDL.h>

namespace idk
{
    namespace gfx
    {
        class RenderEngine;
    }

    namespace gfx__
    {
        class MeshBuffer;
    }
}


class idk::gfx::RenderEngine: public idk::core::IRenderer
{
public:
    RenderEngine(const idk::core::WindowDesc&);
    virtual ~RenderEngine();
    virtual void beginFrame() final;
    virtual void endFrame() final;

    void debugOutputEnable();
    void debugOutputDisable();

private:
    gfx::WindowSDL3 *win_;
    // SDL_Window *mSdlWin;
    // SDL_GLContext mGlCtx;
    GLuint mDummyVao;
    gfx__::MeshBuffer *meshbuf_;

};


namespace idk::gfx
{
    enum class GfxCmdType: int
    {
        Invalid = 0,
        BgColor,
        FgColor,
        SomethingElse
    };


    struct GfxCmdData_BgColor
    {
        static constexpr GfxCmdType COMMAND_TYPE = GfxCmdType::BgColor;
        
    };

    struct GfxCmdData_FgColor
    {
        static constexpr GfxCmdType COMMAND_TYPE = GfxCmdType::FgColor;
        
    };

    struct GfxCmdData
    {
        idk::gfx::GfxCmdType type;

        union
        {
            GfxCmdData_BgColor as_BgColor;
            GfxCmdData_FgColor as_FgColor;
        };
    };


    void submit_gfxcmd(GfxCmdType type, void *data)
    {

    }

    template <typename T>
    void submit_gfxcmd(T cmd_data)
    {
        submit_gfxcmd(T::COMMAND_TYPE, (void*)cmd_data)
    }

}


