#include "idk/gfx/mesh.hpp"
#include "idk/gfx/gl_bindings.hpp"
#include "idk/core/assert.hpp"
#include "idk/core/metric.hpp"

using namespace idk;


gfx::VertexBuffer::VertexBuffer(size_t bytesize)
:   gfx::GfxResource(0),
    capacity(bytesize),
    offset(0),
    totalusage(0.0f)
{
    gl::CreateBuffers(1, &mId);
    gl::NamedBufferData(mId, capacity, nullptr, GL_DYNAMIC_COPY);
}

void gfx::VertexBuffer::write_data(void *src, size_t nbytes)
{
    gl::NamedBufferSubData(mId, offset, nbytes, src);
    offset += nbytes;

    totalusage = float(offset) / capacity;
    totalusage = float(int(10000.0f * totalusage)) / 10000.0f;
    totalusage = 100.0f * totalusage;
}



gfx::VertexArrayObject::VertexArrayObject(const VertexDescriptor &desc, VertexBuffer &vbo, VertexBuffer &ibo)
:   GfxResource(0),
    desc_(desc),
    vbo_(vbo),
    ibo_(ibo)
{
    GLuint binding_index = 0;

    gl::CreateVertexArrays(1, &mId);
    gl::VertexArrayVertexBuffer(mId, binding_index, vbo_.mId, 0, desc.size);
    gl::VertexArrayElementBuffer(mId, ibo_.mId);

    for (uint32_t i=0; i<desc.attrcount; i++)
    {
        gl::EnableVertexArrayAttrib(mId, i);

        gl::VertexArrayAttribFormat(
            mId, i,
            desc_.attrs[i].size,
            desc_.attrs[i].datatype,
            GL_FALSE,
            desc_.attrs[i].offset
        );

        gl::VertexArrayAttribBinding(mId, i, binding_index);
    }
}


gfx::MeshBuffer::MeshBuffer(size_t nbytes_vertices, size_t nbytes_indices)
:   basevertex_(0),
    baseindex_(0),
    vdesc_(),
    vbo_(nbytes_vertices),
    ibo_(nbytes_indices),
    vao_(vdesc_, vbo_, ibo_)
{

}




gfx::MeshDescriptor gfx::MeshBuffer::loadMesh( size_t nbytes_vertices, size_t nbytes_indices,
                                               void *vertices, void *indices )
{
    vbo_.write_data(vertices, nbytes_vertices);
    vbo_.write_data(indices, nbytes_indices);

    VLOG_INFO("MeshBuffer VBO usage: {}%", vbo_.totalusage);
    VLOG_INFO("MeshBuffer IBO usage: {}%", ibo_.totalusage);

    uint32_t vertcount = uint32_t(nbytes_vertices / vdesc_.size);
    uint32_t idxcount  = uint32_t(nbytes_indices  / sizeof(uint32_t));

    MeshDescriptor desc(
        basevertex_, baseindex_, vertcount, idxcount
    );

    basevertex_ += vertcount;
    baseindex_  += idxcount;

    return desc;
}

void gfx::MeshBuffer::bind()
{
    gl::BindVertexArray(vao_.mId);
}

void gfx::MeshBuffer::clear()
{
    vbo_.offset = 0;
    ibo_.offset = 0;
}



void *gfx::MeshBuffer::mapVBO( GLenum access )
{
    return gl::MapNamedBuffer(vbo_.mId, access);
}

void *gfx::MeshBuffer::mapIBO( GLenum access )
{
    return gl::MapNamedBuffer(ibo_.mId, access);
}

void gfx::MeshBuffer::unmapVBO()
{
    gl::UnmapNamedBuffer(vbo_.mId);
}

void gfx::MeshBuffer::unmapIBO()
{
    gl::UnmapNamedBuffer(ibo_.mId);
}

