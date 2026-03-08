#pragma once

#include <cstdint>
#include <vector>


namespace idk::gfxtools
{
    struct RawTexture
    {
        void    *data;
        int32_t  width;
        int32_t  height;
        uint32_t format;
        RawTexture(const char *filepath);
    };

    struct RawMesh
    {
        RawTexture            texture;
        std::vector<float>    vertices;
        std::vector<uint32_t> indices;
    };

    struct RawModel
    {
        std::vector<RawMesh> meshes;
    };
};

