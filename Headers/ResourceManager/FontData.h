
#ifndef _____FONTDATA_H_____
#define _____FONTDATA_H_____

#include <GL/glew.h>
#include <glm/vec2.hpp>
#include <map>

// FontData is able to store the required information for displaying a font
class FontData
{
public:

	// Represents a single character in the font library
	struct Character {
		GLuint     TextureID;  // ID handle of the character texture
		glm::ivec2 Size;       // Size of a single character
		glm::ivec2 Bearing;    // Offset from baseline to left/top of a single character
		GLuint     Advance;    // Offset to advance to next character
	};

	// Constructor
	FontData() {};
	~FontData() {};

	void SetCharacterMap(std::map<GLchar, Character> map);

	// Returns a reference to the character map
	std::map<GLchar, Character>& GetCharacterMap();

private:

	// Map of all the characters in the font 
	std::map<GLchar, Character> m_characterMap;
};
#endif