#version 330 core

in vec4 vPosition;
in vec4 vColor;

out vec4 color;

uniform mat4 ctm;

void main(void)
{
	color = vColor;
	gl_Position = ctm * vPosition;
}
