#pragma once

#include "idk/gfx/gfx.hpp"


namespace idk::gfx::detail
{
    class Shader
    {
    public:
        uint32_t mId;
        bool     mOkay;
        const char *mFilepath;
        Shader(idk::gfx::BaseRaiiShader*, uint32_t id, const char *entryname, const char *filepath);
    };
}



class idk::gfx::BaseRaiiShader
{
public:
    const GLuint mId;
    bool         mOkay;
    const char  *mFilepath;

    BaseRaiiShader(GLuint shaderId, const char *entryname, const char *filepath);
    ~BaseRaiiShader();
};


class idk::gfx::VertexShader: public BaseRaiiShader
{
public:
    VertexShader(const char *filepath);
};


class idk::gfx::FragmentShader: public BaseRaiiShader
{
public:
    FragmentShader(const char *filepath);
};


class idk::gfx::ComputeShader: public BaseRaiiShader
{
public:
    ComputeShader(const char *filepath);
};





class idk::gfx::BaseRaiiProgram: public idk::NonCopyable
{
public:
    const GLuint mId;
    BaseRaiiProgram();
    ~BaseRaiiProgram();
};


class idk::gfx::RenderProgram: public BaseRaiiProgram
{
public:
    VertexShader   mVert;
    FragmentShader mFrag;
    bool mOkay;
    RenderProgram(const char *vertFilePath, const char *fragFilePath);
};


class idk::gfx::ComputeProgram: public BaseRaiiProgram
{
public:
    ComputeShader mComp;
    ComputeProgram(const char *compFilePath);
};



