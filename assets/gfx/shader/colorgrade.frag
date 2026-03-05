#version 450 core

layout (location=0) out vec4 fsout_frag_color;

in FS_in {
    layout (location=0) vec2 texcoord;
} fsin;

layout(binding = 0) uniform sampler2D unInput;


vec3 aces(vec3 x)
{
    const float a = 2.51;
    const float b = 0.03;
    const float c = 2.43;
    const float d = 0.59;
    const float e = 0.14;
    return clamp((x * (a * x + b)) / (x * (c * x + d) + e), 0.0, 1.0);
}


vec3 tonemap_aces( vec3 hdr, float gamma, float exposure )
{
    vec3 sdr = aces(exposure * hdr);
    return pow(sdr, vec3(1.0 / gamma));
}


void main()
{
    float exposure = 1.0;
    float gamma    = 2.2;

    vec3 hdr = textureLod(unInput, fsin.texcoord, 0.0).rgb;
        //  hdr += camera.bloom * texture(un_bloom, fsin_texcoords).rgb;

    vec3 color = tonemap_aces(hdr, gamma, exposure);
    // vec3 color = tonemap_agx(hdr, gamma, exposure);

    fsout_frag_color = vec4(color, 1.0);
}
