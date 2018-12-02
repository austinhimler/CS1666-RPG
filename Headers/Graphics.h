#define GLM_ENABLE_EXPERIMENTAL
#ifndef _____GRAPHICS_H_____
#define _____GRAPHICS_H_____

#include <string>
#include <list>

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

class GraphicsObject
{
public:
	//int index;
	int type; //0 = 3d object color, 1 = 3d object texture
	GLuint VAO;
	int num_vertices;
	GLfloat x;
	GLfloat y;
	GLfloat z;
	glm::vec4 color;
	std::string texture_ID;
	int texture_sheet_x;
	int texture_sheet_y;
	int texture_sheet_size_x;
	int texture_sheet_size_y;
	glm::vec4* position_array;
	glm::vec4* color_array;
	glm::vec2* texture_array;
	glm::mat4 ctm = { { 1.0, 0.0, 0.0, 0.0 },{ 0.0, 1.0, 0.0, 0.0 },{ 0.0, 0.0, 1.0, 0.0 },{ 0.0, 0.0, 0.0, 1.0 } };
private:
};

class Graphics
{
public:
	void init(void);
	void display(void);
	void idle(void);

	void genQuadColor(int x, int y, GLfloat z, int height, int width, glm::vec4 color);
	void genQuadTexture(int x, int y, GLfloat z, int height, int width, const GLchar *file, std::string texture_ID, int texture_sheet_x, int texture_sheet_y, int texture_sheet_size_x, int texture_sheet_size_y);
	void recolorQuad(int index, glm::vec4 color);
	void retextureQuad(int index);
	void removeObject(int index);

	void rotateRandom(void);

	// Adds text to render to the screen.
	// You will need to do this every tick for it to continuously display 
	void addTextToRender(RenderableText text);
	void addTextsToRender(std::vector<RenderableText> texts);

	glm::vec4* cone(); //Redo with new GraphicsObject
	glm::vec4* sphere(GLfloat radius, GLfloat resolution, GLfloat x, GLfloat y, GLfloat z); //Redo with new GraphicsObject
	glm::vec4* cube(GLfloat scale, GLfloat x, GLfloat y, GLfloat z); //Redo with new GraphicsObject
	glm::vec4* genRandomTriangleColors(); //Redo with new GraphicsObject
	glm::vec4* genRandomTriangleColorsSimilar(glm::vec4 color); //Redo with new GraphicsObject
	TextRenderer textRenderer;
private:
	int num_vertices = 0; //CLEANUP
	GLuint buffer;
	GLuint vPosition;
	GLuint vColor;
	GLuint vTexCoords;
	std::list<GraphicsObject> objectList;
	glm::vec4 *vertices; //CLEANUP
	glm::vec4 *colors; //CLEANUP
	glm::vec2 *textures; //CLEANUP
	glm::mat4 ctm = { { 1.0, 0.0, 0.0, 0.0 },{ 0.0, 1.0, 0.0, 0.0 },{ 0.0, 0.0, 1.0, 0.0 },{ 0.0, 0.0, 0.0, 1.0 } }; //CLEANUP
	glm::vec3 randomRotationAxis = { 0.0, 0.0, 0.0 };
	// Vertices for a simple 1x1 textured quad
	int quad_num_vertices = 6;
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
