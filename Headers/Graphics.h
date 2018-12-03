#define GLM_ENABLE_EXPERIMENTAL
#ifndef _____GRAPHICS_H_____
#define _____GRAPHICS_H_____

#include <string>
#include <list>
#include <algorithm>

#include <GL/glew.h>
#include <SDL2/SDL_opengl.h>
#include <SDL2/SDL.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/string_cast.hpp>
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
	GLuint VBO;
	int num_vertices;
	glm::vec4 color;
	std::string texture_ID;
	int texture_sheet_it;
	int texture_sheet_size;
	glm::vec4* position_array;
	glm::vec4* color_array;
	glm::vec2* texture_array;
	glm::mat4 ctm = { { 1.0, 0.0, 0.0, 0.0 },{ 0.0, 1.0, 0.0, 0.0 },{ 0.0, 0.0, 1.0, 0.0 },{ 0.0, 0.0, 0.0, 1.0 } };
	int idle_type = 0; //0 = no animation, 1 = sprite animation, 2 = motion animation, 3 = sprite and motion animation
	glm::mat4 idle_motion = { { 1.0, 0.0, 0.0, 0.0 },{ 0.0, 1.0, 0.0, 0.0 },{ 0.0, 0.0, 1.0, 0.0 },{ 0.0, 0.0, 0.0, 1.0 } };
private:
};

class Graphics
{
public:
	void init(void);
	void display(void);
	void idle(void);

	int genQuadColor(int height, int width, glm::vec4 color);
	int genQuadTexture(int height, int width, const GLchar *file, std::string texture_ID, int texture_sheet_it, int texture_sheet_size);
	int genCone(GLfloat radius, GLfloat height, int resolution, int color_type, glm::vec4 color);
	int genSphere(GLfloat radius, int resolution, int color_type, glm::vec4 color);
	int genCube(int color_type, glm::vec4 color);
	
	//Color Options
	glm::vec4* genRandomTriangleColors(int num_vertices); //color_type == 0
	glm::vec4* genRandomTriangleColorsSimilar(int num_vertices, glm::vec4 color); //color_type == 1

	int recolorQuad(int ID, glm::vec4 color);
	int retextureQuad(int ID, const GLchar *file, std::string texture_ID);
	int removeObject(int ID);

	int translateObjectByPixel(int ID, int x, int y, GLfloat z);
	void iterateSpriteAnimation(std::list<GraphicsObject>::iterator it);

	glm::vec3 rotateRandom(void);

	int setIdleType(int ID, int type);
	int setIdleMotion(int ID, glm::mat4 motion);

	// Adds text to render to the screen.
	// You will need to do this every tick for it to continuously display 
	void addTextToRender(RenderableText text);
	void addTextsToRender(std::vector<RenderableText> texts);

	

	TextRenderer textRenderer;
private:
	int object_counter = 0;
	GLuint HUDVAO;
	GLuint HUDVBO;
	GLuint vPosition;
	GLuint vColor;
	GLuint vTexCoords;
	std::list<GraphicsObject> objectList;
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
