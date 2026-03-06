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
        Shader(idk::gfx::ShaderProgram*, uint32_t id, const char *entryname, const char *filepath);
    };
}



class idk::gfx::ShaderProgram
{
public:
    const GLuint mId;
    ShaderProgram();
    ~ShaderProgram();
};


class idk::gfx::RenderProgram: public ShaderProgram
{
public:
    detail::Shader mVert, mFrag;
    RenderProgram(const char *vertFilePath, const char *fragFilePath);
};


class idk::gfx::ComputeProgram: public ShaderProgram
{
public:
    detail::Shader mComp;
    ComputeProgram(const char *compFilePath);
};



