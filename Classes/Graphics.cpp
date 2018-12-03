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

	// Load fonts
	ResourceManager::loadFont("Fonts/ka1.ttf", "ka1");
	ResourceManager::loadFont("Fonts/Stacked pixel.ttf", "stacked_pixel");

	// Intitialise text renderer
	textRenderer.Initialize(SCREEN_WIDTH, SCREEN_HEIGHT);

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

	if (!objectList.empty()) {
		for (std::list<GraphicsObject>::iterator it = objectList.begin(); it != objectList.end(); ++it) {
			switch (it->type) {
			case 0: //Color Object
				ResourceManager::getShader("simple_color_shader").use();
				ResourceManager::getShader("simple_color_shader").setMatrix4("ctm", it->ctm);
				glBindVertexArray(it->VAO);
				glDrawArrays(GL_TRIANGLES, 0, it->num_vertices);
				break;
			case 1: //Texture Object
				glBindVertexArray(it->VAO);
				ResourceManager::getShader("simple_texture_shader").setInteger("texture", 0, true);
				glActiveTexture(GL_TEXTURE0);
				ResourceManager::getTexture(it->texture_ID).bind();
				glDrawArrays(GL_TRIANGLES, 0, it->num_vertices);
				break;
			default:
				break;
			}
		}
	}
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
	//ctm = ctm * glm::rotate(0.01f, randomRotationAxis);
	display();
}

//Returns the index in graphics arrays
int Graphics::genQuadColor(int x, int y, GLfloat z, int height, int width, glm::vec4 color) {
	GraphicsObject newQuad;
	int it;
	GLfloat h, w;	

	newQuad.ID = object_counter++;
	newQuad.type = 0;
	newQuad.num_vertices = quad_num_vertices;

	newQuad.x = ((GLfloat)(x - SCREEN_WIDTH / 2)) / ((GLfloat)(SCREEN_WIDTH / 2));
	newQuad.y = ((GLfloat)(SCREEN_HEIGHT / 2 - y)) / ((GLfloat)(SCREEN_HEIGHT / 2));
	h = ((GLfloat)height) / ((GLfloat)SCREEN_HEIGHT);
	w = ((GLfloat)width) / ((GLfloat)SCREEN_WIDTH);

	newQuad.z = z;
	newQuad.color = color;

	newQuad.position_array = (glm::vec4*)malloc(sizeof(glm::vec4) * newQuad.num_vertices);
	newQuad.color_array = (glm::vec4*)malloc(sizeof(glm::vec4) * newQuad.num_vertices);

	for (it = 0; it < newQuad.num_vertices; it++) {
		newQuad.position_array[it] = glm::translate(glm::mat4(), glm::vec3(newQuad.x, newQuad.y, z)) * (glm::vec4(w, h, 1.0f, 1.0f) * quadVertices[it]);
		newQuad.color_array[it] = color;
	}

	glGenVertexArrays(1, &newQuad.VAO);
	glBindVertexArray(newQuad.VAO);
	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, (sizeof(glm::vec4) * 2) * newQuad.num_vertices, NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(glm::vec4) * newQuad.num_vertices, newQuad.position_array);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(glm::vec4) * newQuad.num_vertices, sizeof(glm::vec4) * newQuad.num_vertices, newQuad.color_array);
	vPosition = glGetAttribLocation(ResourceManager::getShader("simple_color_shader").Program, "vPosition");
	glEnableVertexAttribArray(vPosition);
	glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
	vColor = glGetAttribLocation(ResourceManager::getShader("simple_color_shader").Program, "vColor");
	glEnableVertexAttribArray(vColor);
	glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_FALSE, 0, (GLvoid *)(sizeof(glm::vec4) * newQuad.num_vertices));

	objectList.push_back(newQuad);

	return newQuad.ID;
}

//Returns the index in graphics arrays
int Graphics::genQuadTexture(int x, int y, GLfloat z, int height, int width, const GLchar *file, std::string texture_ID, int texture_sheet_x, int texture_sheet_y, int texture_sheet_size_x, int texture_sheet_size_y) {
	GraphicsObject newQuad;
	int it;
	GLfloat h, w;

	newQuad.ID = object_counter++;
	newQuad.type = 1;
	newQuad.num_vertices = quad_num_vertices;

	newQuad.x = ((GLfloat)(x - SCREEN_WIDTH / 2)) / ((GLfloat)(SCREEN_WIDTH / 2));
	newQuad.y = ((GLfloat)(SCREEN_HEIGHT / 2 - y)) / ((GLfloat)(SCREEN_HEIGHT / 2));
	h = ((GLfloat)height) / ((GLfloat)SCREEN_HEIGHT);
	w = ((GLfloat)width) / ((GLfloat)SCREEN_WIDTH);

	newQuad.z = z;
	newQuad.texture_ID = texture_ID;
	ResourceManager::loadTexture(file, texture_ID);

	newQuad.texture_sheet_x = texture_sheet_x;
	newQuad.texture_sheet_y = texture_sheet_y;
	newQuad.texture_sheet_size_x = texture_sheet_size_x;
	newQuad.texture_sheet_size_y = texture_sheet_size_y;

	newQuad.position_array = (glm::vec4*)malloc(sizeof(glm::vec4) * newQuad.num_vertices);
	newQuad.texture_array = (glm::vec2*)malloc(sizeof(glm::vec2) * newQuad.num_vertices);

	for (it = 0; it < newQuad.num_vertices; it++) {
		newQuad.position_array[it] = glm::translate(glm::mat4(), glm::vec3(newQuad.x, newQuad.y, z)) * (glm::vec4(w, h, 1.0f, 1.0f) * quadVertices[it]);
		newQuad.texture_array[it] = glm::vec2((GLfloat)(texture_sheet_x + 1) * (quadTexCoords[it].x / texture_sheet_size_x), (GLfloat)(texture_sheet_y + 1) * (quadTexCoords[it].y / texture_sheet_size_y));
	}

	glGenVertexArrays(1, &newQuad.VAO);
	glBindVertexArray(newQuad.VAO);
	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, (sizeof(glm::vec4) + sizeof(glm::vec2)) * newQuad.num_vertices, NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(glm::vec4) * newQuad.num_vertices, newQuad.position_array);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(glm::vec4) * newQuad.num_vertices, sizeof(glm::vec2) * newQuad.num_vertices, newQuad.texture_array);
	vPosition = glGetAttribLocation(ResourceManager::getShader("simple_texture_shader").Program, "vPosition");
	glEnableVertexAttribArray(vPosition);
	glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, sizeof(glm::vec4), BUFFER_OFFSET(0));
	vTexCoords = glGetAttribLocation(ResourceManager::getShader("simple_texture_shader").Program, "vTexCoords");
	glEnableVertexAttribArray(vTexCoords);
	glVertexAttribPointer(vTexCoords, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), (GLvoid*)(sizeof(glm::vec4) * newQuad.num_vertices));

	objectList.push_back(newQuad);
	
	return newQuad.ID;
}

//Recolors the rectangle based on index
void Graphics::recolorQuad(int index, glm::vec4 color) {
	
}

//
void Graphics::retextureQuad(int index) {

}

//Removes rectangle based on index
void Graphics::removeObject(int index) {
	
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

int Graphics::genCone(GLfloat radius, GLfloat height, GLfloat resolution, int color_type, glm::vec4 color)
{
	GraphicsObject newCone;
	float theta, theta_r, theta_next_r;
	int index = 0;
	GLfloat increment = 360.0 / resolution;

	newCone.ID = object_counter++;
	newCone.type = 0;
	newCone.num_vertices = 3 * (2 * resolution);
	
	newCone.position_array = (glm::vec4*)malloc(sizeof(glm::vec4) * newCone.num_vertices);
	
	for (theta = 0; theta < 360.0; theta += increment)
	{
		theta_r = theta * M_PI / 180.0;
		theta_next_r = (theta + increment) * M_PI / 180.0;

		newCone.position_array[index] = glm::vec4(0.0, -height / 2, 0.0, 1.0);
		newCone.position_array[index + 1] = glm::vec4(radius * cos(theta_r), -height / 2, radius * sin(theta_r), 1.0);
		newCone.position_array[index + 2] = glm::vec4(radius * cos(theta_next_r), -height / 2, radius * sin(theta_next_r), 1.0);
		newCone.position_array[index + 3] = glm::vec4(0.0, height / 2, 0.0, 1.0);
		newCone.position_array[index + 4] = glm::vec4(radius * cos(theta_next_r), -height / 2, radius * sin(theta_next_r), 1.0);
		newCone.position_array[index + 5] = glm::vec4(radius * cos(theta_r), -height / 2, radius * sin(theta_r), 1.0);
		index += 6;
	}

	switch (color_type) {
	case 0:
		newCone.color_array = genRandomTriangleColors(newCone.num_vertices);
		break;
	case 1:
		newCone.color = color;
		newCone.color_array = genRandomTriangleColorsSimilar(newCone.num_vertices, newCone.color);
		break;
	default:
		break;
	}

	glGenVertexArrays(1, &newCone.VAO);
	glBindVertexArray(newCone.VAO);
	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, (sizeof(glm::vec4) * 2) * newCone.num_vertices, NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(glm::vec4) * newCone.num_vertices, newCone.position_array);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(glm::vec4) * newCone.num_vertices, sizeof(glm::vec4) * newCone.num_vertices, newCone.color_array);
	vPosition = glGetAttribLocation(ResourceManager::getShader("simple_color_shader").Program, "vPosition");
	glEnableVertexAttribArray(vPosition);
	glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
	vColor = glGetAttribLocation(ResourceManager::getShader("simple_color_shader").Program, "vColor");
	glEnableVertexAttribArray(vColor);
	glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_FALSE, 0, (GLvoid *)(sizeof(glm::vec4) * newCone.num_vertices));

	return newCone.ID;
}

//Generates a sphere centered at 0.0, 0.0, 0.0
int Graphics::genSphere(GLfloat radius, GLfloat resolution, int color_type, glm::vec4 color)
{
	GraphicsObject newSphere;
	GLfloat sphere_it, sphere_i, sphere_j;
	GLfloat band_it, band_i, band_j;
	GLfloat circle_it, circle_i, circle_j;
	int it;
	int index = 0;
	GLfloat increment = 360.0 / resolution;

	newSphere.ID = object_counter++;
	newSphere.type = 0;
	newSphere.num_vertices = 3 * ((floor((ceil(resolution / 2) * 2) / 2) - 1) * 2 * resolution); //Not Perfect, but I was tinkering with this

	newSphere.position_array = (glm::vec4*)malloc(sizeof(glm::vec4) * (newSphere.num_vertices));
	
	for (sphere_it = 0; sphere_it < 180.0; sphere_it += increment) {
		sphere_i = sphere_it * M_PI / 180.0;
		sphere_j = (sphere_it + increment) * M_PI / 180.0;


		if (sphere_it == 0) {
			for (circle_it = 0; circle_it < 360.0; circle_it += increment) {
				circle_i = circle_it * M_PI / 180.0;
				circle_j = (circle_it + increment) * M_PI / 180.0;

				newSphere.position_array[index] = glm::vec4(0.0, radius * cos(sphere_i), 0.0, 1.0);
				newSphere.position_array[index + 1] = glm::vec4((radius * cos(circle_i)) * sin(sphere_j), radius * cos(sphere_j), (radius * sin(circle_i) * sin(sphere_j)), 1.0);
				newSphere.position_array[index + 2] = glm::vec4((radius * cos(circle_j)) * sin(sphere_j), radius * cos(sphere_j), (radius * sin(circle_j) * sin(sphere_j)), 1.0);
				index += 3;
			}
		}

		if (sphere_it != 0 && sphere_it + increment < 180.0) {
			for (band_it = 0; band_it < 360.0; band_it += increment) {
				band_i = band_it * M_PI / 180.0;
				band_j = (band_it + increment) * M_PI / 180.0;

				newSphere.position_array[index] = glm::vec4((radius * cos(band_i)) * sin(sphere_i), radius * cos(sphere_i), (radius * sin(band_i) * sin(sphere_i)), 1.0);
				newSphere.position_array[index + 1] = glm::vec4((radius * cos(band_i)) * sin(sphere_j), radius * cos(sphere_j), (radius * sin(band_i)) * sin(sphere_j), 1.0);
				newSphere.position_array[index + 2] = glm::vec4((radius * cos(band_j)) * sin(sphere_j), radius * cos(sphere_j), (radius * sin(band_j)) * sin(sphere_j), 1.0);
				newSphere.position_array[index + 3] = glm::vec4((radius * cos(band_i)) * sin(sphere_i), radius * cos(sphere_i), (radius * sin(band_i)) * sin(sphere_i), 1.0);
				newSphere.position_array[index + 4] = glm::vec4((radius * cos(band_j)) * sin(sphere_j), radius * cos(sphere_j), (radius * sin(band_j)) * sin(sphere_j), 1.0);
				newSphere.position_array[index + 5] = glm::vec4((radius * cos(band_j)) * sin(sphere_i), radius * cos(sphere_i), (radius * sin(band_j)) * sin(sphere_i), 1.0);
				index += 6;
			}
		}

		if (sphere_it + increment >= 180.0) {
			for (circle_it = 0; circle_it < 360.0; circle_it += increment) {
				circle_i = circle_it * M_PI / 180.0;
				circle_j = (circle_it + increment) * M_PI / 180.0;

				newSphere.position_array[index] = glm::vec4(0.0, radius * cos(sphere_j), 0.0, 1.0);
				newSphere.position_array[index + 1] = glm::vec4((radius * cos(circle_j)) * sin(sphere_i), radius * cos(sphere_i), (radius * sin(circle_j)) * sin(sphere_i), 1.0);
				newSphere.position_array[index + 2] = glm::vec4((radius * cos(circle_i)) * sin(sphere_i), radius * cos(sphere_i), (radius * sin(circle_i)) * sin(sphere_i), 1.0);
				index += 3;
			}
		}

	}

	switch (color_type) {
	case 0:
		newSphere.color_array = genRandomTriangleColors(newSphere.num_vertices);
		break;
	case 1:
		newSphere.color = color;
		newSphere.color_array = genRandomTriangleColorsSimilar(newSphere.num_vertices, newSphere.color);
		break;
	default:
		break;
	}

	glGenVertexArrays(1, &newSphere.VAO);
	glBindVertexArray(newSphere.VAO);
	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, (sizeof(glm::vec4) * 2) * newSphere.num_vertices, NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(glm::vec4) * newSphere.num_vertices, newSphere.position_array);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(glm::vec4) * newSphere.num_vertices, sizeof(glm::vec4) * newSphere.num_vertices, newSphere.color_array);
	vPosition = glGetAttribLocation(ResourceManager::getShader("simple_color_shader").Program, "vPosition");
	glEnableVertexAttribArray(vPosition);
	glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
	vColor = glGetAttribLocation(ResourceManager::getShader("simple_color_shader").Program, "vColor");
	glEnableVertexAttribArray(vColor);
	glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_FALSE, 0, (GLvoid *)(sizeof(glm::vec4) * newSphere.num_vertices));

	return newSphere.ID;
}

int Graphics::genCube(int color_type, glm::vec4 color)
{
	GraphicsObject newCube;
	int it;

	newCube.ID = object_counter++;
	newCube.type = 0;
	newCube.num_vertices = 36;

	glm::vec4 cube[36] = { {-0.5, -0.5, 0.5, 1.0}, {0.5, -0.5, 0.5, 1.0}, {0.5, 0.5, 0.5, 1.0}, {-0.5, -0.5, 0.5, 1.0}, {0.5, 0.5, 0.5, 1.0}, {-0.5, 0.5, 0.5, 1.0}, //Front
						{0.5, -0.5, 0.5, 1.0}, {0.5, -0.5, -0.5, 1.0}, {0.5, 0.5, -0.5, 1.0}, {0.5, -0.5, 0.5, 1.0}, {0.5, 0.5, -0.5, 1.0}, {0.5, 0.5, 0.5, 1.0}, //Right
						{0.5, -0.5, -0.5, 1.0}, {-0.5, -0.5, -0.5, 1.0}, {-0.5, 0.5, -0.5, 1.0}, {0.5, -0.5, -0.5, 1.0}, {-0.5, 0.5, -0.5, 1.0}, {0.5, 0.5, -0.5, 1.0}, //Back
						{-0.5, -0.5, -0.5, 1.0}, {-0.5, -0.5, 0.5, 1.0}, {-0.5, 0.5, 0.5, 1.0}, {-0.5, -0.5, -0.5, 1.0}, {-0.5, 0.5, 0.5, 1.0}, {-0.5, 0.5, -0.5, 1.0}, //Left
						{-0.5, 0.5, 0.5, 1.0}, {0.5, 0.5, 0.5, 1.0}, {0.5, 0.5, -0.5, 1.0}, {-0.5, 0.5, 0.5, 1.0}, {0.5, 0.5, -0.5, 1.0}, {-0.5, 0.5, -0.5, 1.0}, //Up
						{-0.5, -0.5, -0.5, 1.0}, {0.5, -0.5, -0.5, 1.0}, {0.5, -0.5, 0.5, 1.0}, {-0.5, -0.5, -0.5, 1.0}, {0.5, -0.5, 0.5, 1.0}, {-0.5, -0.5, 0.5, 1.0} }; //Down

	newCube.position_array = (glm::vec4*)malloc(sizeof(glm::vec4) * (newCube.num_vertices));
	for (it = 0; it < newCube.num_vertices; it++) {
		newCube.position_array[it] = cube[it];
	}
	switch (color_type) {
	case 0:
		newCube.color_array = genRandomTriangleColors(newCube.num_vertices);
		break;
	case 1:
		newCube.color = color;
		newCube.color_array = genRandomTriangleColorsSimilar(newCube.num_vertices, newCube.color);
		break;
	default:
		break;
	}

	glGenVertexArrays(1, &newCube.VAO);
	glBindVertexArray(newCube.VAO);
	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, (sizeof(glm::vec4) * 2) * newCube.num_vertices, NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(glm::vec4) * newCube.num_vertices, newCube.position_array);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(glm::vec4) * newCube.num_vertices, sizeof(glm::vec4) * newCube.num_vertices, newCube.color_array);
	vPosition = glGetAttribLocation(ResourceManager::getShader("simple_color_shader").Program, "vPosition");
	glEnableVertexAttribArray(vPosition);
	glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
	vColor = glGetAttribLocation(ResourceManager::getShader("simple_color_shader").Program, "vColor");
	glEnableVertexAttribArray(vColor);
	glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_FALSE, 0, (GLvoid *)(sizeof(glm::vec4) * newCube.num_vertices));

	return newCube.ID;
}

glm::vec4* Graphics::genRandomTriangleColors(int num_vertices)
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

glm::vec4* Graphics::genRandomTriangleColorsSimilar(int num_vertices, glm::vec4 color)
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