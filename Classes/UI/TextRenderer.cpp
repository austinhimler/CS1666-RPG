
#include "../../Headers/UI/TextRenderer.h"
#include <glm/gtc/matrix_transform.hpp>

void TextRenderer::Initialize(int screenWidth, int screenHeight)
{
	// Set up the vertex array object for the text
	glGenVertexArrays(1, &m_VAO);
	glGenBuffers(1, &m_VBO);
	glBindVertexArray(m_VAO);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, NULL, GL_DYNAMIC_DRAW); // Set to use dynamic draw because we will be updating the buffer on the fly
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	// Load the text shader
	auto shader = ResourceManager::loadShader("Shaders/textShader.vs", "Shaders/textShader.frag", nullptr, "TextRenderer_shader");

	// Set up the projection
	glm::mat4 projection = glm::ortho(0.0f, (float)screenWidth, 0.0f, (float)screenHeight);
	shader.setMatrix4("projection", projection, true);
}

void TextRenderer::RenderText(RenderableText renderText)
{
	// Set the color in the shader
	auto shader = ResourceManager::getShader("TextRenderer_shader");
	shader.setVector4f("textColor", renderText.Color, true);
	glActiveTexture(GL_TEXTURE0);
	glBindVertexArray(m_VAO);

	auto& map = renderText.Font.GetCharacterMap();
	glm::vec2 currentPos = renderText.Position;

	// Iterate through all characters
	std::string::const_iterator c;
	for (c = renderText.Text.begin(); c != renderText.Text.end(); c++)
	{
		// Get the character using our lookup map
		FontData::Character character = map[*c];

		GLfloat xpos = currentPos.x + character.Bearing.x * renderText.Scale;
		GLfloat ypos = currentPos.y - (character.Size.y - character.Bearing.y) * renderText.Scale;

		GLfloat width = character.Size.x * renderText.Scale;
		GLfloat height = character.Size.y * renderText.Scale;

		// Update the VBO for this character
		GLfloat vertices[6][4] = {
			{ xpos, ypos + height, 0.0, 0.0 },
			{ xpos, ypos, 0.0, 1.0 },
			{ xpos + width, ypos, 1.0, 1.0 },

			{ xpos, ypos + height, 0.0, 0.0 },
			{ xpos + width, ypos, 1.0, 1.0 },
			{ xpos + width, ypos + height, 1.0, 0.0 }
		};

		// Render the character texture over the quad
		glBindTexture(GL_TEXTURE_2D, character.TextureID);

		// Update the content of VBO
		glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		// Render the quad
		glDrawArrays(GL_TRIANGLES, 0, 6);

		// Now advance to the next character (note that advance is number of 1/64 pixels)
		currentPos.x = currentPos.x + (character.Advance >> 6) * renderText.Scale; // Bitshift by 6 to get value in pixels (2^6 = 64)
	}
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}