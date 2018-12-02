#version 330 core

in vec4 vPosition;
in vec2 vTexCoords;

out vec2 TexCoords;

void main()
{
    gl_Position = vPosition; 
    TexCoords = vTexCoords;
}  