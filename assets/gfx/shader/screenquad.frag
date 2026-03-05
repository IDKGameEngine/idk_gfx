#version 460 core

layout (location=0) out vec4 fsout_frag_color;

in FS_in {
    layout (location=0) vec2 texcoord;
} fsin;

layout (std140, binding = 0)
uniform ubo_CameraData {
    vec4 mouse;
    vec4 rgba;
    mat4 T;
    mat4 V;
    mat4 P;
} uboCameraData;
// #include "ubo.glsl"

layout(binding = 1) uniform sampler2D unTexture;


void main()
{
    vec2 uv = fsin.texcoord;

    float dist = distance(uv, uboCameraData.mouse.xy);
    vec3 color = vec3(dist); // uboCameraData.color.xyz;

    if (uv.x > 0.5)
    {
        color = texture(unTexture, uv).rgb;
    }

    fsout_frag_color = vec4(color, 1.0);
}
