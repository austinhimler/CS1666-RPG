#version 120

attribute vec4 vPosition;
attribute vec4 vColor;
varying vec4 color;

uniform mat4 ctm;

void main()
{
	color = vColor;
	gl_Position = ctm * vPosition;
}
