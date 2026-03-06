#version 460 core

out FS_in {
    layout (location = 0) vec2 texcoord;
} vsout;


void main()
{
    // vec2 position  = vec2(gl_VertexIndex%2, gl_VertexIndex/2) * 4.0 - 1.0;
    // vec2 texcoord = (position + 1) * 0.5;

    vec2 position = vec2(gl_VertexID % 2, gl_VertexID / 2) * 4.0 - 1;
    vec2 texcoord = (position + 1) * 0.5;

    vsout.texcoord = texcoord;
    gl_Position = vec4(position, 1.0, 1.0);
}




#version 460
layout(row_major) uniform;
layout(row_major) buffer;

#line 3 0
void main()
{

#line 3
    uint _S1 = uint(gl_VertexIndex);

#line 3
    gl_Position = vec4(vec2(float(_S1 % 2U), float(_S1 / 2U)) * 4.0 - 1.0, 1.0, 1.0);

#line 9
    return;
}

