#pragma once

#include "idk/gfx/fwd.hpp"
// #include "gfx/gl_storage.hpp"


namespace idk::gfx
{
    template <GLenum Target_, uint32_t Idx_>
    class BufferObject: public GfxResourceBase<gl::CreateBuffers, gl::DeleteBuffers>
    {
    private:
        const size_t mSize;

    public:
        BufferObject(size_t size)
        :   GfxResourceBase(), mSize(size)
        {
            gl::NamedBufferData(mId, mSize, nullptr, GL_DYNAMIC_DRAW);
            gl::BindBufferBase(Target_, Idx_, mId);
        }

        void write(size_t offset, size_t nbytes, const void *data)
        {
            IDK_ASSERT(nbytes < mSize, "nbytes cannot be larger than mSize");
            IDK_ASSERT(offset+nbytes < mSize, "out of bounds copy");
            gl::NamedBufferSubData(mId, offset, nbytes, data);
        }

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
        static constexpr auto BINDING_IDX = T::BINDING_IDX;
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
