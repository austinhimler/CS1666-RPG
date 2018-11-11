#define GLM_ENABLE_EXPERIMENTAL
#ifndef _____GRAPHICS_H_____
#define _____GRAPHICS_H_____

#include <glew.h>
#include <SDL_opengl.h>
#include <SDL.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtx/transform.hpp>
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
	glm::vec3 randomRotationAxis = { 0.0, 0.0, 0.0 };

	void init(void);
	void display(void);
	void idle(void);
	void rotateRandom(void);
	glm::vec4* cone(int *num_vertices);
	glm::vec4* genRandomTriangleColors(int num_vertices);
};

#endif
