#!/bin/bash

# if [[ "$#" != "1" ]]; then
#     echo "Usage: shader_build.sh [SHADER_DIR]"
#     exit 1
# fi

if [[ "$IDK_ROOT_DIR" == "" ]]; then
    echo "IDK_ROOT_DIR must be defined"
    exit 1
fi

if [[ "$IDK_OUTPUT_DIR" == "" ]]; then
    echo "IDK_OUTPUT_DIR must be defined"
    exit 1
fi

__slang_to_spirv()
{
    export PATH="${PATH}:${IDK_ROOT_DIR}/bin"
    export LD_LIBRARY_PATH="${LD_LIBRARY_PATH}:${IDK_ROOT_DIR}/lib"

    stage="$1"
    srcpath="$2"
    dstpath="${srcpath%.*}"."${stage}"
    entryname="${stage}main"

    if grep -q "$entryname" "$srcpath"; then
        slangc "${srcpath}" \
            -I "${IDK_POLY_DIR}/idk_gfx/h" \
            -D__SLANG__ \
            -O3 \
            -matrix-layout-column-major \
            -fvk-use-gl-layout -fspv-reflect \
            -emit-spirv-directly \
            -target spirv \
            -profile glsl_460 \
            -entry $entryname \
            -o $dstpath
    fi
}

slang_to_spirv()
{
    while [ $# -gt 0 ]; do
        __slang_to_spirv "vert" "$1"
        __slang_to_spirv "frag" "$1"
        __slang_to_spirv "comp" "$1"
        shift
    done
}

cd "$IDK_OUTPUT_DIR/assets/shader"
slang_to_spirv *.slang && rm *.slang

