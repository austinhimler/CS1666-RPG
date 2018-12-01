#include "../Headers/Graphics.h"
#include "time.h"
#include <iostream>

#include "../dev_lib/includes/ft2build.h"
#include "../dev_lib/includes/freetype/freetype.h"
#include "../Headers/ResourceManager/ResourceManager.h"
#include "../Headers/UI/TextRenderer.h"

GLuint textVAO, textVBO;
GLuint coneVAO;
GLuint HUDVAO;

void Graphics::init(void)
{
	// We want to put the following resource manager stuff somewhere earlier in the program.
	// Or alternatively dont continually reconstruct this graphics class

	// Initialise the resource manager
	ResourceManager::initialise();

	// Load shaders
	ResourceManager::loadShader("Shaders/simpleColorShader.vs", "Shaders/simpleColorShader.frag", nullptr, "simple_color_shader");
	ResourceManager::loadShader("Shaders/screenShader.vs", "Shaders/screenShader.frag", nullptr, "screen_shader");

	// Load textures
	ResourceManager::loadTexture("Images/UI/CombatScene/CombatSceneTransparent.png", "combat_HUD");

	// Load fonts
	ResourceManager::loadFont("Fonts/ka1.ttf", "ka1");
	ResourceManager::loadFont("Fonts/Stacked pixel.ttf", "stacked_pixel");

	// Intitialise text renderer
	textRenderer.Initialize(SCREEN_WIDTH, SCREEN_HEIGHT);

	int shape_num_vertices;
	glm::vec4 *shape_vertices = cone(&shape_num_vertices);  // Consider using glm::vec3, theres not really a need to have a glm::vec4 here
	glm::vec4 *shape_colors = genRandomTriangleColors(shape_num_vertices);
	num_vertices = shape_num_vertices;

	ResourceManager::getShader("simple_color_shader").use();
	glGenVertexArrays(1, &coneVAO);
	glBindVertexArray(coneVAO);
	GLuint buffer;
	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec4) * 2 * num_vertices, NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(glm::vec4) * num_vertices, shape_vertices);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(glm::vec4) * num_vertices, sizeof(glm::vec4) * num_vertices, shape_colors);
	GLuint vPosition = glGetAttribLocation(ResourceManager::getShader("simple_color_shader").Program, "vPosition");
	glEnableVertexAttribArray(vPosition);
	glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
	GLuint vColor = glGetAttribLocation(ResourceManager::getShader("simple_color_shader").Program, "vColor");
	glEnableVertexAttribArray(vColor);
	glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_FALSE, 0, (GLvoid *)(sizeof(glm::vec4) * num_vertices));

	glEnable(GL_DEPTH_TEST);
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glDepthRange(1, 0);

	// Vertices for a simple 1x1 textured quad
	GLfloat quadVertices[] = {
		// Positions   // TexCoords
		-1.0f, 1.0f,   0.0f, 1.0f,
		-1.0f, -1.0f,  0.0f, 0.0f,
		1.0f, -1.0f,   1.0f, 0.0f,
		-1.0f, 1.0f,   0.0f, 1.0f,
		1.0f, -1.0f,   1.0f, 0.0f,
		1.0f, 1.0f,    1.0f, 1.0f
	};

	// Set up the vertex array object for the HUD
	GLuint HUDVBO;
	glGenVertexArrays(1, &HUDVAO);
	glGenBuffers(1, &HUDVBO);
	glBindVertexArray(HUDVAO);
	glBindBuffer(GL_ARRAY_BUFFER, HUDVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)(2 * sizeof(GLfloat)));
	glBindVertexArray(0);
}

void Graphics::display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// Set the clear color to light green (same as combatScene.png)
	glClearColor(0.694, 0.886, 0.78, 1.0);

	// Draw the cone
	glPolygonMode(GL_FRONT, GL_FILL);
	glPolygonMode(GL_BACK, GL_LINE);
	ResourceManager::getShader("simple_color_shader").use();
	ResourceManager::getShader("simple_color_shader").setMatrix4("ctm", ctm);
	glBindVertexArray(coneVAO);
	glDrawArrays(GL_TRIANGLES, 0, num_vertices);

	// Draw the UI
	// Set up the projection. We will use an orthographic projection for any UI elements.
	glm::mat4 projection = glm::ortho(0.0f, (GLfloat)SCREEN_WIDTH, 0.0f, (GLfloat)SCREEN_HEIGHT);
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	// Draw the HUD background image
	glBindVertexArray(HUDVAO);
	glDisable(GL_DEPTH_TEST);
	ResourceManager::getShader("screen_shader").setInteger("screenTexture", 0, true);
	glActiveTexture(GL_TEXTURE0);
	ResourceManager::getTexture("combat_HUD").bind();
	glDrawArrays(GL_TRIANGLES, 0, 6);

	// Draw all the text
	for (auto text : m_textToRender)
	{
		textRenderer.RenderText(text);
	}

	glEnable(GL_DEPTH_TEST);
	glBindVertexArray(0);

	SDL_GL_SwapWindow(gWindow);

	// Clear the text
	m_textToRender.clear();
}

void Graphics::idle(void)
{
	ctm = ctm * glm::rotate(0.01f, randomRotationAxis);
	display();
}

void Graphics::rotateRandom(void)
{
	GLfloat x, y, z;

	x = rand() / (float)RAND_MAX;
	y = rand() / (float)RAND_MAX;
	z = rand() / (float)RAND_MAX;

	randomRotationAxis = glm::vec3(x, y, z);
}

void Graphics::addTextToRender(RenderableText text)
{
	m_textToRender.push_back(text);
}

void Graphics::addTextsToRender(std::vector<RenderableText> text)
{
	for (auto t : text)
	{
		addTextToRender(t);
	}
}

glm::vec4* Graphics::cone(int *num_vertices)
{
	float theta, theta_r, theta10_r;
	int index = 0;

	*num_vertices = 216;
	glm::vec4 *vertices = (glm::vec4 *)malloc(sizeof(glm::vec4) * 216);

	for (theta = 0; theta <= 350; theta = theta + 10)
	{
		theta_r = theta * M_PI / 180.0;
		theta10_r = (theta + 10) * M_PI / 180.0;

		vertices[index] = glm::vec4(0.0, -0.5, 0.0, 1.0);
		vertices[index + 1] = glm::vec4(0.5*cos(theta_r), -0.5, 0.5*sin(theta_r), 1.0);
		vertices[index + 2] = glm::vec4(0.5*cos(theta10_r), -0.5, 0.5*sin(theta10_r), 1.0);
		vertices[index + 3] = glm::vec4(0.0, 0.5, 0.0, 1.0);
		vertices[index + 4] = glm::vec4(0.5*cos(theta10_r), -0.5, 0.5*sin(theta10_r), 1.0);
		vertices[index + 5] = glm::vec4(0.5*cos(theta_r), -0.5, 0.5*sin(theta_r), 1.0);
		index += 6;
	}

	return vertices;
}

glm::vec4* Graphics::genRandomTriangleColors(int num_vertices)
{
	GLfloat r, g, b;
	int index = 0, i;

	srand(time(0));

	glm::vec4 *colors = (glm::vec4 *)malloc(sizeof(glm::vec4) * num_vertices);

	for (i = 0; i < num_vertices / 3; i++)
	{
		r = rand() / (float)RAND_MAX;
		g = rand() / (float)RAND_MAX;
		b = rand() / (float)RAND_MAX;

		colors[index] = glm::vec4(r, g, b, 1.0);
		colors[index + 1] = glm::vec4(r, g, b, 1.0);
		colors[index + 2] = glm::vec4(r, g, b, 1.0);
		index += 3;
	}

	return colors;
}