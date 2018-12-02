#define GLM_ENABLE_EXPERIMENTAL
#ifndef _____GRAPHICS_H_____
#define _____GRAPHICS_H_____

#include <string>

#include <GL/glew.h>
#include <SDL2/SDL_opengl.h>
#include <SDL2/SDL.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include "Globals.h"
#include "ResourceManager/ResourceManager.h"
#include "UI/TextRenderer.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#define BUFFER_OFFSET( offset )   ((GLvoid*) (offset))

class Graphics
{
public:
	

	void init(void);
	void display(void);
	void idle(void);

	int generate2dRectangleColor(int x, int y, GLfloat z, int height, int width, glm::vec4 color);
	int generate2dRectangleColorCorners(int x, int y, GLfloat z, int height, int width, glm::vec4 color_tl, glm::vec4 color_bl, glm::vec4 color_tr, glm::vec4 color_br);
	int generate2dRectangleTexture(int x, int y, GLfloat z, int height, int width, std::string texture_path);
	void recolor2dRectangle(int index, glm::vec4 color);
	void recolorCorners2dRectangle(int index, glm::vec4 color_tl, glm::vec4 color_bl, glm::vec4 color_tr, glm::vec4 color_br);
	//void retexture2dRectangle(int index);
	void remove2dRectangle(int index);

	void rotateRandom(void);

	// Adds text to render to the screen.
	// You will need to do this every tick for it to continuously display 
	void addTextToRender(RenderableText text);
	void addTextsToRender(std::vector<RenderableText> texts);

	glm::vec4* cone(int *num_vertices);
	glm::vec4* genRandomTriangleColors(int num_vertices);
	TextRenderer textRenderer;
private:
	int num_vertices = 0;
	GLuint vPosition;
	GLuint vColor;
	GLuint vTexCoords;
	glm::vec4 *vertices;
	glm::vec4 *colors;
	glm::vec2 *textures;
	glm::mat4 ctm = { { 1.0, 0.0, 0.0, 0.0 },{ 0.0, 1.0, 0.0, 0.0 },{ 0.0, 0.0, 1.0, 0.0 },{ 0.0, 0.0, 0.0, 1.0 } };
	glm::vec3 randomRotationAxis = { 0.0, 0.0, 0.0 };
	// Vertices for a simple 1x1 textured quad
	glm::vec4 quadVertices[6] = {
		{ -1.0f, 1.0f, 0.0f, 1.0f },
		{ -1.0f, -1.0f, 0.0f, 1.0f },
		{ 1.0f, -1.0f, 0.0f, 1.0f },
		{ -1.0f, 1.0f, 0.0f, 1.0f },
		{ 1.0f, -1.0f, 0.0f, 1.0f },
		{ 1.0f, 1.0f, 0.0f, 1.0f }
	};
	glm::vec2 quadTexCoords[6] = {
		{ 0.0f, 0.0f },
		{ 0.0f, 1.0f },
		{ 1.0f, 1.0f },
		{ 0.0f, 0.0f },
		{ 1.0f, 1.0f },
		{ 1.0f, 0.0f }
	};
	std::vector<RenderableText> m_textToRender; // Stores the text to render for the current tick. Cleared after flushing to screen.
};

#endif
