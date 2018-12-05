#version 120

attribute vec4 vPosition;
attribute vec2 vTexCoords;

varying vec2 TexCoords;

uniform mat4 ctm;

void main()
{
    gl_Position = ctm * vPosition; 
    TexCoords = vTexCoords;
}  