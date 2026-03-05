#include "idk/gfx/buffer.hpp"
#include "idk/gfx/shader.hpp"

#include <cstring>

using namespace idk::gfx;



UboGpuOnly::UboGpuOnly(const char *name, size_t size)
:   GfxResource(0),
    mName(name),
    mSize(size)
{
    gl::CreateBuffers(1, &mId);
    gl::NamedBufferData(mId, mSize, nullptr, GL_DYNAMIC_DRAW); 
}

UboGpuOnly::~UboGpuOnly()
{
    gl::DeleteBuffers(1, &mId);
}

void UboGpuOnly::write(size_t offset, size_t nbytes, const void *src)
{
    gl::NamedBufferSubData(mId, offset, nbytes, src);
}

void UboGpuOnly::bindToProgram(ShaderProgram *P)
{
    GLint idx = gl::GetUniformBlockIndex(P->mId, mName);
    gl::BindBufferBase(GL_UNIFORM_BUFFER, idx, mId);
}

void UboGpuOnly::bindToIndex(uint32_t idx)
{
    gl::BindBufferBase(GL_UNIFORM_BUFFER, idx, mId);
}





UboGpuCpu::UboGpuCpu(const char *name, size_t size)
:   UboGpuOnly(name, size),
    mData(std::malloc(size))
{

}

UboGpuCpu::~UboGpuCpu()
{
    std::free(mData);
}

void UboGpuCpu::write(size_t offset, size_t nbytes, const void *src)
{
    uint8_t *base = static_cast<uint8_t*>(mData); 
    std::memcpy(base+offset, src, nbytes);
}

void UboGpuCpu::sendToGpu()
{
    gl::NamedBufferSubData(mId, 0, mSize, mData);
}






SsboGpuOnly::SsboGpuOnly(const char *name, size_t size, const void *data)
:   GfxResource(0),
    mName(name),
    mSize(size)
{
    gl::CreateBuffers(1, &mId);
    gl::NamedBufferData(mId, mSize, data, GL_DYNAMIC_DRAW); 
}

SsboGpuOnly::~SsboGpuOnly()
{
    gl::DeleteBuffers(1, &mId);
}

void SsboGpuOnly::write(size_t offset, size_t nbytes, const void *src)
{
    gl::NamedBufferSubData(mId, offset, nbytes, src);
}

void SsboGpuOnly::bindToProgram(ShaderProgram *P)
{
    GLuint idx = gl::GetProgramResourceIndex(P->mId, GL_SHADER_STORAGE_BLOCK, mName);
    gl::BindBufferBase(GL_SHADER_STORAGE_BUFFER, idx, mId);
}

void SsboGpuOnly::bindToIndex(uint32_t idx)
{
    gl::BindBufferBase(GL_SHADER_STORAGE_BUFFER, idx, mId);
}

