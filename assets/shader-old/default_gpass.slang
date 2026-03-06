struct VertexShaderInput
{
    float4 pos;
    float4 norm;
    float2 tex;
};

struct VertexShaderOutput
{
    float3 worldpos;
    float3 worldnorm;
    float2 texcoord;
    float4 sv_position: SV_Position;
};

struct FragmentShaderOutput
{
    float4 color;
};

struct camera_t
{
    float4x4 M;
    float4x4 V;
    float4x4 P;
    float4x4 MVP;
};

cbuffer Uniforms
{
    ConstantBuffer<camera_t> cam;
    // RWStructuredBuffer<camera_t> rw_ree; // read write
    // StructuredBuffer<camera_t> ro_ree;   // read only
};



[shader("vertex")]
VertexShaderOutput vertmain(VertexShaderInput vsin)
{
    VertexShaderOutput vsout;

    float3 position = mul(cam.M, vsin.pos).xyz;

    vsout.worldpos    = position;
    vsout.worldnorm   = normalize(mul(float3x3(cam.M), vsin.norm.xyz));
    vsout.texcoord    = vsin.tex;
    vsout.sv_position = mul(cam.P * cam.V, float4(position, 1.0));

    return vsout;
}


[shader("fragment")]
FragmentShaderOutput fragmain(VertexShaderOutput fsin): SV_Target
{
    FragmentShaderOutput fsout;

    fsout.color = float4(fsin.worldnorm, 1.0);

    return fsout;
}

