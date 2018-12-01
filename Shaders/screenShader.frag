#version 330 core

in VS_OUT
{
	vec2 TexCoords;
} fs_in;

uniform sampler2D screenTexture;

void main()
{ 
	gl_FragColor = texture(screenTexture, fs_in.TexCoords);
}