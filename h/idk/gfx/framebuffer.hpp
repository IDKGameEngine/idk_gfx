#pragma once

#include "idk/gfx/fwd.hpp"


class idk::gfx::Framebuffer
:   public GfxResourceBase<gl::CreateFramebuffers, gl::DeleteFramebuffers>
{
public:
    TexturePtr mTexture;
    Framebuffer(TexturePtr);
    void bind();

private:
    void _check_status();

};
