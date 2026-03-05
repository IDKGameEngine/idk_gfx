#pragma once

#include <glad/glad.h>
#include <GL/glu.h>
// #include <GL/gl.h>

#include <cstdio>
#include <cassert>


#if defined(__GNUC__)
    #define IDK_INLINE [[gnu::always_inline]] inline
#elif defined(__clang__)
    #define IDK_INLINE [[gnu::always_inline]] [[gnu::gnu_inline]] extern inline
#elif defined(_MSC_VER)
    #pragma warning(error: 4714)
    #define IDK_INLINE __forceinline
#else
    #error Unsupported compiler
#endif


#define IDK_OPENGL_DEBUG
#ifdef IDK_OPENGL_DEBUG
    #define IDK_GLCALL(Content_) \
    { \
        while (glGetError() != GL_NO_ERROR); \
        Content_ \
        GLenum err = glGetError(); \
        if (err != GL_NO_ERROR) \
        { \
            printf("OpenGL Error: %s\n", gluErrorString(err)); \
            fflush(stdout); \
            assert(err == GL_NO_ERROR); \
        } \
    }
#else
    #define IDK_GLCALL(Content_) \
    { \
        Content_ \
    }
#endif


namespace idk::gl
{
    struct DrawCmd
    {
        GLuint count;
        GLuint instanceCount;
        GLuint firstIndex;
        GLuint baseVertex;
        GLuint baseInstance;
    };


    IDK_INLINE void Enable(GLenum cap)  { IDK_GLCALL( glEnable(cap); ) };
    IDK_INLINE void Disable(GLenum cap) { IDK_GLCALL( glDisable(cap); ) };

    IDK_INLINE void Enable()  {  };
    IDK_INLINE void Disable() {  };

    template <typename... GLenums>
    IDK_INLINE void Enable(GLenum cap, GLenums... rest) { Enable(cap);  Enable(rest...); }

    template <typename... GLenums>
    IDK_INLINE void Disable(GLenum cap, GLenums... rest) { Disable(cap);  Disable(rest...); }

    IDK_INLINE void DispatchCompute( GLuint num_groups_x, GLuint num_groups_y, GLuint num_groups_z )
    { IDK_GLCALL( glDispatchCompute(num_groups_x, num_groups_y, num_groups_z); ) }

    IDK_INLINE void MemoryBarrier( GLbitfield barriers )
    { IDK_GLCALL( glMemoryBarrier(barriers); ) }

    IDK_INLINE void GetBooleanv( GLenum pname, GLboolean *data )
    { IDK_GLCALL( glGetBooleanv(pname, data); ) }

    IDK_INLINE void GetDoublev( GLenum pname, GLdouble *data )
    { IDK_GLCALL( glGetDoublev(pname, data); ) }

    IDK_INLINE void GetFloatv( GLenum pname, GLfloat *data )
    { IDK_GLCALL( glGetFloatv(pname, data); ) }

    IDK_INLINE void GetIntegerv( GLenum pname, GLint *data )
    { IDK_GLCALL( glGetIntegerv(pname, data); ) }

    IDK_INLINE void GetInteger64v( GLenum pname, GLint64 *data )
    { IDK_GLCALL( glGetInteger64v(pname, data); ) }

    IDK_INLINE void GetBooleani_v( GLenum target, GLuint index, GLboolean *data )
    { IDK_GLCALL( glGetBooleani_v(target, index, data); ) }

    IDK_INLINE void GetIntegeri_v( GLenum target, GLuint index, GLint *data )
    { IDK_GLCALL( glGetIntegeri_v(target, index, data); ) }

    IDK_INLINE void GetFloati_v( GLenum target, GLuint index, GLfloat *data )
    { IDK_GLCALL( glGetFloati_v(target, index, data); ) }

    IDK_INLINE void GetDoublei_v( GLenum target, GLuint index, GLdouble *data )
    { IDK_GLCALL( glGetDoublei_v(target, index, data); ) }

    IDK_INLINE void GetInteger64i_v( GLenum target, GLuint index, GLint64 *data )
    { IDK_GLCALL( glGetInteger64i_v(target, index, data); ) }

    IDK_INLINE void GetProgramiv( GLuint program, GLenum pname, GLint *param )
    { IDK_GLCALL( glGetProgramiv(program, pname, param); ) }

    IDK_INLINE GLuint GetProgramResourceIndex(GLuint program, GLenum programInterface, const char *name)
    {
        GLuint data;
        IDK_GLCALL( data = glGetProgramResourceIndex(program, programInterface, name); )
        return data;
    }


    // glGen...
    // ---------------------------------------------------------------------------------------
    IDK_INLINE void GenVertexArrays( GLsizei n, GLuint *arrays )
    { IDK_GLCALL( glGenVertexArrays(n, arrays); ) }

    IDK_INLINE void GenBuffers( GLsizei n, GLuint *buffers )
    { IDK_GLCALL( glGenBuffers(n, buffers); ) }

    IDK_INLINE void GenTextures( GLsizei n, GLuint *textures )
    { IDK_GLCALL( glGenTextures(n, textures); ) }

    IDK_INLINE void GenFramebuffers( GLsizei n, GLuint *framebuffers )
    { IDK_GLCALL( glGenFramebuffers(n, framebuffers); ) }

    IDK_INLINE void GenRenderbuffers( GLsizei n, GLuint *renderbuffers )
    { IDK_GLCALL( glGenRenderbuffers(n, renderbuffers); ) }
    // ---------------------------------------------------------------------------------------

    // glDelete...
    // ---------------------------------------------------------------------------------------
    IDK_INLINE void DeleteVertexArrays( GLsizei n, GLuint *arrays )
    { IDK_GLCALL( glDeleteVertexArrays(n, arrays); ) }

    IDK_INLINE void DeleteBuffers( GLsizei n, GLuint *buffers )
    { IDK_GLCALL( glDeleteBuffers(n, buffers); ) }

    IDK_INLINE void DeleteTextures( GLsizei n, GLuint *textures )
    { IDK_GLCALL( glDeleteTextures(n, textures); ) }

    IDK_INLINE void DeleteFramebuffers( GLsizei n, GLuint *framebuffers )
    { IDK_GLCALL( glDeleteFramebuffers(n, framebuffers); ) }

    IDK_INLINE void DeleteRenderbuffers( GLsizei n, GLuint *renderbuffers )
    { IDK_GLCALL( glDeleteRenderbuffers(n, renderbuffers); ) }
    // ---------------------------------------------------------------------------------------


    // glBind...
    // ---------------------------------------------------------------------------------------
    IDK_INLINE void BindVertexArray( GLuint VAO )
    { IDK_GLCALL( glBindVertexArray(VAO); ) }

    IDK_INLINE void BindBuffer( GLenum type, GLuint buf )
    { IDK_GLCALL( glBindBuffer(type, buf); ) }

    IDK_INLINE void BindBufferBase( GLenum target, GLuint index, GLuint buffer )
    { IDK_GLCALL( glBindBufferBase(target, index, buffer); ) }

    IDK_INLINE void BindFramebuffer( GLenum target, GLuint framebuffer )
    { IDK_GLCALL( glBindFramebuffer(target, framebuffer); ) }

    IDK_INLINE void BindRenderbuffer( GLenum target, GLuint renderbuffer )
    { IDK_GLCALL( glBindRenderbuffer(target, renderbuffer); ) }

    IDK_INLINE void BindTexture( GLenum target, GLuint texture )
    { IDK_GLCALL( glBindTexture(target, texture); ) }

    IDK_INLINE void BindTextureUnit( GLuint unit, GLuint texture )
    { IDK_GLCALL( glBindTextureUnit(unit, texture); ) }

    IDK_INLINE void BindImageTexture( GLuint unit, GLuint texture, GLint level, GLboolean layered,
                            GLint layer, GLenum access, GLenum format )
    { IDK_GLCALL( glBindImageTexture(unit, texture, level, layered, layer, access, format); ) }

    IDK_INLINE void BindImageTextures( GLuint first, GLsizei count, const GLuint *textures )
    { IDK_GLCALL( glBindImageTextures(first, count, textures); ) }

    IDK_INLINE void ClearTexImage( GLuint texture, GLint level, GLenum format, GLenum type,
                            const void *data )
    { IDK_GLCALL( glClearTexImage(texture, level, format, type, data); ) }
    // ---------------------------------------------------------------------------------------


    // glDrawXXX -----------------------------------------------------------------------------
    /**/
    IDK_INLINE void DrawArrays( GLenum mode, GLint first, GLsizei count )
    { IDK_GLCALL( glDrawArrays(mode, first, count); ) }

    IDK_INLINE void DrawElements( GLenum mode, GLsizei count, GLenum type, const void *indices )
    { IDK_GLCALL( glDrawElements(mode, count, type, indices); ) }

    IDK_INLINE void DrawElementsInstanced( GLenum mode, GLsizei count, GLenum type,
                                    const void *indices, GLsizei instancecount )
    { IDK_GLCALL( glDrawElementsInstanced(mode, count, type, indices, instancecount); ) }

    IDK_INLINE void MultiDrawElements( GLenum mode, const GLsizei *count, GLenum type,
                                const void *const *indices, GLsizei drawcount )
    { IDK_GLCALL( glMultiDrawElements(mode, count, type, indices, drawcount); ) }

    IDK_INLINE void MultiDrawElementsIndirect( GLenum mode, GLenum type, const void *indirect,
                                        GLsizei primcount, GLsizei stride )
    { IDK_GLCALL( glMultiDrawElementsIndirect(mode, type, indirect, primcount, stride); ) }
    // ---------------------------------------------------------------------------------------


    // glBufferXXX ---------------------------------------------------------------------------
    /**/
    IDK_INLINE void BufferData( GLenum target, GLsizeiptr size, const void *data, GLenum usage )
    { IDK_GLCALL( glBufferData(target, size, data, usage); ) }

    IDK_INLINE void BufferSubData( GLenum target, GLintptr offset, GLsizeiptr size, const void *data)
    { IDK_GLCALL( glBufferSubData(target, offset, size, data); ) }

    IDK_INLINE void ShaderSource( GLuint shader, GLsizei count, const GLchar *const *string,
                                   const GLint *length )
    { IDK_GLCALL( glShaderSource(shader, count, string, length); ) }

    IDK_INLINE void ShaderBinary( GLsizei count, const GLuint *shaders, GLenum binaryformat,
                        const void *binary, GLsizei length )
    { IDK_GLCALL( glShaderBinary(count, shaders, binaryformat, binary, length); ) }

    IDK_INLINE void SpecializeShader(GLuint shader, const GLchar *pEntryPoint, GLuint numSpecializationConstants,
                                      const GLuint *pConstantIndex, const GLuint *pConstantValue)
    { IDK_GLCALL( glSpecializeShaderARB(shader, pEntryPoint, numSpecializationConstants, pConstantIndex, pConstantValue); ) }

    IDK_INLINE void CompileShader( GLuint shader )
    { IDK_GLCALL( glCompileShader(shader); ) }

    IDK_INLINE void GetShaderiv( GLuint shader, GLenum pname, GLint *param )
    { IDK_GLCALL( glGetShaderiv(shader, pname, param); ) }

    IDK_INLINE void GetShaderInfoLog( GLuint shader, GLsizei bufSize, GLsizei *length, GLchar *infoLog )
    { IDK_GLCALL( glGetShaderInfoLog(shader, bufSize, length, infoLog); ) }

    IDK_INLINE void AttachShader( GLuint program, GLuint shader )
    { IDK_GLCALL( glAttachShader(program, shader); ) }

    IDK_INLINE void DeleteShader( GLuint shader )
    { IDK_GLCALL( glDeleteShader(shader); ) }

    IDK_INLINE void ValidateProgram( GLuint program )
    { IDK_GLCALL( glValidateProgram(program); ) }

    IDK_INLINE void LinkProgram( GLuint program )
    { IDK_GLCALL( glLinkProgram(program); ) }

    IDK_INLINE void ObjectLabel(GLenum identifier, GLuint name, GLsizei length, const char *label)
    { IDK_GLCALL( glObjectLabel(identifier, name, length, label); ) }

    IDK_INLINE void ObjectPtrLabel(void *ptr, GLsizei length, const char *label)
    { IDK_GLCALL( glObjectPtrLabel(ptr, length, label); ) }

    IDK_INLINE void GetObjectLabel(GLenum identifier, GLuint name, GLsizei bufSize, GLsizei *length, char *label)
    { IDK_GLCALL( glGetObjectLabel(identifier, name, bufSize, length, label); ) }

    IDK_INLINE void GetObjectPtrLabel(void *ptr, GLsizei bufSize, GLsizei *length, char *label)
    { IDK_GLCALL( glGetObjectPtrLabel(ptr, bufSize, length, label); ) }

    IDK_INLINE void *MapBuffer( GLenum target, GLenum access )
    {
        void *data;
        IDK_GLCALL( data = glMapBuffer(target, access); )
        return data;
    }

    IDK_INLINE void UnmapBuffer( GLenum target )
    { IDK_GLCALL( glUnmapBuffer(target); ) }
    // ---------------------------------------------------------------------------------------


    // glActiveXXX ---------------------------------------------------------------------------
    /**/
    /**/
    IDK_INLINE void ActiveTexture( GLenum texture )
    { IDK_GLCALL( glActiveTexture(texture); ) }
    // ---------------------------------------------------------------------------------------


    // glTexXXX ------------------------------------------------------------------------------
    /**/
    IDK_INLINE void TexImage2D( GLenum target, GLint level, GLint internalformat, GLsizei w, GLsizei h,
                        GLint border, GLenum format, GLenum type, const void *data )
    { IDK_GLCALL( glTexImage2D(target, level, internalformat, w, h, border, format, type, data); ) }

    IDK_INLINE void TexSubImage2D( GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width,
                            GLsizei height, GLenum format, GLenum type, const void *pixels )
    { IDK_GLCALL( glTexSubImage2D(target, level, xoffset, yoffset, width, height, format, type, pixels); ) }

    IDK_INLINE void TexImage3D( GLenum target, GLint level, GLint internalformat, GLsizei w, GLsizei h,
                        GLsizei d, GLint border, GLenum format, GLenum type, const void *data )
    { IDK_GLCALL( glTexImage3D(target, level, internalformat, w, h, d, border, format, type, data); ) }

    IDK_INLINE void TexParameteri( GLenum target, GLenum pname, GLint param )
    { IDK_GLCALL( glTexParameteri(target, pname, param); ) }

    IDK_INLINE void GetTextureImage( GLuint texture, GLint level, GLenum format, GLenum type,
                            GLsizei bufSize, void *pixels )
    { IDK_GLCALL( glGetTextureImage(texture, level, format, type, bufSize, pixels); ) }


    IDK_INLINE GLuint64 GetTextureHandleARB( GLuint texture )
    {
        GLuint64 handle;
        IDK_GLCALL( handle = glGetTextureHandleARB(texture); )
        return handle;
    }

    IDK_INLINE void MakeTextureHandleResidentARB( GLuint64 handle )
    { IDK_GLCALL( glMakeTextureHandleResidentARB(handle); ) }

    IDK_INLINE void MakeTextureHandleNonResidentARB( GLuint64 handle )
    { IDK_GLCALL( glMakeTextureHandleNonResidentARB(handle); ) }

    IDK_INLINE void GenerateTextureMipmap( GLuint texture )
    { IDK_GLCALL( glGenerateTextureMipmap(texture); ) }

    IDK_INLINE void GenerateMipmap( GLenum target )
    { IDK_GLCALL( glGenerateMipmap(target); ) }

    IDK_INLINE void PixelStorei( GLenum pname, GLint param )
    { IDK_GLCALL( glPixelStorei(pname, param); ) }
    // ---------------------------------------------------------------------------------------------



    // glTextureXXX
    // ---------------------------------------------------------------------------------------------
    IDK_INLINE void TextureStorage2D( GLuint texture, GLsizei levels, GLenum internalformat,
                                      GLsizei width, GLsizei height )
    { IDK_GLCALL( glTextureStorage2D(texture, levels, internalformat, width, height); ) }

    IDK_INLINE void TextureSubImage2D( GLenum texture, GLint level, GLint xoffset, GLint yoffset,
                                       GLsizei width, GLsizei height, GLenum format, GLenum type,
                                       const void *pixels )
    {
        IDK_GLCALL(
            glTextureSubImage2D(
                texture, level, xoffset, yoffset, width, height, format, type, pixels
            );
        )
    }

    IDK_INLINE void TextureStorage3D( GLuint texture, GLsizei levels, GLenum internalformat,
                                      GLsizei width, GLsizei height, GLsizei depth )
    { IDK_GLCALL( glTextureStorage3D(texture, levels, internalformat, width, height, depth); ) }


    IDK_INLINE void TextureSubImage3D( GLuint texture, GLint level, GLint xoffset, GLint yoffset,
                                       GLint zoffset, GLsizei width, GLsizei height, GLsizei depth,
                                       GLenum format, GLenum type, const void *pixels )
    {
        IDK_GLCALL(
            glTextureSubImage3D(
                texture,
                level,
                xoffset, yoffset, zoffset,
                width, height, depth,
                format,
                type,
                pixels
            );
        )
    }


    IDK_INLINE void TextureParameteri( GLuint texture, GLenum pname, GLint param )
    { IDK_GLCALL( glTextureParameteri(texture, pname, param); ) }

    IDK_INLINE void TextureParameterf( GLuint texture, GLenum pname, GLfloat param )
    { IDK_GLCALL( glTextureParameterf(texture, pname, param); ) }
    // ---------------------------------------------------------------------------------------------


    // glFramebufferXXX ---------------------------------------------------------------------
    IDK_INLINE void FramebufferTexture( GLenum target, GLenum attachment, GLuint texture, GLint level )
    { IDK_GLCALL( glFramebufferTexture(target, attachment, texture, level); ) }

    IDK_INLINE void FramebufferTexture2D( GLenum target, GLenum attachment, GLenum textarget,
                                          GLuint texture, GLint level )
    { IDK_GLCALL( glFramebufferTexture2D(target, attachment, textarget, texture, level); ) }

    IDK_INLINE void FramebufferTexture3D( GLenum target, GLenum attachment, GLenum textarget,
                                          GLuint texture, GLint level, GLint layer )

    { IDK_GLCALL( glFramebufferTexture3D(target, attachment, textarget, texture, level, layer); ) }

    IDK_INLINE void FramebufferTextureLayer( GLenum target, GLenum attachment, GLuint texture,
                                             GLint level, GLint layer )
    { IDK_GLCALL( glFramebufferTextureLayer(target, attachment, texture, level, layer); ) }
    // ---------------------------------------------------------------------------------------


    // Uniforms ------------------------------------------------------------------------------

    IDK_INLINE GLint GetUniformLocation( GLuint program, const char *name )
    {
        int32_t data;
        IDK_GLCALL( data = glGetUniformLocation(program, name); )
        return data;
    }

    IDK_INLINE GLuint GetUniformBlockIndex(GLuint program, const GLchar *uniformBlockName)
    {
        GLuint data;
        IDK_GLCALL( data = glGetUniformBlockIndex(program, uniformBlockName); )
        return data;
    }

    IDK_INLINE void UniformBlockBinding(GLuint program, GLuint uniformBlockIndex, GLuint uniformBlockBinding)
    { IDK_GLCALL( glUniformBlockBinding(program, uniformBlockIndex, uniformBlockBinding); ) }

    IDK_INLINE void Uniform1i( GLint loc, int i )
    { IDK_GLCALL( glUniform1i(loc, i); ) }

    IDK_INLINE void Uniform1f( GLint loc, float f )
    { IDK_GLCALL( glUniform1f(loc, f); ) }

    IDK_INLINE void Uniform2iv( GLint loc, GLsizei count, const int *value )
    { IDK_GLCALL( glUniform2iv(loc, count, value); ) }

    IDK_INLINE void Uniform3iv( GLint loc, GLsizei count, const int *value )
    { IDK_GLCALL( glUniform3iv(loc, count, value); ) }

    IDK_INLINE void Uniform2fv( GLint loc, GLsizei count, float *value )
    { IDK_GLCALL( glUniform2fv(loc, count, value); ) }

    IDK_INLINE void Uniform3fv( GLint loc, GLsizei count, float *value )
    { IDK_GLCALL( glUniform3fv(loc, count, value); ) }

    IDK_INLINE void Uniform4fv( GLint loc, GLsizei count, float *value )
    { IDK_GLCALL( glUniform4fv(loc, count, value); ) }

    IDK_INLINE void UniformMatrix3fv( GLint loc, GLsizei count, GLboolean transpose, float *value )
    { IDK_GLCALL( glUniformMatrix3fv(loc, count, transpose, value); ) }

    IDK_INLINE void UniformMatrix4fv( GLint loc, GLsizei count, GLboolean transpose, float *value )
    { IDK_GLCALL( glUniformMatrix4fv(loc, count, transpose, value); ) }

    IDK_INLINE void UniformHandleui64ARB( GLint location, GLuint64 value )
    { IDK_GLCALL( glUniformHandleui64ARB(location, value); ) }
    // ---------------------------------------------------------------------------------------


    IDK_INLINE void VertexAttribPointer( GLuint index, GLint size,
                                GLenum type, GLboolean normalized,
                                GLsizei stride, GLuint offset )
    {
        IDK_GLCALL(
            glVertexAttribPointer(
                index, size, type, normalized,
                stride, reinterpret_cast<void *>(offset)
            );
        )
    }

    IDK_INLINE void EnableVertexAttribArray( GLuint index )
    { IDK_GLCALL( glEnableVertexAttribArray(index); ) }

    IDK_INLINE void VertexArrayVertexBuffer( GLuint vaobj, GLuint bindingindex, GLuint buffer,
                                    GLintptr offset, GLsizei stride )
    { IDK_GLCALL( glVertexArrayVertexBuffer(vaobj, bindingindex, buffer, offset, stride); ) }

    IDK_INLINE void VertexArrayElementBuffer( GLuint vaobj, GLuint buffer )
    { IDK_GLCALL( glVertexArrayElementBuffer(vaobj, buffer); ) }

    IDK_INLINE void EnableVertexArrayAttrib( GLuint vaobj, GLuint index )
    { IDK_GLCALL( glEnableVertexArrayAttrib(vaobj, index); ) }

    IDK_INLINE void VertexArrayAttribFormat( GLuint vaobj, GLuint attribindex, GLint size, GLenum type,
                                    GLboolean normalized, GLuint relativeoffset )
    {
        IDK_GLCALL(
            glVertexArrayAttribFormat(
                vaobj, attribindex, size, type, normalized, relativeoffset
            );
        )
    }

    IDK_INLINE void VertexArrayAttribBinding( GLuint vaobj, GLuint attribindex, GLuint bindingindex )
    { IDK_GLCALL( glVertexArrayAttribBinding(vaobj, attribindex, bindingindex); ) }

    IDK_INLINE void UseProgram( GLuint program )
    { IDK_GLCALL( glUseProgram(program); ) }

    IDK_INLINE void DeleteProgram( GLuint program )
    { IDK_GLCALL( glDeleteProgram(program); ) }

    IDK_INLINE void ClearColor( float r, float g, float b, float a )
    { IDK_GLCALL( glClearColor(r, g, b, a); ) }

    IDK_INLINE void Clear( GLbitfield mask )
    { IDK_GLCALL( glClear(mask); ) }

    IDK_INLINE void Viewport( GLint x, GLint y, GLsizei w, GLsizei h )
    { IDK_GLCALL( glViewport(x, y, w, h); ) }

    IDK_INLINE void CullFace( GLenum mode )
    { IDK_GLCALL( glCullFace(mode); ) }

    IDK_INLINE void BlendFunc( GLenum sfactor, GLenum dfactor )
    { IDK_GLCALL( glBlendFunc(sfactor, dfactor); ) }

    IDK_INLINE void BlendFunci( GLuint buf, GLenum sfactor, GLenum dfactor )
    { IDK_GLCALL( glBlendFunci(buf, sfactor, dfactor); ) }


    IDK_INLINE void DebugMessageControl(GLenum source, GLenum type, GLenum severity,
                                    GLsizei count, const GLuint *ids, GLboolean enabled)
    { IDK_GLCALL( glDebugMessageControl(source, type, severity, count, ids, enabled); ) }

    IDK_INLINE void DebugMessageCallback(GLDEBUGPROC callback, const void *userParam)
    { IDK_GLCALL( glDebugMessageCallback(callback, userParam); ) }




    // OpenGL 4.6 AZDO functions
    // ---------------------------------------------------------------------------------------
    IDK_INLINE void CreateTextures( GLenum target, GLsizei n, GLuint *textures )
    { IDK_GLCALL( glCreateTextures(target, n, textures); ) }

    IDK_INLINE void CreateFramebuffers( GLsizei n, GLuint *framebuffers )
    { IDK_GLCALL( glCreateFramebuffers(n, framebuffers); ) }

    IDK_INLINE void CreateRenderbuffers( GLsizei n, GLuint *framebuffers )
    { IDK_GLCALL( glCreateRenderbuffers(n, framebuffers); ) }

    IDK_INLINE void CreateVertexArrays(GLsizei n, GLuint *arrays)
    { IDK_GLCALL( glCreateVertexArrays(n, arrays); ) }

    IDK_INLINE void CreateBuffers(GLsizei n, GLuint *buffers)
    { IDK_GLCALL( glCreateBuffers(n, buffers); ) }

    IDK_INLINE GLuint CreateProgram()
    { IDK_GLCALL( return glCreateProgram(); ) }

    IDK_INLINE GLuint CreateShader(GLenum type)
    { IDK_GLCALL( return glCreateShader(type); ) }

    IDK_INLINE void NamedBufferData( GLuint buffer, GLsizeiptr size, const void *data, GLenum usage )
    { IDK_GLCALL( glNamedBufferData(buffer, size, data, usage); ) }

    IDK_INLINE void NamedBufferSubData( GLuint buffer, GLintptr offset, GLsizeiptr size, const void *data )
    { IDK_GLCALL( glNamedBufferSubData(buffer, offset, size, data); ) }

    IDK_INLINE void NamedBufferStorage( GLuint buffer, GLsizeiptr size, const void *data, GLbitfield flags )
    { IDK_GLCALL( glNamedBufferStorage(buffer, size, data, flags); ) }

    IDK_INLINE void NamedRenderbufferStorage( GLuint renderbuffer, GLenum internalformat, GLsizei width,
                                    GLsizei height )
    { IDK_GLCALL( glNamedRenderbufferStorage(renderbuffer, internalformat, width, height); ) }

    IDK_INLINE void NamedFramebufferRenderbuffer( GLuint framebuffer, GLenum attachment,
                                        GLenum renderbuffertarget, GLuint renderbuffer )
    { IDK_GLCALL( glNamedFramebufferRenderbuffer(framebuffer, attachment, renderbuffertarget, renderbuffer); ) }

    IDK_INLINE void *MapNamedBuffer( GLuint buffer, GLenum access )
    {
        void *data;
        IDK_GLCALL( data = glMapNamedBuffer(buffer, access); )
        return data;
    }

    IDK_INLINE void UnmapNamedBuffer( GLuint buffer )
    { IDK_GLCALL( glUnmapNamedBuffer(buffer); ) }

    IDK_INLINE void *MapNamedBufferRange( GLuint buffer, GLintptr offset, GLsizeiptr length, GLbitfield access )
    {
        void *data;
        IDK_GLCALL( data = glMapNamedBufferRange(buffer, offset, length, access); )
        return data;
    }

    IDK_INLINE void NamedFramebufferTexture( GLuint framebuffer, GLenum attachment, GLuint texture,
                                             GLint level )
    { IDK_GLCALL( glNamedFramebufferTexture(framebuffer, attachment, texture, level); ) }

    IDK_INLINE void NamedFramebufferDrawBuffer( GLuint framebuffer, GLenum mode )
    { IDK_GLCALL( glNamedFramebufferDrawBuffer(framebuffer, mode); ) }

    IDK_INLINE void NamedFramebufferDrawBuffers( GLuint framebuffer, GLsizei n, const GLenum *bufs )
    { IDK_GLCALL( glNamedFramebufferDrawBuffers(framebuffer, n, bufs); ) }

    IDK_INLINE void NamedFramebufferTextureLayer( GLuint framebuffer, GLenum attachment, GLuint texture,
                                                  GLint level, GLint layer )
    { IDK_GLCALL( glNamedFramebufferTextureLayer(framebuffer, attachment, texture, level, layer); ) }

    IDK_INLINE void ClearNamedFramebufferiv(GLuint framebuffer, GLenum buffer,
                                        GLint drawbuffer, const GLint *value)
    { IDK_GLCALL( glClearNamedFramebufferiv(framebuffer, buffer, drawbuffer, value); ) }

    IDK_INLINE void ClearNamedFramebufferuiv(GLuint framebuffer, GLenum buffer,
                                         GLint drawbuffer, const GLuint *value)
    { IDK_GLCALL( glClearNamedFramebufferuiv(framebuffer, buffer, drawbuffer, value); ) }

    IDK_INLINE void ClearNamedFramebufferfv(GLuint framebuffer, GLenum buffer,
                                        GLint drawbuffer, const GLfloat *value)
    { IDK_GLCALL( glClearNamedFramebufferfv(framebuffer, buffer, drawbuffer, value); ) }

    IDK_INLINE void ClearNamedFramebufferfi(GLuint framebuffer, GLenum buffer,
                                        GLint drawbuffer, GLfloat depth, GLint stencil)
    { IDK_GLCALL( glClearNamedFramebufferfi(framebuffer, buffer, drawbuffer, depth, stencil); ) }

    IDK_INLINE GLenum CheckNamedFramebufferStatus(GLuint framebuffer, GLenum target)
    {
        GLenum status;
        IDK_GLCALL( status = glCheckNamedFramebufferStatus(framebuffer, target); )
        return status;
    }
    // ---------------------------------------------------------------------------------------

};




