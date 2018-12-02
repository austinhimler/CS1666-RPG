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

	int genQuadColor(int x, int y, GLfloat z, int height, int width, glm::vec4 color);
	int genQuadTexture(int x, int y, GLfloat z, int height, int width, std::string texture_path);
	void recolorQuad(int index, glm::vec4 color);
	void retextureQuad(int index);
	void removeObject(int index);

	void rotateRandom(void);

	// Adds text to render to the screen.
	// You will need to do this every tick for it to continuously display 
	void addTextToRender(RenderableText text);
	void addTextsToRender(std::vector<RenderableText> texts);

	glm::vec4* cone();
	glm::vec4* sphere(GLfloat radius, GLfloat resolution, GLfloat x, GLfloat y, GLfloat z);
	glm::vec4* cube(GLfloat scale, GLfloat x, GLfloat y, GLfloat z);
	glm::vec4* genRandomTriangleColors();
	glm::vec4* genRandomTriangleColorsSimilar(glm::vec4 color);
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
