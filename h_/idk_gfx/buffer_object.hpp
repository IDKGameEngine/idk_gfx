#pragma once

#include "idk_gfx/mesh.hpp"
// #include "gfx/gl_storage.hpp"

namespace idk::gfx__
{
    struct BufferObject;
    struct VertexArrayObject;
}


struct idk::gfx__::BufferObject: public idk::gfx::GfxResource
{
    size_t capacity;
    size_t writeidx;
    uint8_t *baseptr;
    BufferObject(size_t capacity);
};

struct idk::gfx__::VertexArrayObject: public idk::gfx::GfxResource
{
    VertexDescriptor desc_;
    BufferObject &vbo_;
    BufferObject &ibo_;
    VertexArrayObject(const VertexDescriptor&, BufferObject &vbo, BufferObject &ibo);
};
