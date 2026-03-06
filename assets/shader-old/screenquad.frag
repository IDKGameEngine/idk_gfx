#version 460 core

layout (location=0) out vec4 fsout_frag_color;

in FS_in {
    layout (location=0) vec2 texcoord;
} fsin;


void main()
{
    fsout_frag_color = vec4(fsin.texcoord, 0.0, 1.0);
}
