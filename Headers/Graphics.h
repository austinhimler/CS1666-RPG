#ifndef _____GRAPHICS_H_____
#define _____GRAPHICS_H_____

#include <glew/glew.h>
#include <SDL/SDL_opengl.h>
#include <glm/glm.hpp>
#include "Shader.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#define BUFFER_OFFSET( offset )   ((GLvoid*) (offset))

int num_vertices;
GLuint ctm_location;

class Graphics
{
public:
	void init(void);
	//void rotateRandom(void);
	glm::vec4* cone(int *num_vertices);
	glm::vec4* genRandomTriangleColors(int num_vertices);
};

#endif
