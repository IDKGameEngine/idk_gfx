#include "idk/gfx/framebuffer.hpp"
#include "idk/gfx/texture.hpp"

using namespace idk::gfx;


Framebuffer::Framebuffer()
:   GfxResource(0)
{
    gl::CreateFramebuffers(1, &mId);
    // gl::NamedFramebufferTexture(mId, GL_COLOR_ATTACHMENT0, mTexture->mId, 0);
    // gl::NamedFramebufferTexture(mId, GL_DEPTH_ATTACHMENT, depthTex, 0);

    // if (gl::CheckNamedFramebufferStatus(mId, GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    // {
    //     VLOG_ERROR("Error on glCheckNamedFramebufferStatus");
    // }
}


Framebuffer::Framebuffer(Texture &&tex)
:   GfxResource(0), mTexture(new Texture(std::move(tex)))
{
    gl::CreateFramebuffers(1, &mId);
    gl::NamedFramebufferTexture(mId, GL_COLOR_ATTACHMENT0, mTexture->mId, 0);
    // gl::NamedFramebufferTexture(mId, GL_DEPTH_ATTACHMENT, depthTex, 0);

    _check_status();
}


Framebuffer::~Framebuffer()
{
    gl::DeleteFramebuffers(1, &mId);
}


void Framebuffer::_check_status()
{
    if (gl::CheckNamedFramebufferStatus(mId, GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        VLOG_ERROR("Error on glCheckNamedFramebufferStatus");
    }
}


void Framebuffer::setTextureDst(const std::shared_ptr<Texture> &outTex)
{
    mTexture = outTex;
    gl::NamedFramebufferTexture(mId, GL_COLOR_ATTACHMENT0, mTexture->mId, 0);
    _check_status();
}


