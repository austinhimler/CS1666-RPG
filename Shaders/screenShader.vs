#version 330 core

layout (location = 0) in vec2 position;
layout (location = 1) in vec2 texCoords;

out VS_OUT
{
	vec2 TexCoords;
} vs_out;

void main()
{
    gl_Position = vec4(position.x, position.y, 0.0f, 1.0f); 
    vs_out.TexCoords = vec2(texCoords.x, 1 - texCoords.y);
}  