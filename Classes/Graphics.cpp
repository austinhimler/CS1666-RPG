#include "../Headers/Graphics.h"
#include "time.h"
#include <string>

GLuint Graphics::getProgram(void)
{
	return program;
}

void Graphics::init(void)
{
	int outp;

	program = Shader::initShader("vshader.glsl", "fshader.glsl");
	glUseProgram(program);

	generate2dRectangleColor(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f, SCREEN_HEIGHT, SCREEN_WIDTH, { 0.0f, 0.0f, 1.0f, 1.0f });
	//printf("outp: %d\n", outp);

	//num_vertices = 54;

	//vertices = (glm::vec4 *)malloc(sizeof(glm::vec4) * num_vertices);
	//colors = (glm::vec4 *)malloc(sizeof(glm::vec4) * num_vertices);
	//textures = (glm::vec2 *)malloc(sizeof(glm::vec2) * num_vertices);

	//generateCombat(vertices, colors, textures);

	//int shape_num_vertices;
	//glm::vec4 *shape_vertices = cone(&shape_num_vertices);
	//glm::vec4 *shape_colors = genRandomTriangleColors(shape_num_vertices);
	//num_vertices = shape_num_vertices;

	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	GLuint vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, (2 * sizeof(glm::vec4) + sizeof(glm::vec2)) * num_vertices, NULL, GL_STATIC_DRAW);

	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(glm::vec4) * num_vertices, vertices);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(glm::vec4) * num_vertices, sizeof(glm::vec4) * num_vertices, colors);
	glBufferSubData(GL_ARRAY_BUFFER, 2 * sizeof(glm::vec4) * num_vertices, sizeof(glm::vec2) * num_vertices, textures);

	GLuint vPosition = glGetAttribLocation(program, "vPosition");
	glEnableVertexAttribArray(vPosition);
	glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

	GLuint vColor = glGetAttribLocation(program, "vColor");
	glEnableVertexAttribArray(vColor);
	glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_FALSE, 0, (GLvoid *)(sizeof(glm::vec4) * num_vertices));

	GLuint vTexCoord = glGetAttribLocation(program, "vTexCoord");
	glEnableVertexAttribArray(vTexCoord);
	glVertexAttribPointer(vTexCoord, 4, GL_FLOAT, GL_FALSE, 0, (GLvoid *)(sizeof(glm::vec2) * 2 * num_vertices));

	ctm_location = glGetUniformLocation(program, "ctm");

	glEnable(GL_DEPTH_TEST);
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glDepthRange(1, 0);
}

void Graphics::display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glPolygonMode(GL_FRONT, GL_FILL);
	glPolygonMode(GL_BACK, GL_LINE);

	glUniformMatrix4fv(ctm_location, 1, GL_FALSE, (GLfloat *)&ctm);

	glDrawArrays(GL_TRIANGLES, 0, num_vertices);

	SDL_GL_SwapWindow(gWindow);
}

void Graphics::idle(void)
{
	//ctm = ctm * glm::rotate(0.01f, randomRotationAxis);
	//display();
}

/*void Graphics::loadTexture(string file, int width, int height)
{
	GLubyte* my_texels = (GLubyte*)malloc(sizeof(GLubyte) * width * height * 3);
	FILE *fp = fopen(file, "r");
	if (fp == NULL)
	{
		printf("Unable to open the texture file\n");
	}
	fread(my_texels, width * height * 3, 1, fp);
	glUseProgram(program);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
}*/

void Graphics::generateCombat(glm::vec4 *vert, glm::vec4 *color, glm::vec2 *text)
{
	int i, j, outp;
	int it = 0;
	//Background
	/*for (i = 0; i < 6; i++) {
		vert[it] = glm::translate(glm::mat4(), glm::vec3(0.0f, 0.0f, -0.9f)) * rectangle[i];
		color[it] = { 0.0f, 0.0f, 1.0f, 1.0f };
		//Filler
		text[it] = { 0.0f, 0.0f };
		it++;
	}*/
	printf("test\n");
	

	//Player
	for (i = 0; i < 6; i++) {
		vert[it] = glm::translate(glm::mat4(), glm::vec3(-0.5f, 0.25f, 0.0f)) * (glm::vec4(0.25f, 0.25f, 0.25f, 1.0f) * rectangle[i]);
		color[it] = { 0.0f, 0.0f, 0.0f, 1.0f };
		//Filler
		text[it] = { 0.0f, 0.0f };
		it++;
	}

	//Enemy
	for (i = 0; i < 6; i++) {
		vert[it] = glm::translate(glm::mat4(), glm::vec3(0.5f, 0.25f, 0.0f)) * (glm::vec4(0.25f, 0.25f, 0.25f, 1.0f) * rectangle[i]);
		color[it] = { 1.0f, 0.0f, 0.0f, 1.0f };
		//Filler
		text[it] = { 0.0f, 0.0f };
		it++;
	}
	/*load texture for buttons
	Tried getting SOIL to link correctly for hours but I had no luck, I'll try again tonight but incase someone else figures it out before I do here's some code that should work
	GLuint tex;
	glGenTextures(1, &tex);
	glBindTexture(GL_TEXTURE_2D, tex);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	int width, height;
	unsigned char* image = SOIL_load_image("Images/UI/CombatScene/Button.png", &width, &height, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	*/
	//Buttons
	for (j = 0; j < 6; j++) {
		for (i = 0; i < 6; i++) {
			vert[it] = glm::translate(glm::mat4(), glm::vec3(-0.75f + (0.25f * (j % 3)), (-0.5f - (0.25f * ((j > 2) ? 1 : 0))), 1.0f)) * (glm::vec4(0.0625f, 0.0625f, 0.0625f, 1.0f) * rectangle[i]);
			color[it] = { 0.0f, 1.0f, 0.0f, 1.0f };
			//Filler
			text[it] = { 0.0f, 0.0f };
			it++;
		}
	}
}

//Returns the index in graphics arrays
int Graphics::generate2dRectangleColor(int x, int y, GLfloat z, int height, int width, glm::vec4 color) {
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
		temp_vertices[index + it] = glm::translate(glm::mat4(), glm::vec3(i, j, z)) * (glm::vec4(w, h, 1.0f, 1.0f) * rectangle[it]);
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
int Graphics::generate2dRectangleColorCorners(int x, int y, GLfloat z, int height, int width, glm::vec4 color_tl, glm::vec4 color_bl, glm::vec4 color_tr, glm::vec4 color_br) {
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
		temp_vertices[index + it] = glm::translate(glm::mat4(), glm::vec3(i, j, z)) * (glm::vec4(w, h, 1.0f, 1.0f) * rectangle[it]);
		temp_textures[index + it] = { 0.0f, 0.0f };
	}
	temp_colors[index + 0] = color_bl;
	temp_colors[index + 1] = color_tr;
	temp_colors[index + 2] = color_tl;
	temp_colors[index + 3] = color_bl;
	temp_colors[index + 4] = color_br;
	temp_colors[index + 5] = color_tr;
	
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
int Graphics::generate2dRectangleTexture(int x, int y, GLfloat z, int height, int width, std::string texture_path) {
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
		temp_vertices[index + it] = glm::translate(glm::mat4(), glm::vec3(i, j, z)) * (glm::vec4(w, h, 1.0f, 1.0f) * rectangle[it]);
		temp_colors[index + it] = { 0.0f, 0.0f, 0.0f, 0.0f };
		temp_textures[index + it] = rectangle_texture[it];
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
void Graphics::recolor2dRectangle(int index, glm::vec4 color) {
	int it;
	for (it = 0; it < 6; it++) {
		colors[index + it] = color;
	}
}

//Recolors the rectangle's corners based on index
void Graphics::recolorCorners2dRectangle(int index, glm::vec4 color_tl, glm::vec4 color_bl, glm::vec4 color_tr, glm::vec4 color_br) {
	colors[index + 0] = color_bl;
	colors[index + 1] = color_tr;
	colors[index + 2] = color_tl;
	colors[index + 3] = color_bl;
	colors[index + 4] = color_br;
	colors[index + 5] = color_tr;
}

//Removes rectangle based on index
void Graphics::remove2dRectangle(int index) {
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