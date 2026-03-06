struct VertexShaderInput
{
    uint id : SV_VertexID;
};

struct VertexShaderOutput
{
    float2 uv;
    float4 sv_position : SV_Position;
};


[shader("vertex")]
VertexShaderOutput vertmain(VertexShaderInput vsin)
{
    VertexShaderOutput vsout;

    float2 pos = float2(vsin.id % 2, vsin.id / 2) * 4.0 - 1.0;
    float2 tex = (pos + 1.0) * 0.5;

    vsout.sv_position = float4(pos, 1.0, 1.0);
    return vsout;
}


[shader("fragment")]
float4 fragmain(VertexShaderOutput fsin)
{
    return float4(fsin.uv, 0.0, 1.0);
}

