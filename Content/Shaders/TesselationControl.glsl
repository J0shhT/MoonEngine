#version 450 core

layout (vertices = 6) out;

void main(void)
{
	if (gl_InvocationID == 0)
	{
		gl_TessLevelInner[0] = 3;
		gl_TessLevelOuter[0] = 3;
		gl_TessLevelOuter[1] = 3;
		gl_TessLevelOuter[2] = 3;
	}
	gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;
}