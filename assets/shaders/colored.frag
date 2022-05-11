#version 450 core

//shader input
layout(location = 0) in vec4 inCol;

//output write
layout(location = 0) out vec4 FragColor;


void main()
{
	//return color
	FragColor = inCol;
	//FragColor = vec4(1.0, 1.0, 1.0, 1.0f);
}