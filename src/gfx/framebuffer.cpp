#include "idk/gfx/framebuffer.hpp"
#include "idk/gfx/texture.hpp"

using namespace idk::gfx;


// Framebuffer::Framebuffer()
// :   GfxResourceBase()
// {
//     // gl::NamedFramebufferTexture(mId, GL_COLOR_ATTACHMENT0, mTexture->mId, 0);
//     // gl::NamedFramebufferTexture(mId, GL_DEPTH_ATTACHMENT, depthTex, 0);

//     // if (gl::CheckNamedFramebufferStatus(mId, GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
//     // {
//     //     VLOG_ERROR("Error on glCheckNamedFramebufferStatus");
//     // }
// }


Framebuffer::Framebuffer(TexturePtr tex)
:   GfxResourceBase(),
    mTexture(tex)
{
    gl::NamedFramebufferTexture(mId, GL_COLOR_ATTACHMENT0, mTexture->mId, 0);
    // gl::NamedFramebufferTexture(mId, GL_DEPTH_ATTACHMENT, depthTex, 0);
    _check_status();
}


void Framebuffer::bind()
{
    gl::BindFramebuffer(GL_FRAMEBUFFER, mId);
}


void Framebuffer::_check_status()
{
    if (gl::CheckNamedFramebufferStatus(mId, GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        VLOG_ERROR("Error on glCheckNamedFramebufferStatus");
    }
}


// void Framebuffer::setTextureDst(const std::shared_ptr<Texture> &outTex)
// {
//     mTexture = outTex;
//     gl::NamedFramebufferTexture(mId, GL_COLOR_ATTACHMENT0, mTexture->mId, 0);
//     _check_status();
// }


