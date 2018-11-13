#ifndef _____SHADER_H_____
#define _____SHADER_H_____

#include <glew/glew.h>
#include <SDL2/SDL_opengl.h>
#include <stdio.h>
#include <stdlib.h>

class Shader
{
public:
	const char*  filename;
	GLenum       type;
	char*        source;
	
	Shader(const char* f, GLenum t, char* s);
	
	static GLuint initShader(const char* vertexShaderFile, const char* fragmentShaderFile);

protected:
	static char* readShaderSource(const char* shaderFile);
};

#endif // _____SHADER_H_____
