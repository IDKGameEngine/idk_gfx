#pragma once

#include "idk/gfx/fwd.hpp"

namespace idk::gfx
{
    class  VertexDescriptor;
    struct VertexBuffer;
    struct VertexArrayObject;
    struct MeshDescriptor;
    class  MeshBuffer;
}

class idk::gfx::VertexDescriptor
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

    void pushAttr(GLuint sz, GLuint bytesz, GLenum dtype, GLboolean normed = GL_FALSE)
    {
        attrs[attrcount++] = {sz, bytesz, dtype, offset_, normed};
        offset_ += bytesz;
        size    += bytesz;
        stride  += bytesz;
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


struct idk::gfx::MeshDescriptor
{
    int    material;
    GLuint baseVertex, firstIndex, numVertices, numIndices;

    MeshDescriptor() {  }
    MeshDescriptor(GLuint basevert, GLuint firstidx, GLuint vertcount, GLuint idxcount)
    :   material(0), baseVertex(basevert), firstIndex(firstidx),
        numVertices(vertcount), numIndices(idxcount) {  }
};


struct idk::gfx::VertexBuffer: public idk::gfx::GfxResource
{
    size_t capacity;
    size_t offset;
    float  totalusage;
    VertexBuffer(size_t capacity);
    void write_data(void *src, size_t nbytes);
};


struct idk::gfx::VertexArrayObject: public idk::gfx::GfxResource
{
    VertexBuffer &vbo_;
    VertexBuffer &ibo_;
    VertexArrayObject(const VertexDescriptor&, VertexBuffer &vbo, VertexBuffer &ibo);
};


class idk::gfx::MeshBuffer
{
public:
    MeshBuffer(size_t nbytes_vertices, size_t nbytes_indices);

    MeshDescriptor loadMesh( size_t nbytes_vertices, size_t nbytes_indices,
                             void *vertices, void *indices );

    MeshDescriptor generateCircle(float x, float y, float r, int n);

    void  bind();
    void  clear();
    void *mapVBO(GLenum access);
    void *mapIBO(GLenum access);
    void  unmapVBO();
    void  unmapIBO();

private:
    uint32_t basevertex_, baseindex_;
    VertexDescriptor  vdesc_;
    VertexBuffer      vbo_;
    VertexBuffer      ibo_;
    VertexArrayObject vao_;

    void *vertex_alloc(size_t);
    void *index_alloc(size_t);

};


