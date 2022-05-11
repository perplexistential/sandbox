#version 450 core

layout (location=0) in vec2 position;
layout (location=1) in vec4 color;

layout (location=0) out vec4 outColor;

void main()
{
	//output the position of each vertex
	gl_Position = vec4(position, 0, 1.0);
	outColor = color;
}