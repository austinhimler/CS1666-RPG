#version 330 core

in vec4 vText;

out vec2 texCoords;

uniform mat4 projection;

void main()
{
    gl_Position = projection * vec4(vText.xy, 0.0, 1.0);
    texCoords = vText.zw;
}  