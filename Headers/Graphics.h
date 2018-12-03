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
	int ID;
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

	int genQuadColor(int x, int y, GLfloat z, int height, int width, glm::vec4 color);
	int genQuadTexture(int x, int y, GLfloat z, int height, int width, const GLchar *file, std::string texture_ID, int texture_sheet_x, int texture_sheet_y, int texture_sheet_size_x, int texture_sheet_size_y);
	void recolorQuad(int ID, glm::vec4 color);
	void retextureQuad(int ID);
	void removeObject(int ID);

	void rotateRandom(void);

	// Adds text to render to the screen.
	// You will need to do this every tick for it to continuously display 
	void addTextToRender(RenderableText text);
	void addTextsToRender(std::vector<RenderableText> texts);

	int genCone(GLfloat radius, GLfloat height, GLfloat resolution, int color_type, glm::vec4 color);
	int genSphere(GLfloat radius, GLfloat resolution, int color_type, glm::vec4 color);
	int genCube(int color_type, glm::vec4 color);
	//Color Options
	glm::vec4* genRandomTriangleColors(int num_vertices); //color_type == 0
	glm::vec4* genRandomTriangleColorsSimilar(int num_vertices, glm::vec4 color);//color_type == 1

	TextRenderer textRenderer;
private:
	int object_counter = 0;
	GLuint buffer;
	GLuint vPosition;
	GLuint vColor;
	GLuint vTexCoords;
	std::list<GraphicsObject> objectList;
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
