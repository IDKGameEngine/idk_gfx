#pragma once

#include "idk/gfx/fwd.hpp"
// #include "gfx/gl_storage.hpp"


namespace idk::gfx
{
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



// namespace idk::gfx
// {
//     class SsboGpuOnly: public GfxResource
//     {
//     protected:
//         const char *mName;
//         const size_t mSize;

//     public:
//         SsboGpuOnly(const char *name, size_t size, const void *data = nullptr);
//         ~SsboGpuOnly();
//         void write(size_t offset, size_t size, const void *data);
//         void bindToProgram(BaseRaiiProgram*);
//         void bindToIndex(uint32_t idx);
//     };
// }

