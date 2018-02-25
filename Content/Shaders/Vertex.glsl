#version 450 core

#define PI 3.14159265359

layout (location = 0) in vec3 vertexPosition;
layout (location = 1) in float currentTime;

uniform mat4 worldViewMatrix;

void main(void)
{
	gl_Position = worldViewMatrix * vec4(vertexPosition, 1.0);
}

