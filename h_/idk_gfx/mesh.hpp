#pragma once

#include "idk/gfx/fwd.hpp"

// desc.attrib_size[idx]   = 2;
// desc.attrib_nbytes[idx] = sizeof(glm::vec2);
// idx += 1;

// desc.attrib_size[idx]   = 4;
// desc.attrib_nbytes[idx] = sizeof(glm::ivec4);
// idx += 1;

// desc.attrib_size[idx]   = 4;
// desc.attrib_nbytes[idx] = sizeof(glm::vec4);
// idx += 1;

namespace idk::gfx__
{
    class  VertexDescriptor;
    struct BufferObject;
    struct VertexArrayObject;
    struct MeshDescriptor;
    class  MeshBuffer;
}



class idk::gfx__::VertexDescriptor
{
private:
    GLuint offset_;

public:
    struct Attribute
    {
        GLuint size;
        GLuint bytesize;
        GLenum datatype;
        GLuint offset;
        GLboolean normalized;
    };

    uint32_t  size;
    uint32_t  stride;
    uint32_t  attrcount;
    Attribute attrs[8];

    void pushAttr(GLuint size, GLuint bytesize, GLenum datatype, GLboolean normalized = GL_FALSE)
    {
        attrs[attrcount++] = {size, bytesize, datatype, offset_, normalized};
        offset_ += bytesize;
        size    += bytesize;
        stride  += bytesize;
    }

    void pushAttrf(GLuint n, bool norm=false) { pushAttr(n, n*sizeof(float), GL_FLOAT, norm); }
    void pushAttri(GLuint n, bool norm=false) { pushAttr(n, n*sizeof(int32_t), GL_INT, norm); }
    void pushAttru(GLuint n, bool norm=false) { pushAttr(n, n*sizeof(uint32_t), GL_UNSIGNED_INT, norm); }

    VertexDescriptor()
    :   offset_(0), size(0), stride(0), attrcount(0)
    {
        pushAttrf(3);        // x y z
        pushAttrf(3, true);  // x y z
        pushAttrf(2);        // u v
    }

};


struct idk::gfx__::MeshDescriptor
{
    int    material;
    GLuint baseVertex, firstIndex, numVertices, numIndices;

    MeshDescriptor(GLuint basevert, GLuint firstidx, GLuint vertcount, GLuint idxcount)
    :   material(0), baseVertex(basevert), firstIndex(firstidx),
        numVertices(vertcount), numIndices(idxcount) {  }
};


struct idk::gfx__::BufferObject: public idk::gfx::GfxResource
{
    size_t capacity;
    size_t offset;
    float  totalusage;
    BufferObject(size_t capacity);
    void write_data(void *src, size_t nbytes);
};


struct idk::gfx__::VertexArrayObject: public idk::gfx::GfxResource
{
    VertexDescriptor desc_;
    BufferObject &vbo_;
    BufferObject &ibo_;
    VertexArrayObject(const VertexDescriptor&, BufferObject &vbo, BufferObject &ibo);
};


class idk::gfx__::MeshBuffer
{
public:
    MeshBuffer(size_t nbytes_vertices, size_t nbytes_indices);

    MeshDescriptor loadMesh( size_t nbytes_vertices, size_t nbytes_indices,
                             void *vertices, void *indices );

    void  bind();
    void  clear();
    void *mapVBO(GLenum access);
    void *mapIBO(GLenum access);
    void  unmapVBO();
    void  unmapIBO();

private:
    uint32_t basevertex_, baseindex_;
    VertexDescriptor  vdesc_;
    BufferObject      vbo_;
    BufferObject      ibo_;
    VertexArrayObject vao_;

    void *vertex_alloc(size_t);
    void *index_alloc(size_t);

};

