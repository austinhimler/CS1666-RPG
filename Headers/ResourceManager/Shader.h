
#ifndef _____SHADER_H_____
#define _____SHADER_H_____

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <GL/glew.h>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>

// Const
enum SHADER_TYPE
{
	VERTEX = 0,
	FRAGMENT = 1,
	GEOMETRY = 2
};

const GLuint SHADER_TYPE_VERTEX = 0;
const GLuint SHADER_TYPE_FRAGMENT = 1;
const GLuint SHADER_TYPE_GEOMETRY = 2;

class Shader
{
public:

	// State
	GLuint Program;
	// Constructor
	Shader();
	// Sets the current shader as active
	void use() { glUseProgram(this->Program); }

	void release();

	// Add shader parts to shader
	void addVertexShader(const GLchar* vertexSourcePath);
	void addFragmentShader(const GLchar* fragmentSourcePath);
	void addGeometryShader(const GLchar* geometrySourcePath);

	void compile(const GLchar* vertexSourcePath, const GLchar* fragmentSourcePath, const GLchar* geometrySourcePath);

	// Set functions
	void setFloat(const GLchar *name, GLfloat value, GLboolean useShader = false);
	void setInteger(const GLchar *name, GLint value, GLboolean useShader = false);
	void setBoolean(const GLchar *name, GLboolean value, GLboolean useShader = false);
	void setVector2f(const GLchar *name, GLfloat x, GLfloat y, GLboolean useShader = false);
	void setVector2f(const GLchar *name, const glm::vec2 &value, GLboolean useShader = false);
	void setVector3f(const GLchar *name, GLfloat x, GLfloat y, GLfloat z, GLboolean useShader = false);
	void setVector3f(const GLchar *name, const glm::vec3 &value, GLboolean useShader = false);
	void setVector4f(const GLchar *name, GLfloat x, GLfloat y, GLfloat z, GLfloat w, GLboolean useShader = false);
	void setVector4f(const GLchar *name, const glm::vec4 &value, GLboolean useShader = false);
	void setMatrix4(const GLchar *name, const glm::mat4 &matrix, GLboolean useShader = false);
	void setMatrix4(const GLchar *name, float* matrix, GLboolean useShader = false);

	// Checks if compilation or linking failed and if so, print the error logs
	void checkCompileErrors(GLuint object, std::string type);

private:

	GLuint m_vertexShader;
	GLuint m_fragmentShader;
	GLuint m_geometryShader;

	std::string m_vertexSourcePath;
	std::string m_fragmentSourcePath;
	std::string m_geometrySourcePath;

	void addShader(const GLchar* SourceFilePath, GLuint SHADER_TYPE);

	// Gets the type of shader as a string
	std::string getTypeAsString(GLuint ShaderType);

	void logErrors(std::string sourceFilePath, GLuint shaderID, GLuint shaderType);
};
#endif