
#version 330 core
attribute vec4 vPosition;
attribute vec4 vColor;

out vec4 color;

uniform mat4 ctm;

void main(void)
{
	color = vColor;
	gl_Position = ctm * vPosition;
}
