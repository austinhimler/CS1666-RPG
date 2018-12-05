#version 120

attribute vec4 vText;

varying vec2 texCoords;

uniform mat4 projection;

void main()
{
    gl_Position = projection * vec4(vText.xy, 0.0, 1.0);
    texCoords = vText.zw;
}  