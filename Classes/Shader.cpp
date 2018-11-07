#include "../Headers/Shader.h"


Shader::Shader(const char* f, GLenum t, char* s)
{
	filename = f;
	type = t;
	source = s;
}

// Create a NULL-terminated string by reading the provided file
char* Shader::readShaderSource(const char* shaderFile)
{
	FILE* fp = fopen(shaderFile, "rb");

	if (fp == NULL)
		return NULL;

	fseek(fp, 0L, SEEK_END);
	long size = ftell(fp);

	fseek(fp, 0L, SEEK_SET);
	char *buf = (char *)malloc(size + 1);
	fread(buf, 1, size, fp);

	buf[size] = '\0';
	fclose(fp);

	return buf;
}


// Create a GLSL program object from vertex and fragment shader files
GLuint Shader::initShader(const char* vShaderFile, const char* fShaderFile)
{
	int i;

	Shader shaders[2] = {
		{ vShaderFile, GL_VERTEX_SHADER, NULL },
		{ fShaderFile, GL_FRAGMENT_SHADER, NULL }
	};

	GLuint program = glCreateProgram();

	for (i = 0; i < 2; ++i)
	{
		struct Shader s = shaders[i];
		s.source = readShaderSource(s.filename);
		if (s.source == NULL)
		{
			fprintf(stderr, "Failed to read %s\n", s.filename);
			exit(EXIT_FAILURE);
		}

		GLuint shader = glCreateShader(s.type);
		glShaderSource(shader, 1, (const GLchar**)&s.source, NULL);
		glCompileShader(shader);

		GLint  compiled;
		glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
		if (!compiled)
		{
			fprintf(stderr, "%s failed to compile:\n", s.filename);
			GLint  logSize;
			glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logSize);
			char* logMsg = (char *)malloc(logSize);
			glGetShaderInfoLog(shader, logSize, NULL, logMsg);
			fprintf(stderr, "%s\n", logMsg);
			free(logMsg);

			exit(EXIT_FAILURE);
		}

		free(s.source);

		glAttachShader(program, shader);
	}

	// link  and error check 
	glLinkProgram(program);

	GLint  linked;
	glGetProgramiv(program, GL_LINK_STATUS, &linked);
	if (!linked)
	{
		fprintf(stderr, "Shader program failed to link\n");
		GLint  logSize;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logSize);
		char *logMsg = (char *)malloc(logSize);
		glGetProgramInfoLog(program, logSize, NULL, logMsg);
		fprintf(stderr, "%s\n", logMsg);
		free(logMsg);

		exit(EXIT_FAILURE);
	}

	//use program object 
	glUseProgram(program);

	return program;
}