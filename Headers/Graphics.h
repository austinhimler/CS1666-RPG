#define GLM_ENABLE_EXPERIMENTAL
#ifndef _____GRAPHICS_H_____
#define _____GRAPHICS_H_____

#include <GL/glew.h>
#include <SDL2/SDL_opengl.h>
#include <SDL2/SDL.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include "Shader.h"
#include "Globals.h"
#include <string>
//To load images
//#include <SOIL/SOIL.h>
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#define BUFFER_OFFSET( offset )   ((GLvoid*) (offset))

class Graphics
{
public:
	GLuint getProgram(void);

	void init(void);
	void display(void);
	void idle(void);

	//void Graphics::loadTexture(string file, int width, int height)
	void generateCombat(glm::vec4 *vert, glm::vec4 *color, glm::vec2 *text);

	int generate2dRectangleColor(int x, int y, GLfloat z, int height, int width, glm::vec4 color);
	int generate2dRectangleColorCorners(int x, int y, GLfloat z, int height, int width, glm::vec4 color_tl, glm::vec4 color_bl, glm::vec4 color_tr, glm::vec4 color_br);
	int generate2dRectangleTexture(int x, int y, GLfloat z, int height, int width, std::string texture_path);
	void recolor2dRectangle(int index, glm::vec4 color);
	void recolorCorners2dRectangle(int index, glm::vec4 color_tl, glm::vec4 color_bl, glm::vec4 color_tr, glm::vec4 color_br);
	//void retexture2dRectangle(int index);
	void remove2dRectangle(int index);

	void rotateRandom(void);
	glm::vec4* cone(int *num_vertices);
	glm::vec4* genRandomTriangleColors(int num_vertices);
private:
	GLuint program;
	GLuint ctm_location;
	int num_vertices = 0;
	glm::vec4 *vertices;
	glm::vec4 *colors;
	glm::vec2 *textures;
	glm::mat4 ctm = { { 1.0, 0.0, 0.0, 0.0 },{ 0.0, 1.0, 0.0, 0.0 },{ 0.0, 0.0, 1.0, 0.0 },{ 0.0, 0.0, 0.0, 1.0 } };
	glm::vec3 randomRotationAxis = { 0.0, 0.0, 0.0 };
	glm::vec4 rectangle[6] = { { -1.0, -1.0, 0.0, 1.0 },
								{ 1.0, 1.0, 0.0, 1.0 },
								{ -1.0, 1.0, 0.0, 1.0 },
								{ -1.0, -1.0, 0.0, 1.0 },
								{ 1.0, -1.0, 0.0, 1.0 },
								{ 1.0, 1.0, 0.0, 1.0 },
							};
	glm::vec2 rectangle_texture[6] = { { -1.0, -1.0 }, { 1.0, 1.0 }, { -1.0, 1.0 },
								{ -1.0, -1.0 }, { 1.0, -1.0 }, { 1.0, 1.0 },
	};
};

#endif
