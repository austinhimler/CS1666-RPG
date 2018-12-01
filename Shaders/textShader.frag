#version 330 core
in vec2 texCoords;

uniform sampler2D text;
uniform vec4 textColor;

void main()
{    
    vec4 sampled = vec4(1.0, 1.0, 1.0, texture(text, texCoords).r);
    gl_FragColor = vec4(textColor) * sampled;
} 