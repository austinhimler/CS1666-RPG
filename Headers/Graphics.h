#ifndef _____GRAPHICS_H_____
#define _____GRAPHICS_H_____

#include <glew/glew.h>
#include <SDL/SDL_opengl.h>
#include <glm/glm.hpp>
#include "Shader.h"


#define BUFFER_OFFSET( offset )   ((GLvoid*) (offset))

int num_vertices;
GLuint ctm_location;

class Graphics
{
public:
	void init(void);
};

#endif
