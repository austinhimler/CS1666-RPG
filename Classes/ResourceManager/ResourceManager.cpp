
#include "../../Headers/ResourceManager/ResourceManager.h"

// SOIL used for image loading
#include "../../dev_lib/includes/SOIL2/SOIL2.h"

// Freetype used for loading fonts
#include "../../dev_lib/includes/ft2build.h"
#include "../../dev_lib/includes/freetype/freetype.h"

// Instantiate static variables
std::map<std::string, Shader> ResourceManager::Shaders;
std::map<std::string, Texture2D> ResourceManager::Textures;
std::map<std::string, FontData> ResourceManager::Fonts;

// Resource Manager Default ID's
// We use these when an ID is passed to reference manager that it is unaware of
const std::string UNKNOWN_ID = "DEFAULT";

void ResourceManager::initialise()
{
	// Shader provided when unknown ID given is a shader that just renders red
    Shaders[UNKNOWN_ID] = loadShaderFromFile("Shaders/NO_SHADER.vs", "Shaders/NO_SHADER.frag", nullptr);
	// Texture provided when unknown ID given is a texture that has the words NO TEXTURE written on it
	Textures[UNKNOWN_ID] = loadTextureFromFile("Images/ResourceManager/NO_TEXTURE.png");
	// Font provided when an unknown ID is given is arial
	Fonts[UNKNOWN_ID] = loadFontFromFile("Fonts/arial.ttf");
}

Shader& ResourceManager::loadShader(const GLchar *vShaderFile, const GLchar *fShaderFile, const GLchar *gShaderFile, std::string ID)
{
    Shaders[ID] = loadShaderFromFile(vShaderFile, fShaderFile, gShaderFile);
    return Shaders[ID];
}

Texture2D& ResourceManager::loadTexture(const GLchar *file, std::string ID)
{
	if (Textures.find(ID) != Textures.end())
	{
		// Just return a reference if this texture is already loaded
		return Textures[ID];
	}
	Textures[ID] = loadTextureFromFile(file);
	return Textures[ID];
}

FontData& ResourceManager::loadFont(const GLchar *file, std::string ID)
{
	if (Fonts.find(ID) != Fonts.end())
	{
		// Just return a reference if this font is already loaded
		return Fonts[ID];
	}
	Fonts[ID] = loadFontFromFile(file);
	return Fonts[ID];
}

Shader& ResourceManager::getShader(std::string ID)
{
    std::map<std::string, Shader>::iterator it;
    it = Shaders.find(ID);
    if (it != Shaders.end())
        return Shaders[ID];
    else
    {
        return Shaders[UNKNOWN_ID];
    }
}

Texture2D& ResourceManager::getTexture(std::string ID)
{
	std::map<std::string, Texture2D>::iterator it;
	it = Textures.find(ID);
	if (it != Textures.end())
		return Textures[ID];
	else
	{
		return Textures[UNKNOWN_ID];
	}
}

FontData& ResourceManager::getFontData(std::string ID)
{
	std::map<std::string, FontData>::iterator it;
	it = Fonts.find(ID);
	if (it != Fonts.end())
		return Fonts[ID];
	else
	{
		return Fonts[UNKNOWN_ID];
	}
}

FontData& ResourceManager::getDefaultFontData()
{
	return Fonts[UNKNOWN_ID];
}

void ResourceManager::release()
{
    // Release all shaders	
    for (auto iter : Shaders)
        glDeleteProgram(iter.second.Program);

	// Delete all textures
	for (auto iter : Textures)
		glDeleteTextures(1, &iter.second.ID);
}

Shader ResourceManager::loadShaderFromFile(const GLchar *vShaderFile, const GLchar *fShaderFile, const GLchar *gShaderFile)
{
    // Create the shader
    Shader shader;

    if (gShaderFile != nullptr)
        std::cout << "Loading Shader: " << vShaderFile << ", " << fShaderFile << ", " << gShaderFile << std::endl;
    else
        std::cout << "Loading Shader: " << vShaderFile << ", " << fShaderFile << std::endl;


    // Compile vertex shader
    std::string vertexSource;
	if (!loadFileIntoString(vShaderFile, vertexSource))
	{
		std::cout << "ERROR::SHADER: Failed to read vertex shader file: " << vShaderFile  << std::endl;
		// Return the default shader, no point continuining compilation
		return Shaders[UNKNOWN_ID];
	}

    // Link to shader
    shader.addVertexShader(vertexSource.c_str());

    std::string fragmentSource;
	if (!loadFileIntoString(fShaderFile, fragmentSource))
	{
		std::cout << "ERROR::SHADER: Failed to read fragment shader file: " << fShaderFile << std::endl;
		// Return the default shader, no point continuining compilation
		return Shaders[UNKNOWN_ID];
	}
    shader.addFragmentShader(fragmentSource.c_str());

    if (gShaderFile != NULL)
    {
        std::string geometrySource;
		if (!loadFileIntoString(gShaderFile, geometrySource))
		{
			std::cout << "ERROR::SHADER: Failed to read geometry shader file: " << gShaderFile << std::endl;
			// Return the default shader, no point continuining compilation
			return Shaders[UNKNOWN_ID];
		}
        shader.addGeometryShader(geometrySource.c_str());
    }

    return shader;
}

Texture2D ResourceManager::loadTextureFromFile(const GLchar *file)
{
	// Create Texture object
	Texture2D texture;

	// Output to log
	std::cout << "Loading Texture: " << file << std::endl;

	// load texture
	int width, height, channels;
	unsigned char* textureData = SOIL_load_image(file, &width, &height, &channels, SOIL_LOAD_AUTO);

	texture.wrap_S = GL_REPEAT;
	texture.wrap_T = GL_REPEAT;

	// Base formats on the number of channels in the image
	if (channels == 2)
	{
		texture.internal_format = GL_LUMINANCE;
		texture.image_format = GL_LUMINANCE;
	}
	if (channels == 3)
	{
		texture.internal_format = GL_RGB;
		texture.image_format = GL_RGB;
	}
	else if (channels == 4)
	{
		texture.internal_format = GL_RGBA;
		texture.image_format = GL_RGBA;
	}
	texture.generate(width, height, textureData);

	// free data
	SOIL_free_image_data(textureData);

	return texture;
}

FontData ResourceManager::loadFontFromFile(const GLchar *file)
{
	// Create Font
	FontData font;

	// Create map
	std::map<GLchar, FontData::Character> characterMap;

	// Output to log
	std::cout << "Loading Font: " << file << std::endl;

	FT_Library ft;
	if (FT_Init_FreeType(&ft))
	{
		std::cout << "ERROR::FREETYPE: Could not init FreeType Library. Default font will be used instead." << std::endl;
		// No point continuing
		return Fonts[UNKNOWN_ID];
	}

	FT_Face face;
	if (FT_New_Face(ft, file, 0, &face))
	{
		std::cout << "ERROR::FREETYPE: Failed to load font. Default font will be used instead." << std::endl;
		// No point continuing
		return Fonts[UNKNOWN_ID];
	}

	FT_Set_Pixel_Sizes(face, 0, 48);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // Disable byte-alignment restriction

	for (GLubyte c = 0; c < 128; c++)
	{
		// Load character glyph 
		if (FT_Load_Char(face, c, FT_LOAD_RENDER))
		{
			std::cout << "ERROR::FREETYTPE: Failed to load character" << std::endl;
			continue;
		}
		// Generate texture for the font
		GLuint texture;
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexImage2D(
			GL_TEXTURE_2D,
			0,
			GL_RED,
			face->glyph->bitmap.width,
			face->glyph->bitmap.rows,
			0,
			GL_RED,
			GL_UNSIGNED_BYTE,
			face->glyph->bitmap.buffer
		);

		// Set texture options
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		FontData::Character character = 
		{
			texture,
			glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
			glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
			face->glyph->advance.x
		};

		characterMap.insert(std::pair <GLchar, FontData::Character> (c, character));
	}

	// Clear 
	FT_Done_Face(face);
	FT_Done_FreeType(ft);

	font.SetCharacterMap(characterMap);

	return font;
}

bool ResourceManager::loadFileIntoString(const char *filepath, std::string &str)
{
	std::string file;
	try
	{
		// Open files
		std::ifstream shaderFile(filepath);

		// Check if shaders were opened
		if (!shaderFile.is_open())
			std::cout << "ERROR - Error opening file: " << filepath << std::endl;

		// Read file's buffer contents into streams
		std::stringstream ss;
		ss << shaderFile.rdbuf();

		// close file handlers
		shaderFile.close();

		// Convert stream into string
		str = ss.str();
		return true;
	}
	catch (std::exception e)
	{
		return false;
	}
}
