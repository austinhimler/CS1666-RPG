
#ifndef _____RESOURCE_MANAGER_H_____
#define _____RESOURCE_MANAGER_H_____

#include <map>
#include <string>
#include "GL/glew.h"
#include "Shader.h"
#include "Texture2D.h"
#include "FontData.h"
#include <map>
#include <vector>

// SOIL used for image loading
#include <SOIL2/SOIL2.h>

// Freetype used for loading fonts
#include <ft2build.h>
#include <freetype/freetype.h>

// A static ResourceManager class that manages resources for the game
class ResourceManager
{
public:
    // Resource storage
    static std::map<std::string, Shader> Shaders;
	static std::map<std::string, Texture2D> Textures;
	static std::map<std::string, FontData> Fonts;

    // Initialise the resource manager with defaults that are used when an unknown ID is provided.
    static void initialise();

    // Loads (and generates) a shader program from file loading vertex, fragment (and geometry) shader's source code.
	// If gShaderFile is not nullptr, it also loads a geometry shader
    static Shader& loadShader(const GLchar *vShaderFile, const GLchar *fShaderFile, const GLchar *gShaderFile, std::string ID);

	// Loads (and generates) a texture from file
	static Texture2D& loadTexture(const GLchar *file, std::string ID);

	// Loads a font from file
	static FontData& loadFont(const GLchar *file, std::string ID);

	// Retrieves a stored shader
	// If an unknown ID is passed in as a parameter, a default shader is returned
	static Shader& getShader(std::string ID);

	// Retrieves a stored texture
    // If an unknown ID is passed in as a parameter, a default texture is returned
	static Texture2D& getTexture(std::string ID);

	// Retrieves a stored font
	// If an unknown ID is passed in as a parameter, a default font is returned
	static FontData& getFontData(std::string ID);

	// Returns the default font to use
	static FontData& getDefaultFontData();

	// Properly de-allocates all loaded resources
	static void	release();

private:
    // Private constructor, that is we do not want any actual resource manager objects. Its members and functions should be publicly available (static).
    ResourceManager() { }

    // Loads and generates a shader from file
    static Shader loadShaderFromFile(const GLchar *vShaderFile, const GLchar *fShaderFile, const GLchar *gShaderFile = nullptr);

	// Loads and generates a texture from file
	static Texture2D loadTextureFromFile(const GLchar *file);

	// Loads and generates a font from file
	static FontData loadFontFromFile(const GLchar *file);

	// Reads a file into a string
	// Returns true if successfull, returns false if unsuccessful
	static bool loadFileIntoString(const char *filepath, std::string &str);
};
#endif

