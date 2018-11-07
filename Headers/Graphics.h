#ifndef _____GRAPHICS_H_____
#define _____GRAPHICS_H_____

#include <glew/glew.h>
#include <SDL/SDL_opengl.h>
#include <SDL/SDL.h>
#include <glm/glm.hpp>
#include "Shader.h"
#include "Globals.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#define BUFFER_OFFSET( offset )   ((GLvoid*) (offset))

class Graphics
{
public:
	GLuint ctm_location;
	int num_vertices;
	glm::mat4 ctm = { { 1.0, 0.0, 0.0, 0.0 },{ 0.0, 1.0, 0.0, 0.0 },{ 0.0, 0.0, 1.0, 0.0 },{ 0.0, 0.0, 0.0, 1.0 } };

	void init(void);
	void display(void);
	//void rotateRandom(void);
	glm::vec4* cone(int *num_vertices);
	glm::vec4* genRandomTriangleColors(int num_vertices);
};

#endif
