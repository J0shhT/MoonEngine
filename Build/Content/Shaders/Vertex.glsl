#version 450 core

#define PI 3.14159265359

layout (location = 0) in vec4 offset;
layout (location = 1) in float currentTime;

void main(void)
{
	/*
	const vec4 vertices[] = vec4[](
		vec4(0.25 - 0.1*sin(PI*currentTime), -0.25 - 0.1*cos(PI*currentTime), 0.5, 1.0),
		vec4(-0.25 + 0.1*cos(PI*currentTime), -0.25 + 0.1*sin(PI*currentTime), 0.5, 1.0),
		vec4(0.25 + 0.1*sin(PI*currentTime),  0.25 + 0.1*cos(PI*currentTime), 0.5, 1.0)
	);
	*/
	/*
	const vec4 vertices[] = vec4[](
		vec4(0.25, -0.25, 0.5, 1.0),
		vec4(-0.25, -0.25, 0.5, 1.0),
		vec4(0.25,  0.25, 0.5, 1.0),
		vec4(-0.25, 0.25, 0.5, 1.0),
		vec4(-0.25, -0.25, 0.5, 1.0),
		vec4(0.25,  0.25, 0.5, 1.0)
	);
	*/
	const vec4 vertices[] = vec4[](
		vec4(0.25 - 0.1*sin(PI*currentTime), -0.25 - 0.1*cos(PI*currentTime), 0.5, 1.0),
		vec4(-0.25 - 0.1*cos(PI*currentTime), -0.25 + 0.1*sin(PI*currentTime), 0.5, 1.0),
		vec4(0.25 + 0.1*sin(PI*currentTime), 0.25 + 0.1*cos(PI*currentTime), 0.5, 1.0),
		vec4(-0.25, 0.25, 0.5, 1.0),
		vec4(-0.25 - 0.1*cos(PI*currentTime), -0.25 + 0.1*sin(PI*currentTime), 0.5, 1.0),
		vec4(0.25 + 0.1*sin(PI*currentTime), 0.25 + 0.1*cos(PI*currentTime), 0.5, 1.0)
	);
	gl_Position = vertices[gl_VertexID] + offset;
}

