#version 460 core

out FS_in {
    layout (location = 0) vec2 texcoord;
} vsout;

layout (std140, binding = 0)
uniform ubo_CameraData {
    vec4 mouse;
    vec4 rgba;
    mat4 T;
    mat4 V;
    mat4 P;
} uboCameraData;
// #include "ubo.glsl"

void main()
{
    // vec2 position  = vec2(gl_VertexIndex%2, gl_VertexIndex/2) * 4.0 - 1.0;
    // vec2 texcoord = (position + 1) * 0.5;

    vec2 position = vec2(gl_VertexID % 2, gl_VertexID / 2) * 4.0 - 1;
    vec2 texcoord = (position + 1) * 0.5;

    vsout.texcoord = texcoord;
    gl_Position = vec4(position, 1.0, 1.0);
}

