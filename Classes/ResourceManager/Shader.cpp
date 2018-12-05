#include "../../Headers/ResourceManager/Shader.h"
#include <glm/vec2.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

Shader::Shader()
    : m_vertexShader(0)
    , m_fragmentShader(0)
    , m_geometryShader(0)
{
    this->Program = glCreateProgram();
}

void Shader::release()
{
}

void Shader::addVertexShader(const GLchar* vertexSourcePath)
{
    addShader(vertexSourcePath, SHADER_TYPE_VERTEX);
}
void Shader::addFragmentShader(const GLchar* fragmentSourcePath)
{
    addShader(fragmentSourcePath, SHADER_TYPE_FRAGMENT);
}
void Shader::addGeometryShader(const GLchar* geometrySourcePath)
{
    addShader(geometrySourcePath, SHADER_TYPE_GEOMETRY);
}

void Shader::addShader(const GLchar* SourceFilePath, GLuint shader_type)
{
    if (SourceFilePath == NULL)
        return;

    GLint success;
    GLuint shaderPart;

    if (shader_type == SHADER_TYPE_VERTEX)
    {
        shaderPart = glCreateShader(GL_VERTEX_SHADER);
    }
    else if (shader_type == SHADER_TYPE_FRAGMENT)
    {
        shaderPart = glCreateShader(GL_FRAGMENT_SHADER);
    }
    else if (shader_type == SHADER_TYPE_GEOMETRY)
    {
        shaderPart = glCreateShader(GL_GEOMETRY_SHADER);
    }
    else
    {
        std::cout << "ERROR - Unknown Shader type\n";
    }

    glShaderSource(shaderPart, 1, &SourceFilePath, NULL);
    glCompileShader(shaderPart);
    // Print compile errors if any
    glGetShaderiv(shaderPart, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        logErrors(SourceFilePath, shaderPart, shader_type);
        
		// Exit the program. Up to you if you want to continue with a broken shader. 
		exit(EXIT_FAILURE);
        return;
    };

    glAttachShader(this->Program, shaderPart);
    glLinkProgram(this->Program);

    // Print linking errors if any
    glGetProgramiv(this->Program, GL_LINK_STATUS, &success);
    if (!success)
    {
        GLchar infoLog[512];
        glGetProgramInfoLog(this->Program, 512, NULL, infoLog);
        std::cout << "ERROR - " << getTypeAsString(shader_type) << " - LINKING FAILED" << "\n" << infoLog << std::endl;
        return;
    }

    // Delete the shader part as they're linked into our program now and no longer necessery
    glDeleteShader(shaderPart);
}

void Shader::compile(const GLchar* vertexSourcePath, const GLchar* fragmentSourcePath, const GLchar* geometrySourcePath)
{
    GLint success;
    GLchar infoLog[512];

    m_vertexSourcePath = std::string(vertexSourcePath);
    m_fragmentSourcePath = std::string(fragmentSourcePath);

    // Vertex Shader
    m_vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(m_vertexShader, 1, &vertexSourcePath, NULL);
    glCompileShader(m_vertexShader);
    // Print compile errors if any
    glGetShaderiv(m_vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        logErrors(vertexSourcePath, m_vertexShader, SHADER_TYPE_VERTEX);
    };

    // Fragment Shader
    m_fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(m_fragmentShader, 1, &fragmentSourcePath, NULL);
    glCompileShader(m_fragmentShader);
    // Print compile errors if any
    glGetShaderiv(m_fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        logErrors(fragmentSourcePath, m_fragmentShader, SHADER_TYPE_FRAGMENT);
    };


    // Geometry Shader
    if (geometrySourcePath != nullptr)
    {
        m_geometrySourcePath = std::string(geometrySourcePath);
        m_geometryShader = glCreateShader(GL_GEOMETRY_SHADER);
        glShaderSource(m_geometryShader, 1, &geometrySourcePath, NULL);
        glCompileShader(m_geometryShader);
        // Print compile errors if any
        glGetShaderiv(m_geometryShader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            logErrors(geometrySourcePath, m_geometryShader, SHADER_TYPE_GEOMETRY);
        };
    }

    // Shader Program
    this->Program = glCreateProgram();
    glAttachShader(this->Program, m_vertexShader);
    glAttachShader(this->Program, m_fragmentShader);
    if (geometrySourcePath != nullptr)
        glAttachShader(this->Program, m_geometryShader);
    glLinkProgram(this->Program);
    // Print linking errors if any
    glGetProgramiv(this->Program, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(this->Program, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }

    // Delete the shaders as they're linked into our program now and no longer necessary
    glDeleteShader(m_vertexShader);
    glDeleteShader(m_fragmentShader);
    if (geometrySourcePath != nullptr)
        glDeleteShader(m_geometryShader);
}

void Shader::setFloat(const GLchar *name, GLfloat value, GLboolean useShader)
{
    if (useShader)
        this->use();
    glUniform1f(glGetUniformLocation(this->Program, name), value);
}

void Shader::setInteger(const GLchar *name, GLint value, GLboolean useShader)
{
    if (useShader)
        this->use();
    glUniform1i(glGetUniformLocation(this->Program, name), value);
}

void Shader::setBoolean(const GLchar *name, GLboolean value, GLboolean useShader)
{
    if (useShader)
        this->use();
    glUniform1i(glGetUniformLocation(this->Program, name), value);
}

void Shader::setVector2f(const GLchar *name, GLfloat x, GLfloat y, GLboolean useShader)
{
    if (useShader)
        this->use();
    glUniform2f(glGetUniformLocation(this->Program, name), x, y);
}
void Shader::setVector2f(const GLchar *name, const glm::vec2 &value, GLboolean useShader)
{
    if (useShader)
        this->use();
    glUniform2f(glGetUniformLocation(this->Program, name), value.x, value.y);
}

void Shader::setVector3f(const GLchar *name, GLfloat x, GLfloat y, GLfloat z, GLboolean useShader)
{
    if (useShader)
        this->use();
    glUniform3f(glGetUniformLocation(this->Program, name), x, y, z);
}
void Shader::setVector3f(const GLchar *name, const glm::vec3 &value, GLboolean useShader)
{
    if (useShader)
        this->use();
    glUniform3f(glGetUniformLocation(this->Program, name), value.x, value.y, value.z);
}

void Shader::setVector4f(const GLchar *name, GLfloat x, GLfloat y, GLfloat z, GLfloat w, GLboolean useShader)
{
    if (useShader)
        this->use();
    glUniform4f(glGetUniformLocation(this->Program, name), x, y, z, w);
}

void Shader::setVector4f(const GLchar *name, const glm::vec4 &value, GLboolean useShader)
{
    if (useShader)
        this->use();
    glUniform4f(glGetUniformLocation(this->Program, name), value.x, value.y, value.z, value.w);
}

void Shader::setMatrix4(const GLchar *name, const glm::mat4 &matrix, GLboolean useShader)
{
    if (useShader)
        this->use();
    glUniformMatrix4fv(glGetUniformLocation(this->Program, name), 1, GL_FALSE, glm::value_ptr(matrix));
}

void Shader::setMatrix4(const GLchar *name, float* mat, GLboolean useShader)
{
    if (useShader)
        this->use();
    glUniformMatrix4fv(glGetUniformLocation(this->Program, name), 1, GL_FALSE, &mat[0]);
}

void Shader::checkCompileErrors(GLuint object, std::string type)
{
    GLint success;
    GLchar infoLog[1024];
    if (type != "PROGRAM")
    {
        glGetShaderiv(object, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(object, 1024, NULL, infoLog);
            std::cout << "| ERROR::SHADER: Compile-time error: Type: " << type << std::endl
                << "-------------------------------------------------------"
                << std::endl << infoLog << std::endl;
        }
    }
    else
    {
        glGetProgramiv(object, GL_LINK_STATUS, &success);
        if (!success)
        {
            glGetProgramInfoLog(object, 1024, NULL, infoLog);
            std::cout << "| ERROR::Shader: Link-time error: Type: " << type << std::endl
                << "-------------------------------------------------------"
                << std::endl << infoLog << std::endl;
        }
    }
}


std::string Shader::getTypeAsString(GLuint ShaderType)
{
    if (ShaderType == SHADER_TYPE_VERTEX)
    {
        return "SHADER_TYPE_VERTEX";
    }
    else if (ShaderType == SHADER_TYPE_FRAGMENT)
    {
        return "SHADER_TYPE_FRAGMENT";
    }
    else if (ShaderType == SHADER_TYPE_GEOMETRY)
    {
        return "SHADER_TYPE_GEOMETRY";
    }
    else
    {
        return "SHADER_TYPE_UNKNOWN";
    }
}

void Shader::logErrors(std::string sourceFilePath, GLuint shaderID, GLuint shaderType)
{
    GLchar infoLog[512];

    glGetShaderInfoLog(shaderID, 512, NULL, infoLog);
    std::cout << "\n-----\n" << "ERROR - " << getTypeAsString(shaderType) << " - COMPILATION FAILED" << "\n------\n";
    std::cout << "OpenGL InfoLog contains: \n" << infoLog << "\n";

    std::string str(infoLog);
    size_t openBracketPos = str.find("(");
    size_t closeBracketPos = str.find(")");
    std::string bracketContents = str.substr(openBracketPos + 1, (closeBracketPos - openBracketPos) - 1);
    // convert to int

    int errorLineNumber; //number which will contain the result

    std::stringstream convert(bracketContents); // stringstream used for the conversion initialized with the contents of Text

    if (!(convert >> errorLineNumber))//give the value to Result using the characters in the string
        errorLineNumber = 0;


    std::cout << "Sample from code containing error: " << "\n--------------------------------------------\n" << std::endl;

    std::istringstream iss(sourceFilePath);

    std::string line;

    const int SAMPLE_SIZE = 5;
    int currentLineNum = 1;
    while (std::getline(iss, line))
    {
        std::stringstream ss;
        ss << currentLineNum;
        std::string lineNumString = ss.str();

        if (currentLineNum >= (errorLineNumber - SAMPLE_SIZE))
            std::cout << lineNumString << "\t" << line << std::endl;

        if (currentLineNum >= (errorLineNumber + SAMPLE_SIZE))
            break;

        currentLineNum++;
    }

    std::cout << "\n--------------------------------------------\n" << std::endl;
}

