#version 330 core

in vec4 vPosition;
in vec2 vTexCoords;

out vec2 TexCoords;

uniform mat4 ctm;

void main()
{
    gl_Position = ctm * vPosition; 
    TexCoords = vTexCoords;
}  