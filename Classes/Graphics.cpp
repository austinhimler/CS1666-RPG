#include "../Headers/Graphics.h"
#include "time.h"
#include <iostream>

#include "../dev_lib/includes/ft2build.h"
#include "../dev_lib/includes/freetype/freetype.h"
#include "../Headers/ResourceManager/ResourceManager.h"
#include "../Headers/UI/TextRenderer.h"

GLuint textVAO, textVBO, coneVAO, HUDVAO, tempVAO;

void Graphics::init(void)
{
	// We want to put the following resource manager stuff somewhere earlier in the program.
	// Or alternatively dont continually reconstruct this graphics class

	// Initialise the resource manager
	ResourceManager::initialise();

	// Load shaders
	ResourceManager::loadShader("Shaders/simpleColorShader.vs", "Shaders/simpleColorShader.frag", nullptr, "simple_color_shader");
	ResourceManager::loadShader("Shaders/simpleTextureShader.vs", "Shaders/simpleTextureShader.frag", nullptr, "simple_texture_shader");

	// Load textures
	ResourceManager::loadTexture("Images/UI/CombatScene/CombatSceneTransparent.png", "combat_HUD");
	ResourceManager::loadTexture("Images/Player/Idle_Down.png", "player");

	// Load fonts
	ResourceManager::loadFont("Fonts/ka1.ttf", "ka1");
	ResourceManager::loadFont("Fonts/Stacked pixel.ttf", "stacked_pixel");

	// Intitialise text renderer
	textRenderer.Initialize(SCREEN_WIDTH, SCREEN_HEIGHT);

	glm::vec4 *shape_vertices = sphere(0.5, 36, 0.0, 0.0, 0.0);
	glm::vec4 *shape_colors = genRandomTriangleColorsSimilar(glm::vec4(1.0, 0.5, 0.0, 1.0));

	int x, y, height, width;
	x = SCREEN_WIDTH/5;
	y = SCREEN_HEIGHT/3;
	height = 144;
	width = 144;
	
	GLfloat i, j, h, w;
	i = ((GLfloat)(x - SCREEN_WIDTH / 2)) / ((GLfloat)(SCREEN_WIDTH / 2));
	j = ((GLfloat)(SCREEN_HEIGHT / 2 - y)) / ((GLfloat)(SCREEN_HEIGHT / 2));
	h = ((GLfloat)height) / ((GLfloat)SCREEN_HEIGHT);
	w = ((GLfloat)width) / ((GLfloat)SCREEN_WIDTH);

	glm::vec4 playerVertices[6];
	glm::vec2 playerTexCoords[6];
	for (int it = 0; it < 6; it++) {
		playerVertices[it] = glm::vec4(i + w * quadVertices[it].x, j + h * quadVertices[it].y, 0.0, quadVertices[it].w);
		playerTexCoords[it] = glm::vec2(quadTexCoords[it].x / 6, quadTexCoords[it].y);
	}
	
	ResourceManager::getShader("simple_color_shader").use();
	glGenVertexArrays(1, &coneVAO);
	glBindVertexArray(coneVAO);
	GLuint buffer;
	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec4) * 2 * num_vertices, NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(glm::vec4) * num_vertices, shape_vertices);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(glm::vec4) * num_vertices, sizeof(glm::vec4) * num_vertices, shape_colors);
	vPosition = glGetAttribLocation(ResourceManager::getShader("simple_color_shader").Program, "vPosition");
	glEnableVertexAttribArray(vPosition);
	glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
	vColor = glGetAttribLocation(ResourceManager::getShader("simple_color_shader").Program, "vColor");
	glEnableVertexAttribArray(vColor);
	glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_FALSE, 0, (GLvoid *)(sizeof(glm::vec4) * num_vertices));

	glGenVertexArrays(1, &tempVAO);
	glBindVertexArray(tempVAO);
	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, (sizeof(glm::vec4) + sizeof(glm::vec2)) * 6, NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(glm::vec4) * 6, playerVertices);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(glm::vec4) * 6, sizeof(glm::vec2) * 6, playerTexCoords);
	vPosition = glGetAttribLocation(ResourceManager::getShader("simple_texture_shader").Program, "vPosition");
	glEnableVertexAttribArray(vPosition);
	glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, sizeof(glm::vec4), BUFFER_OFFSET(0));
	vTexCoords = glGetAttribLocation(ResourceManager::getShader("simple_texture_shader").Program, "vTexCoords");
	glEnableVertexAttribArray(vTexCoords);
	glVertexAttribPointer(vTexCoords, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), (GLvoid*)(sizeof(glm::vec4) * 6));

	// Set up the vertex array object for the HUD
	
	glGenVertexArrays(1, &HUDVAO);
	glBindVertexArray(HUDVAO);
	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, (sizeof(glm::vec4) + sizeof(glm::vec2)) * 6, NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(glm::vec4) * 6, quadVertices);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(glm::vec4) * 6, sizeof(glm::vec2) * 6, quadTexCoords);
	vPosition = glGetAttribLocation(ResourceManager::getShader("simple_texture_shader").Program, "vPosition");
	glEnableVertexAttribArray(vPosition);
	glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
	vTexCoords = glGetAttribLocation(ResourceManager::getShader("simple_texture_shader").Program, "vTexCoords");
	glEnableVertexAttribArray(vTexCoords);
	glVertexAttribPointer(vTexCoords, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid*)(sizeof(glm::vec4) * 6));

	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	// Set the clear color to light green (same as combatScene.png)
	glClearColor(0.694, 0.886, 0.78, 1.0);
	glDepthRange(1, 0);
}

void Graphics::display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glPolygonMode(GL_FRONT, GL_FILL);
	glPolygonMode(GL_BACK, GL_LINE);

	// Draw the cone
	ResourceManager::getShader("simple_color_shader").use();
	ResourceManager::getShader("simple_color_shader").setMatrix4("ctm", ctm);
	glBindVertexArray(coneVAO);
	glDrawArrays(GL_TRIANGLES, 0, num_vertices);

	// Draw the character
	glBindVertexArray(tempVAO);
	ResourceManager::getShader("simple_texture_shader").setInteger("texture", 0, true);
	glActiveTexture(GL_TEXTURE0);
	ResourceManager::getTexture("player").bind();
	glDrawArrays(GL_TRIANGLES, 0, 6);

	// Draw the UI
	// Set up the projection. We will use an orthographic projection for any UI elements.
	glm::mat4 projection = glm::ortho(0.0f, (GLfloat)SCREEN_WIDTH, 0.0f, (GLfloat)SCREEN_HEIGHT);
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Draw the HUD background image
	glBindVertexArray(HUDVAO);
	ResourceManager::getShader("simple_texture_shader").setInteger("texture", 0, true);
	glActiveTexture(GL_TEXTURE0);
	ResourceManager::getTexture("combat_HUD").bind();
	glDrawArrays(GL_TRIANGLES, 0, 6);

	// Draw all the text
	for (auto text : m_textToRender)
	{
		textRenderer.RenderText(text);
	}

	// Clear the text
	m_textToRender.clear();

	glEnable(GL_DEPTH_TEST);

	SDL_GL_SwapWindow(gWindow);
}

void Graphics::idle(void)
{
	ctm = ctm * glm::rotate(0.01f, randomRotationAxis);
	display();
}

//Returns the index in graphics arrays
int Graphics::genQuadColor(int x, int y, GLfloat z, int height, int width, glm::vec4 color) {
	int it;
	GLfloat i, j, h, w;
	int index = num_vertices;

	if ((x + width / 2) < 0 || (x - width / 2) > SCREEN_WIDTH || (y + height / 2) < 0 || (y + height / 2) > SCREEN_HEIGHT || z < -1 || z > 1 || height != 0 || width != 0) {
		return NULL;
	}

	i = (x - SCREEN_WIDTH / 2) / (SCREEN_WIDTH / 2);
	j = (SCREEN_HEIGHT / 2 - y) / (SCREEN_HEIGHT / 2);
	h = height / SCREEN_HEIGHT;
	w = width / SCREEN_WIDTH;

	num_vertices += 6;

	glm::vec4 *temp_vertices = (glm::vec4 *)malloc(sizeof(glm::vec4) * num_vertices);
	glm::vec4 *temp_colors = (glm::vec4 *)malloc(sizeof(glm::vec4) * num_vertices);
	glm::vec2 *temp_textures = (glm::vec2 *)malloc(sizeof(glm::vec2) * num_vertices);

	for (it = 0; it < index; it++) {
		temp_vertices[it] = vertices[it];
		temp_colors[it] = colors[it];
		temp_textures[it] = textures[it];
	}

	for (it = 0; it < 6; it++) {
		temp_vertices[index + it] = glm::translate(glm::mat4(), glm::vec3(i, j, z)) * (glm::vec4(w, h, 1.0f, 1.0f) * quadVertices[it]);
		temp_colors[index + it] = color;
		temp_textures[index + it] = { 0.0f, 0.0f };
	}

	free(vertices);
	free(colors);
	free(textures);

	vertices = (glm::vec4 *)malloc(sizeof(glm::vec4) * num_vertices);
	colors = (glm::vec4 *)malloc(sizeof(glm::vec4) * num_vertices);
	textures = (glm::vec2 *)malloc(sizeof(glm::vec2) * num_vertices);

	for (it = 0; it < num_vertices; it++) {
		temp_vertices[it] = vertices[it];
		temp_colors[it] = colors[it];
		temp_textures[it] = textures[it];
	}

	free(temp_vertices);
	free(temp_colors);
	free(temp_textures);

	return index;
}

//Returns the index in graphics arrays
int Graphics::genQuadTexture(int x, int y, GLfloat z, int height, int width, std::string texture_path) {
	int it;
	GLfloat i, j, h, w;
	int index = num_vertices;

	if ((x + width / 2) < 0 || (x - width / 2) > SCREEN_WIDTH || (y + height / 2) < 0 || (y + height / 2) > SCREEN_HEIGHT || z < -1 || z > 1 || height != 0 || width != 0) {
		return NULL;
	}

	i = (x - SCREEN_WIDTH / 2) / (SCREEN_WIDTH / 2);
	j = (SCREEN_HEIGHT / 2 - y) / (SCREEN_HEIGHT / 2);
	h = height / SCREEN_HEIGHT;
	w = width / SCREEN_WIDTH;

	num_vertices += 6;

	glm::vec4 *temp_vertices = (glm::vec4 *)malloc(sizeof(glm::vec4) * num_vertices);
	glm::vec4 *temp_colors = (glm::vec4 *)malloc(sizeof(glm::vec4) * num_vertices);
	glm::vec2 *temp_textures = (glm::vec2 *)malloc(sizeof(glm::vec2) * num_vertices);
	
	for (it = 0; it < index; it++) {
		temp_vertices[it] = vertices[it];
		temp_colors[it] = colors[it];
		temp_textures[it] = textures[it];
	}

	for (it = 0; it < 6; it++) {
		temp_vertices[index + it] = glm::translate(glm::mat4(), glm::vec3(i, j, z)) * (glm::vec4(w, h, 1.0f, 1.0f) * quadVertices[it]);
		temp_colors[index + it] = { 0.0f, 0.0f, 0.0f, 0.0f };
		temp_textures[index + it] = quadTexCoords[it];
	}

	free(vertices);
	free(colors);
	free(textures);

	vertices = (glm::vec4 *)malloc(sizeof(glm::vec4) * num_vertices);
	colors = (glm::vec4 *)malloc(sizeof(glm::vec4) * num_vertices);
	textures = (glm::vec2 *)malloc(sizeof(glm::vec2) * num_vertices);

	for (it = 0; it < num_vertices; it++) {
		temp_vertices[it] = vertices[it];
		temp_colors[it] = colors[it];
		temp_textures[it] = textures[it];
	}

	free(temp_vertices);
	free(temp_colors);
	free(temp_textures);

	return index;
}

//Recolors the rectangle based on index
void Graphics::recolorQuad(int index, glm::vec4 color) {
	int it;
	for (it = 0; it < 6; it++) {
		colors[index + it] = color;
	}
}

//
void Graphics::retextureQuad(int index) {

}

//Removes rectangle based on index
void Graphics::removeObject(int index) {
	int it;

	glm::vec4 *temp_vertices = (glm::vec4 *)malloc(sizeof(glm::vec4) * num_vertices);
	glm::vec4 *temp_colors = (glm::vec4 *)malloc(sizeof(glm::vec4) * num_vertices);
	glm::vec2 *temp_textures = (glm::vec2 *)malloc(sizeof(glm::vec2) * num_vertices);

	for (it = 0; it < num_vertices; it++) {
		temp_vertices[it] = vertices[it];
		temp_colors[it] = colors[it];
		temp_textures[it] = textures[it];
	}

	num_vertices -= 6;

	free(vertices);
	free(colors);
	free(textures);

	vertices = (glm::vec4 *)malloc(sizeof(glm::vec4) * num_vertices);
	colors = (glm::vec4 *)malloc(sizeof(glm::vec4) * num_vertices);
	textures = (glm::vec2 *)malloc(sizeof(glm::vec2) * num_vertices);

	for (it = 0; it < num_vertices; it++) {
		if (it < index) {
			vertices[it] = temp_vertices[it];
			colors[it] = temp_colors[it];
			textures[it] = temp_textures[it];
		}
		else {
			vertices[it] = temp_vertices[it + 6];
			colors[it] = temp_colors[it + 6];
			textures[it] = temp_textures[it + 6];
		}
	}

	free(temp_vertices);
	free(temp_colors);
	free(temp_textures);
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

glm::vec4* Graphics::cone()
{
	float theta, theta_r, theta10_r;
	int index = 0;
	int cone_num_vertices = 216;
	
	glm::vec4 *vertices = (glm::vec4 *)malloc(sizeof(glm::vec4) * cone_num_vertices);

	num_vertices = cone_num_vertices;

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

//Generates a sphere centered at 0.0, 0.0, 0.0
glm::vec4* Graphics::sphere(GLfloat radius, GLfloat resolution, GLfloat x, GLfloat y, GLfloat z)
{
	GLfloat sphere_it, sphere_i, sphere_j;
	GLfloat band_it, band_i, band_j;
	GLfloat circle_it, circle_i, circle_j;
	int it;
	GLfloat increment = 360.0 / resolution;
	int index = 0;
	int sphere_num_vertices = 3 * ((floor((ceil(resolution / 2) * 2) / 2) - 1) * 2 * resolution); //Not Perfect, but I was tinkering with this

	glm::vec4* sphere_vertices = (glm::vec4*)malloc(sizeof(glm::vec4) * (sphere_num_vertices));

	num_vertices = sphere_num_vertices;

	for (sphere_it = 0; sphere_it < 180.0; sphere_it += increment) {
		sphere_i = sphere_it * M_PI / 180.0;
		sphere_j = (sphere_it + increment) * M_PI / 180.0;


		if (sphere_it == 0) {
			for (circle_it = 0; circle_it < 360.0; circle_it += increment) {
				circle_i = circle_it * M_PI / 180.0;
				circle_j = (circle_it + increment) * M_PI / 180.0;

				sphere_vertices[index] = glm::vec4(x, y + (radius * cos(sphere_i)), z, 1.0);
				sphere_vertices[index + 1] = glm::vec4(x + ((radius * cos(circle_i)) * sin(sphere_j)), y + (radius * cos(sphere_j)), z + ((radius * sin(circle_i)) * sin(sphere_j)), 1.0);
				sphere_vertices[index + 2] = glm::vec4(x + ((radius * cos(circle_j)) * sin(sphere_j)), y + (radius * cos(sphere_j)), z + ((radius * sin(circle_j)) * sin(sphere_j)), 1.0);
				index += 3;
			}
		}

		if (sphere_it != 0 && sphere_it + increment < 180.0) {
			for (band_it = 0; band_it < 360.0; band_it += increment) {
				band_i = band_it * M_PI / 180.0;
				band_j = (band_it + increment) * M_PI / 180.0;

				sphere_vertices[index] = glm::vec4(x + ((radius * cos(band_i)) * sin(sphere_i)), y + (radius * cos(sphere_i)), z + ((radius * sin(band_i)) * sin(sphere_i)), 1.0);
				sphere_vertices[index + 1] = glm::vec4(x + ((radius * cos(band_i)) * sin(sphere_j)), y + (radius * cos(sphere_j)), z + ((radius * sin(band_i)) * sin(sphere_j)), 1.0);
				sphere_vertices[index + 2] = glm::vec4(x + ((radius * cos(band_j)) * sin(sphere_j)), y + (radius * cos(sphere_j)), z + ((radius * sin(band_j)) * sin(sphere_j)), 1.0);
				sphere_vertices[index + 3] = glm::vec4(x + ((radius * cos(band_i)) * sin(sphere_i)), y + (radius * cos(sphere_i)), z + ((radius * sin(band_i)) * sin(sphere_i)), 1.0);
				sphere_vertices[index + 4] = glm::vec4(x + ((radius * cos(band_j)) * sin(sphere_j)), y + (radius * cos(sphere_j)), z + ((radius * sin(band_j)) * sin(sphere_j)), 1.0);
				sphere_vertices[index + 5] = glm::vec4(x + ((radius * cos(band_j)) * sin(sphere_i)), y + (radius * cos(sphere_i)), z + ((radius * sin(band_j)) * sin(sphere_i)), 1.0);
				index += 6;
			}
		}

		if (sphere_it + increment >= 180.0) {
			for (circle_it = 0; circle_it < 360.0; circle_it += increment) {
				circle_i = circle_it * M_PI / 180.0;
				circle_j = (circle_it + increment) * M_PI / 180.0;

				sphere_vertices[index] = glm::vec4(x, y + (radius * cos(sphere_j)), z, 1.0);
				sphere_vertices[index + 1] = glm::vec4(x + ((radius * cos(circle_j)) * sin(sphere_i)), y + (radius * cos(sphere_i)), z + ((radius * sin(circle_j)) * sin(sphere_i)), 1.0);
				sphere_vertices[index + 2] = glm::vec4(x + ((radius * cos(circle_i)) * sin(sphere_i)), y + (radius * cos(sphere_i)), z + ((radius * sin(circle_i)) * sin(sphere_i)), 1.0);
				index += 3;
			}
		}

	}
	return sphere_vertices;
}

glm::vec4* Graphics::cube(GLfloat scale, GLfloat x, GLfloat y, GLfloat z)
{
	int it;
	glm::vec4 cube[36] = { {-0.5, -0.5, 0.5, 1.0}, {0.5, -0.5, 0.5, 1.0}, {0.5, 0.5, 0.5, 1.0}, {-0.5, -0.5, 0.5, 1.0}, {0.5, 0.5, 0.5, 1.0}, {-0.5, 0.5, 0.5, 1.0}, //Front
						{0.5, -0.5, 0.5, 1.0}, {0.5, -0.5, -0.5, 1.0}, {0.5, 0.5, -0.5, 1.0}, {0.5, -0.5, 0.5, 1.0}, {0.5, 0.5, -0.5, 1.0}, {0.5, 0.5, 0.5, 1.0}, //Right
						{0.5, -0.5, -0.5, 1.0}, {-0.5, -0.5, -0.5, 1.0}, {-0.5, 0.5, -0.5, 1.0}, {0.5, -0.5, -0.5, 1.0}, {-0.5, 0.5, -0.5, 1.0}, {0.5, 0.5, -0.5, 1.0}, //Back
						{-0.5, -0.5, -0.5, 1.0}, {-0.5, -0.5, 0.5, 1.0}, {-0.5, 0.5, 0.5, 1.0}, {-0.5, -0.5, -0.5, 1.0}, {-0.5, 0.5, 0.5, 1.0}, {-0.5, 0.5, -0.5, 1.0}, //Left
						{-0.5, 0.5, 0.5, 1.0}, {0.5, 0.5, 0.5, 1.0}, {0.5, 0.5, -0.5, 1.0}, {-0.5, 0.5, 0.5, 1.0}, {0.5, 0.5, -0.5, 1.0}, {-0.5, 0.5, -0.5, 1.0}, //Up
						{-0.5, -0.5, -0.5, 1.0}, {0.5, -0.5, -0.5, 1.0}, {0.5, -0.5, 0.5, 1.0}, {-0.5, -0.5, -0.5, 1.0}, {0.5, -0.5, 0.5, 1.0}, {-0.5, -0.5, 0.5, 1.0} }; //Down
	int cube_num_vertices = 36;
	glm::vec4* cube_vertices = (glm::vec4*)malloc(sizeof(glm::vec4) * (cube_num_vertices));

	num_vertices = cube_num_vertices;

	for (it = 0; it < cube_num_vertices; it++) {
		cube_vertices[it] = glm::vec4(x + (scale * cube[it].x), y + (scale * cube[it].y), z + (scale * cube[it].z), cube[it].w);
	}

	return cube_vertices;
}

glm::vec4* Graphics::genRandomTriangleColors()
{
	GLfloat r, g, b;
	int index = 0, it;

	srand(time(0));

	glm::vec4 *colors = (glm::vec4 *)malloc(sizeof(glm::vec4) * num_vertices);

	for (it = 0; it < num_vertices / 3; it++)
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

glm::vec4* Graphics::genRandomTriangleColorsSimilar(glm::vec4 color)
{
	GLfloat r, g, b, modifying_color;
	int index = 0, it;

	srand(time(0));

	glm::vec4 *colors = (glm::vec4 *)malloc(sizeof(glm::vec4) * num_vertices);

	for (it = 0; it < num_vertices / 3; it++)
	{
		modifying_color = (rand() / (float)RAND_MAX) - 0.5;
		r = color.x + modifying_color * 0.1;
		g = color.y + modifying_color * 0.1;
		b = color.z + modifying_color * 0.1;

		colors[index] = glm::vec4(r, g, b, 1.0);
		colors[index + 1] = glm::vec4(r, g, b, 1.0);
		colors[index + 2] = glm::vec4(r, g, b, 1.0);
		index += 3;
	}

	return colors;
}