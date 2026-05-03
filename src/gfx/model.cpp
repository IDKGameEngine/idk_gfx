#include "idk/gfx/mesh.hpp"
#include "idk/gfx/gl_bindings.hpp"
#include "idk/core/assert.hpp"
#include "idk/core/metric.hpp"

#include <glm/glm.hpp>
#include <cmath>
#include <vector>

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
            desc.attrs[i].size,
            desc.attrs[i].datatype,
            GL_FALSE,
            desc.attrs[i].offset
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



struct VertexTypeA
{
    glm::vec3 pos;
    glm::vec3 nrm;
    glm::vec2 tex;
};


gfx::MeshDescriptor gfx::MeshBuffer::generateCircle(float x, float y, float r, int n)
{
    std::vector<VertexTypeA> vertices;
    std::vector<uint32_t> indices;

    IDK_ASSERT(sizeof(VertexTypeA) == vdesc_.size, "{} != {}", sizeof(VertexTypeA), vdesc_.size);

    vertices.push_back({
        glm::vec3(x, y, 0.0f),
        glm::vec3(0.0f),
        glm::vec2(0.0f)
    });

    for (int i=0; i<=n; i++)
    {
        float a = 2.0f * M_PI * (i/n);
        vertices.push_back({
            glm::vec3(x + r*cos(a), y + r*sin(a), 0.0f),
            glm::vec3(0.0f),
            glm::vec2(0.0f)
        });
    }

    for (int i=0; i<=n; i++)
    {
        indices.push_back(0);       // center
        indices.push_back(i);       // current perimeter vertex
        indices.push_back(i%n + 1); // next (wraps around)
    }

    return loadMesh(
        vertices.size() * sizeof(vertices[0]),
        indices.size() * sizeof(indices[0]),
        vertices.data(),
        indices.data()
    );
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

