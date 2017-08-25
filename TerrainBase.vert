#version 400

layout (location = 0) in vec4 position;
layout (location = 1) in vec2 texCoord;

out vec2 TexCoord;

void main()
{
	gl_Position =  position;
	TexCoord = texCoord;
}
