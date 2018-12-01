
#ifndef _____TEXT_RENDERER_H_____
#define _____TEXT_RENDERER_H_____

#include <string>
#include <glm/vec2.hpp>
#include <glm/vec4.hpp>
#include <GL/glew.h>
#include "../ResourceManager/ResourceManager.h"

// Represents text that will be rendered to the screen
struct RenderableText
{
	std::string Text;
	glm::vec2 Position;
	float Scale;
	glm::vec4 Color;
	FontData &Font;
	
	RenderableText()
		: Text("Uninitialised Text")
		, Position(glm::vec2(0.0, 0.0))
		, Scale(1.0f)
		, Color(glm::vec4(0.0, 0.0, 0.0, 1.0))
		, Font(ResourceManager::getDefaultFontData())
	{
	}

	RenderableText(std::string text, glm::vec2 position, float scale, glm::vec4 color, FontData& font)
		: Text(text)
		, Position(position)
		, Scale(scale)
		, Color(color)
		, Font(font)
	{
	}
};

// Handles rendering text to the screen in OpenGL
class TextRenderer {
public:

	TextRenderer() {};

	// Intialises the text renderer
	void Initialize(int screenWidth, int screenHeight);

	// Renders text to the screen
	void RenderText(RenderableText text);

private:

	GLuint m_VBO;	// The vertex buffer object we use for the text rendering. This is updated dynamically.
	GLuint m_VAO;	// The vertex array object we use for the text rendering
};
#endif