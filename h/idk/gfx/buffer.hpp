#pragma once

#include "idk/gfx/fwd.hpp"
#include "idk/core/assert.hpp"
// #include "gfx/gl_storage.hpp"


namespace idk::gfx
{
    template <GLenum Target_>
    class BufferObject: public GfxResourceBase<gl::CreateBuffers, gl::DeleteBuffers>
    {
    private:
        const size_t mSize;

    public:
        BufferObject(size_t sz)
        : GfxResourceBase(), mSize(sz)
        {
            gl::NamedBufferData(mId, mSize, nullptr, GL_DYNAMIC_DRAW);
        }

        BufferObject(size_t sz, uint32_t idx)
        : GfxResourceBase(), mSize(sz)
        {
            gl::NamedBufferData(mId, mSize, nullptr, GL_DYNAMIC_DRAW);
            gl::BindBufferBase(Target_, idx, mId);
        }

        void write(size_t offset, size_t nbytes, const void *data)
        {
            IDK_ASSERT(nbytes <= mSize, "nbytes cannot be larger than mSize");
            IDK_ASSERT(offset+nbytes <= mSize, "out of bounds copy");
            gl::NamedBufferSubData(mId, offset, nbytes, data);
        }

        void bind(uint32_t idx)
        {
            gl::BindBufferBase(Target_, idx, mId);
        }
    };

    template <typename T>
    class UboWriter: public gfx::BufferObject<GL_UNIFORM_BUFFER>
    {
    private:
        T object_;
    public:
        UboWriter(): BufferObject<GL_UNIFORM_BUFFER>(sizeof(T), T::BIND_IDX) {  }
        void sendToGpu() { this->write(0, sizeof(T), &object_); }
        T *operator->() { return &object_; }
    };

    template <typename T>
    class SsboWriter: public gfx::BufferObject<GL_SHADER_STORAGE_BUFFER>
    {
    private:
        T object_;
    public:
        SsboWriter(): BufferObject<GL_SHADER_STORAGE_BUFFER>(sizeof(T)) {  }
        void sendToGpu() { this->write(0, sizeof(T), &object_); }
        T *operator->() { return &object_; }
    };



    class UboGpuOnly: public GfxResource
    {
    protected:
        const size_t mSize;
        const char *mName;

    public:
        UboGpuOnly(size_t size, const char *name = "");
        ~UboGpuOnly();
        void write(size_t offset, size_t nbytes, const void *src);
        void bindToProgram(BaseRaiiProgram*);
        void bindToIndex(uint32_t idx);
    };


    class UboGpuCpu: public UboGpuOnly
    {
    protected:
        void *mData;

    public:
        UboGpuCpu(size_t size, const char *name = "");
        ~UboGpuCpu();
        void write(size_t offset, size_t nbytes, const void *src);
        void sendToGpu();
    };


    template <typename T>
    class UboWrapperT: public UboGpuCpu
    {
    private:
        T &mObject;

    public:
        static constexpr auto BIND_IDX = T::BIND_IDX;
        bool mDirty;

        UboWrapperT(const char *name = "")
        :   UboGpuCpu(sizeof(T), name),
            mObject(*static_cast<T*>(mData)),
            mDirty(true)
        {
            static_assert(std::is_standard_layout_v<T>, "T must be standard layout");
        }

        T *operator->() { return &mObject; }
    };
}
