#include "../Headers/Graphics.h"

/*
	Initalize Graphics
*/
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
	glGenBuffers(1, &HUDVBO);
	glBindBuffer(GL_ARRAY_BUFFER, HUDVBO);
	glBufferData(GL_ARRAY_BUFFER, (sizeof(glm::vec4) + sizeof(glm::vec2)) * 6, NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(glm::vec4) * 6, quadVertices);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(glm::vec4) * 6, sizeof(glm::vec2) * 6, quadTexCoords);
	vPosition = glGetAttribLocation(ResourceManager::getShader("simple_texture_shader").Program, "vPosition");
	glEnableVertexAttribArray(vPosition);
	glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
	vTexCoords = glGetAttribLocation(ResourceManager::getShader("simple_texture_shader").Program, "vTexCoords");
	glEnableVertexAttribArray(vTexCoords);
	glVertexAttribPointer(vTexCoords, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid*)(sizeof(glm::vec4) * 6));

	// Some OpenGL settings we want to set
	//glEnable(GL_CULL_FACE);
	// Set the clear color to light green (same as combatScene.png)
	glClearColor(0.694f, 0.886f, 0.78f, 1.0f);
	glDepthRange(1, 0);
}

void Graphics::clean(void)
{
	glDeleteVertexArrays(1, &HUDVAO);
	glDeleteBuffers(1, &HUDVBO);
	objectList.clear();
}

void Graphics::display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glPolygonMode(GL_FRONT, GL_FILL);
	glPolygonMode(GL_BACK, GL_FILL);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


	if (!objectList.empty()) {
		for (std::list<GraphicsObject>::iterator it = objectList.begin(); it != objectList.end(); ++it) {
			switch (it->type) {
			case 0: //Color Object
				//glEnable(GL_DEPTH_TEST);
				ResourceManager::getShader("simple_color_shader").use();
				ResourceManager::getShader("simple_color_shader").setMatrix4("ctm", it->ctm);
				glBindVertexArray(it->VAO);
				glDrawArrays(GL_TRIANGLES, 0, it->num_vertices);
				break;
			case 1: //Texture Object
				//glEnable(GL_BLEND);
				//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
				ResourceManager::getShader("simple_texture_shader").use();
				ResourceManager::getShader("simple_texture_shader").setInteger("texture", 0, true);
				ResourceManager::getShader("simple_texture_shader").setMatrix4("ctm", it->ctm);
				glBindVertexArray(it->VAO);
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
	//glDisable(GL_DEPTH_TEST);
	//glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Draw the HUD background image
	ResourceManager::getShader("simple_texture_shader").use();
	ResourceManager::getShader("simple_texture_shader").setInteger("texture", 0, true);
	ResourceManager::getShader("simple_texture_shader").setMatrix4("ctm", { { 1.0, 0.0, 0.0, 0.0 },{ 0.0, 1.0, 0.0, 0.0 },{ 0.0, 0.0, 1.0, 0.0 },{ 0.0, 0.0, 0.0, 1.0 } });
	glBindVertexArray(HUDVAO);
	glActiveTexture(GL_TEXTURE0);
	ResourceManager::getTexture("combat_HUD").bind();
	glDrawArrays(GL_TRIANGLES, 0, 6);

	// Draw all the text
	for (auto text : m_textToRender)
	{
		textRenderer.RenderText(text);
	}

	m_textToRender.clear();

	//glDrawArrays(GL_TRIANGLES, 0, num_vertices);

	SDL_GL_SwapWindow(gWindow);
}

void Graphics::idle(void)
{
	if (!objectList.empty()) {
		for (std::list<GraphicsObject>::iterator it = objectList.begin(); it != objectList.end(); ++it) {
			switch (it->idle_animation_type) {
				case 0: //0 = no animation
					break;
				case 1: //1 = sprite animation
					iterateSpriteAnimation(it);
					break;
				case 2: //2 = motion animation
					it->ctm = it->ctm * it->idle_animation_motion;
					break;
				case 3: //3 = sprite and motion animation
					iterateSpriteAnimation(it);
					it->ctm = it->ctm * it->idle_animation_motion;
					break;
				default:
					break;
			}
			switch (it->animation_type) {
				case 0: //0 = no animation
					break;
				case 1: //1 = motion animation
					animateMotion(it);
					break;
				case 2: //2 = motion animation then consume
					animateMotionConsume(it);
					break;
				case 3: //3 = multistep motion animation
					animateMultiStep(it);
					break;
				case 4: //3 = multistep motion animation then consume
					animateMultiStepConsume(it);
					break;
				default:
					break;
			}
		}
		
	}
	if (!eraseBuffer.empty()) {
		for (std::list<int>::iterator it = eraseBuffer.begin(); it != eraseBuffer.end(); ++it) {
			removeObject(*it);
		}
	}
	eraseBuffer.clear();

	display();
}

int Graphics::genQuadColor(int height, int width, glm::vec4 color)
{
	GraphicsObject newQuad;
	int it;
	GLfloat h, w;	

	newQuad.ID = object_counter++;
	newQuad.type = 0;
	newQuad.num_vertices = quad_num_vertices;

	h = ((GLfloat)height) / ((GLfloat)SCREEN_HEIGHT);
	w = ((GLfloat)width) / ((GLfloat)SCREEN_WIDTH);

	newQuad.color = color;

	newQuad.position_array = (glm::vec4*)malloc(sizeof(glm::vec4) * newQuad.num_vertices);
	newQuad.color_array = (glm::vec4*)malloc(sizeof(glm::vec4) * newQuad.num_vertices);

	for (it = 0; it < newQuad.num_vertices; it++) {
		newQuad.position_array[it] = glm::vec4(w, h, 1.0f, 1.0f) * quadVertices[it];
		newQuad.color_array[it] = color;
	}

	glGenVertexArrays(1, &newQuad.VAO);
	glBindVertexArray(newQuad.VAO);
	glGenBuffers(1, &newQuad.VBO);
	glBindBuffer(GL_ARRAY_BUFFER, newQuad.VBO);
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

int Graphics::genQuadTexture(int height, int width, const GLchar *file, std::string texture_ID, int texture_sheet_it, int texture_sheet_size)
{
	GraphicsObject newQuad;
	int it;
	GLfloat h, w;

	newQuad.ID = object_counter++;
	newQuad.type = 1;
	newQuad.num_vertices = quad_num_vertices;

	h = ((GLfloat)height) / ((GLfloat)SCREEN_HEIGHT);
	w = ((GLfloat)width) / ((GLfloat)SCREEN_WIDTH);

	newQuad.texture_ID = texture_ID;
	ResourceManager::loadTexture(file, texture_ID);

	newQuad.texture_sheet_it = texture_sheet_it;
	newQuad.texture_sheet_size = texture_sheet_size;

	newQuad.position_array = (glm::vec4*)malloc(sizeof(glm::vec4) * newQuad.num_vertices);
	newQuad.texture_array = (glm::vec2*)malloc(sizeof(glm::vec2) * newQuad.num_vertices);

	for (it = 0; it < newQuad.num_vertices; it++) {
		newQuad.position_array[it] = glm::vec4(w, h, 1.0f, 1.0f) * quadVertices[it];
		newQuad.texture_array[it] = glm::vec2((GLfloat)(newQuad.texture_sheet_it + quadTexCoords[it].x) * (1.0 / newQuad.texture_sheet_size), quadTexCoords[it].y);
	}

	glGenVertexArrays(1, &newQuad.VAO);
	glBindVertexArray(newQuad.VAO);
	glGenBuffers(1, &newQuad.VBO);
	glBindBuffer(GL_ARRAY_BUFFER, newQuad.VBO);
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

int Graphics::genCone(GLfloat radius, GLfloat height, int resolution, int color_type, glm::vec4 color)
{
	GraphicsObject newCone;
	float theta, theta_r, theta_next_r;
	int index = 0;
	GLfloat increment = 360.0f / resolution;

	newCone.ID = object_counter++;
	newCone.type = 0;
	newCone.num_vertices = 3 * (2 * resolution);

	newCone.position_array = (glm::vec4*)malloc(sizeof(glm::vec4) * newCone.num_vertices);

	for (theta = 0; theta < 360.0f; theta += increment)
	{
		theta_r =(float)(theta * M_PI / 180.0f);
		theta_next_r =(float)((theta + increment) * M_PI / 180.0f);

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
		newCone.color_array = genRandomTriangleColorsAlpha(newCone.num_vertices);
		break;
	case 2:
		newCone.color = color;
		newCone.color_array = genRandomTriangleColorsSimilar(newCone.num_vertices, newCone.color);
		break;
	default:
		break;
	}

	glGenVertexArrays(1, &newCone.VAO);
	glBindVertexArray(newCone.VAO);
	glGenBuffers(1, &newCone.VBO);
	glBindBuffer(GL_ARRAY_BUFFER, newCone.VBO);
	glBufferData(GL_ARRAY_BUFFER, (sizeof(glm::vec4) * 2) * newCone.num_vertices, NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(glm::vec4) * newCone.num_vertices, newCone.position_array);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(glm::vec4) * newCone.num_vertices, sizeof(glm::vec4) * newCone.num_vertices, newCone.color_array);
	vPosition = glGetAttribLocation(ResourceManager::getShader("simple_color_shader").Program, "vPosition");
	glEnableVertexAttribArray(vPosition);
	glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
	vColor = glGetAttribLocation(ResourceManager::getShader("simple_color_shader").Program, "vColor");
	glEnableVertexAttribArray(vColor);
	glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_FALSE, 0, (GLvoid *)(sizeof(glm::vec4) * newCone.num_vertices));

	objectList.push_back(newCone);

	return newCone.ID;
}

int Graphics::genSphere(GLfloat radius, int resolution, int color_type, glm::vec4 color)
{
	GraphicsObject newSphere;
	GLfloat sphere_it, sphere_i, sphere_j;
	GLfloat band_it, band_i, band_j;
	GLfloat circle_it, circle_i, circle_j;
	int index = 0;
	GLfloat increment = 360.0f / resolution;

	newSphere.ID = object_counter++;
	newSphere.type = 0;
	newSphere.num_vertices =(int)( 3 * (resolution * (2 + (2 * ((180.0f / increment) - 2)))));

	newSphere.position_array = (glm::vec4*)malloc(sizeof(glm::vec4) * (newSphere.num_vertices));

	for (sphere_it = 0; sphere_it < 180.0f; sphere_it += increment) {
		sphere_i =(GLfloat)(sphere_it * M_PI / 180.0f);
		sphere_j =(GLfloat)((sphere_it + increment) * M_PI / 180.0f);


		if (sphere_it == 0) {
			for (circle_it = 0; circle_it < 360.0f; circle_it += increment) {
				circle_i = (GLfloat)(circle_it * M_PI / 180.0f);
				circle_j = (GLfloat)((circle_it + increment) * M_PI / 180.0f);

				newSphere.position_array[index] = glm::vec4(0.0, radius * cos(sphere_i), 0.0, 1.0);
				newSphere.position_array[index + 1] = glm::vec4((radius * cos(circle_j)) * sin(sphere_j), radius * cos(sphere_j), (radius * sin(circle_j) * sin(sphere_j)), 1.0);
				newSphere.position_array[index + 2] = glm::vec4((radius * cos(circle_i)) * sin(sphere_j), radius * cos(sphere_j), (radius * sin(circle_i) * sin(sphere_j)), 1.0);
				index += 3;
			}
		}

		if (sphere_it != 0 && sphere_it + increment < 180.0f) {
			for (band_it = 0; band_it < 360.0f; band_it += increment) {
				band_i = (GLfloat)(band_it * M_PI / 180.0f);
				band_j = (GLfloat)((band_it + increment) * M_PI / 180.0f);

				newSphere.position_array[index] = glm::vec4((radius * cos(band_i)) * sin(sphere_i), radius * cos(sphere_i), (radius * sin(band_i) * sin(sphere_i)), 1.0);
				newSphere.position_array[index + 1] = glm::vec4((radius * cos(band_j)) * sin(sphere_j), radius * cos(sphere_j), (radius * sin(band_j)) * sin(sphere_j), 1.0);
				newSphere.position_array[index + 2] = glm::vec4((radius * cos(band_i)) * sin(sphere_j), radius * cos(sphere_j), (radius * sin(band_i)) * sin(sphere_j), 1.0);
				newSphere.position_array[index + 3] = glm::vec4((radius * cos(band_i)) * sin(sphere_i), radius * cos(sphere_i), (radius * sin(band_i)) * sin(sphere_i), 1.0);
				newSphere.position_array[index + 4] = glm::vec4((radius * cos(band_j)) * sin(sphere_i), radius * cos(sphere_i), (radius * sin(band_j)) * sin(sphere_i), 1.0);
				newSphere.position_array[index + 5] = glm::vec4((radius * cos(band_j)) * sin(sphere_j), radius * cos(sphere_j), (radius * sin(band_j)) * sin(sphere_j), 1.0);
				index += 6;
			}
		}

		if (sphere_it + increment >= 180.0f) {
			for (circle_it = 0; circle_it < 360.0f; circle_it += increment) {
				circle_i = (GLfloat)(circle_it * M_PI / 180.0f);
				circle_j = (GLfloat)((circle_it + increment) * M_PI / 180.0f);

				newSphere.position_array[index] = glm::vec4(0.0, radius * cos(sphere_j), 0.0, 1.0);
				newSphere.position_array[index + 1] = glm::vec4((radius * cos(circle_i)) * sin(sphere_i), radius * cos(sphere_i), (radius * sin(circle_i)) * sin(sphere_i), 1.0);
				newSphere.position_array[index + 2] = glm::vec4((radius * cos(circle_j)) * sin(sphere_i), radius * cos(sphere_i), (radius * sin(circle_j)) * sin(sphere_i), 1.0);
				index += 3;
			}
		}

	}

	switch (color_type) {
	case 0:
		newSphere.color_array = genRandomTriangleColors(newSphere.num_vertices);
		break;
	case 1:
		newSphere.color_array = genRandomTriangleColorsAlpha(newSphere.num_vertices);
		break;
	case 2:
		newSphere.color = color;
		newSphere.color_array = genRandomTriangleColorsSimilar(newSphere.num_vertices, newSphere.color);
		break;
	default:
		break;
	}

	glGenVertexArrays(1, &newSphere.VAO);
	glBindVertexArray(newSphere.VAO);
	glGenBuffers(1, &newSphere.VBO);
	glBindBuffer(GL_ARRAY_BUFFER, newSphere.VBO);
	glBufferData(GL_ARRAY_BUFFER, (sizeof(glm::vec4) * 2) * newSphere.num_vertices, NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(glm::vec4) * newSphere.num_vertices, newSphere.position_array);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(glm::vec4) * newSphere.num_vertices, sizeof(glm::vec4) * newSphere.num_vertices, newSphere.color_array);
	vPosition = glGetAttribLocation(ResourceManager::getShader("simple_color_shader").Program, "vPosition");
	glEnableVertexAttribArray(vPosition);
	glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
	vColor = glGetAttribLocation(ResourceManager::getShader("simple_color_shader").Program, "vColor");
	glEnableVertexAttribArray(vColor);
	glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_FALSE, 0, (GLvoid *)(sizeof(glm::vec4) * newSphere.num_vertices));

	objectList.push_back(newSphere);

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
		newCube.color_array = genRandomTriangleColorsAlpha(newCube.num_vertices);
		break;
	case 2:
		newCube.color = color;
		newCube.color_array = genRandomTriangleColorsSimilar(newCube.num_vertices, newCube.color);
		break;
	default:
		break;
	}

	glGenVertexArrays(1, &newCube.VAO);
	glBindVertexArray(newCube.VAO);
	glGenBuffers(1, &newCube.VBO);
	glBindBuffer(GL_ARRAY_BUFFER, newCube.VBO);
	glBufferData(GL_ARRAY_BUFFER, (sizeof(glm::vec4) * 2) * newCube.num_vertices, NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(glm::vec4) * newCube.num_vertices, newCube.position_array);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(glm::vec4) * newCube.num_vertices, sizeof(glm::vec4) * newCube.num_vertices, newCube.color_array);
	vPosition = glGetAttribLocation(ResourceManager::getShader("simple_color_shader").Program, "vPosition");
	glEnableVertexAttribArray(vPosition);
	glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
	vColor = glGetAttribLocation(ResourceManager::getShader("simple_color_shader").Program, "vColor");
	glEnableVertexAttribArray(vColor);
	glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_FALSE, 0, (GLvoid *)(sizeof(glm::vec4) * newCube.num_vertices));

	objectList.push_back(newCube);

	return newCube.ID;
}

glm::vec4* Graphics::genRandomTriangleColors(int num_vertices)
{
	GLfloat r, g, b;
	int index = 0, it;

	srand((unsigned int)time(0));

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

glm::vec4* Graphics::genRandomTriangleColorsAlpha(int num_vertices)
{
	GLfloat r, g, b, a;
	int index = 0, it;

	srand((unsigned int)time(0));

	glm::vec4 *colors = (glm::vec4 *)malloc(sizeof(glm::vec4) * num_vertices);

	for (it = 0; it < num_vertices / 3; it++)
	{
		r = rand() / (float)RAND_MAX;
		g = rand() / (float)RAND_MAX;
		b = rand() / (float)RAND_MAX;
		a = rand() / (float)RAND_MAX;

		colors[index] = glm::vec4(r, g, b, a);
		colors[index + 1] = glm::vec4(r, g, b, a);
		colors[index + 2] = glm::vec4(r, g, b, a);
		index += 3;
	}

	return colors;
}


glm::vec4* Graphics::genRandomTriangleColorsSimilar(int num_vertices, glm::vec4 color)
{
	GLfloat r, g, b, a, modifying_color;
	int index = 0, it;

	srand((unsigned int)time(0));

	glm::vec4 *colors = (glm::vec4 *)malloc(sizeof(glm::vec4) * num_vertices);

	for (it = 0; it < num_vertices / 3; it++)
	{
		modifying_color = (rand() / (float)RAND_MAX) - 0.5f;
		r = color.x + modifying_color * 0.1f;
		g = color.y + modifying_color * 0.1f;
		b = color.z + modifying_color * 0.1f;
		a = color.w + modifying_color * 0.1f;

		colors[index] = glm::vec4(r, g, b, a);
		colors[index + 1] = glm::vec4(r, g, b, a);
		colors[index + 2] = glm::vec4(r, g, b, a);
		index += 3;
	}

	return colors;
}

int Graphics::recolorQuad(int ID, glm::vec4 color)
{
	int i;
	std::list<GraphicsObject>::iterator it = std::find_if(objectList.begin(), objectList.end(), [&ID](GraphicsObject const& gObj) { return gObj.ID == ID; });
	if (it != objectList.end()) {
		it->color = color;
		for (i = 0; i < it->num_vertices; i++) {
			it->color_array[i] = color;
		}
		return 1;
	}
	else {
		return 0;
	}
}

int Graphics::retextureQuad(int ID, const GLchar *file, std::string texture_ID)
{
	std::list<GraphicsObject>::iterator it = std::find_if(objectList.begin(), objectList.end(), [&ID](GraphicsObject const& gObj) { return gObj.ID == ID; });
	if (it != objectList.end()) {
		it->texture_ID = texture_ID;
		ResourceManager::loadTexture(file, texture_ID);
		return 1;
	}
	else {
		return 0;
	}
}

int Graphics::removeObject(int ID)
{
	std::list<GraphicsObject>::iterator it = std::find_if(objectList.begin(), objectList.end(), [&ID](GraphicsObject const& gObj) { return gObj.ID == ID; });
	if (it != objectList.end()) {
		//glDeleteVertexArrays(1, it->VAO);
		//glDeleteBuffers(1, it->VBO);
		objectList.erase(it);
		return 1;
	}
	else {
		return 0;
	}
}

int Graphics::translateObjectByPixel(int ID, int x, int y, GLfloat z)
{
	GLfloat i, j;
	i = ((GLfloat)(x - SCREEN_WIDTH / 2)) / ((GLfloat)(SCREEN_WIDTH / 2));
	j = ((GLfloat)(SCREEN_HEIGHT / 2 - y)) / ((GLfloat)(SCREEN_HEIGHT / 2));

	std::list<GraphicsObject>::iterator it = std::find_if(objectList.begin(), objectList.end(), [&ID](GraphicsObject const& gObj) { return gObj.ID == ID; });
	if (it != objectList.end()) {
		it->position = glm::vec3(i, j, z);
		it->ctm = glm::translate(it->ctm, glm::vec3(i, j, z));
		return 1;
	}
	else {
		return 0;
	}
}

int Graphics::transformCtm(int ID, glm::mat4 transform)
{
	std::list<GraphicsObject>::iterator it = std::find_if(objectList.begin(), objectList.end(), [&ID](GraphicsObject const& gObj) { return gObj.ID == ID; });
	if (it != objectList.end()) {
		it->ctm = it->ctm * transform;
		return 1;
	}
	else {
		return 0;
	}
}

glm::vec3 Graphics::rotateRandom(void)
{
	GLfloat x, y, z;

	x = rand() / (float)RAND_MAX;
	y = rand() / (float)RAND_MAX;
	z = rand() / (float)RAND_MAX;

	return glm::vec3(x, y, z);
}

int Graphics::setPosition(int ID, glm::vec3 position)
{
	std::list<GraphicsObject>::iterator it = std::find_if(objectList.begin(), objectList.end(), [&ID](GraphicsObject const& gObj) { return gObj.ID == ID; });
	if (it != objectList.end()) {
		it->position = position;
		return 1;
	}
	else {
		return 0;
	}
}

glm::vec3 Graphics::getPosition(int ID)
{
	std::list<GraphicsObject>::iterator it = std::find_if(objectList.begin(), objectList.end(), [&ID](GraphicsObject const& gObj) { return gObj.ID == ID; });
	if (it != objectList.end()) {
		return it->position;
	}
	else {
		return glm::vec3();
	}
}

glm::vec3 Graphics::getVectorFromTo(int ID1, int ID2)
{
	std::list<GraphicsObject>::iterator it1 = std::find_if(objectList.begin(), objectList.end(), [&ID1](GraphicsObject const& gObj) { return gObj.ID == ID1; });
	if (it1 != objectList.end()) {
		std::list<GraphicsObject>::iterator it2 = std::find_if(objectList.begin(), objectList.end(), [&ID2](GraphicsObject const& gObj) { return gObj.ID == ID2; });
		if (it2 != objectList.end()) {
			return it2->position - it1->position;
		}
		else {
			return glm::vec3();
		}
	}
	else {
		return glm::vec3();
	}
}

int Graphics::setIdleAnimationType(int ID, int type)
{
	std::list<GraphicsObject>::iterator it = std::find_if(objectList.begin(), objectList.end(), [&ID](GraphicsObject const& gObj) { return gObj.ID == ID; });
	if (it != objectList.end()) {
		it->idle_animation_type = type;
		return 1;
	}
	else {
		return 0;
	}
}

void Graphics::iterateSpriteAnimation(std::list<GraphicsObject>::iterator it)
{
	int i;
	if (++it->texture_sheet_it >= it->texture_sheet_size) {
		it->texture_sheet_it = 0;
	}

	for (i = 0; i < it->num_vertices; i++) {
		it->texture_array[i] = glm::vec2((GLfloat)(it->texture_sheet_it + quadTexCoords[i].x) * (1.0 / it->texture_sheet_size), quadTexCoords[i].y);
	}

	glBindVertexArray(it->VAO);
	glBindBuffer(GL_ARRAY_BUFFER, it->VBO);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(glm::vec4) * it->num_vertices, sizeof(glm::vec2) * it->num_vertices, it->texture_array);
	vTexCoords = glGetAttribLocation(ResourceManager::getShader("simple_texture_shader").Program, "vTexCoords");
	glEnableVertexAttribArray(vTexCoords);
	glVertexAttribPointer(vTexCoords, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), (GLvoid*)(sizeof(glm::vec4) * it->num_vertices));
}

int Graphics::setIdleAnimationMotion(int ID, glm::mat4 motion)
{
	std::list<GraphicsObject>::iterator it = std::find_if(objectList.begin(), objectList.end(), [&ID](GraphicsObject const& gObj) { return gObj.ID == ID; });
	if (it != objectList.end()) {
		it->idle_animation_motion = motion;
		return 1;
	}
	else {
		return 0;
	}
}

int Graphics::setAnimation(int ID, int type)
{
	std::list<GraphicsObject>::iterator it = std::find_if(objectList.begin(), objectList.end(), [&ID](GraphicsObject const& gObj) { return gObj.ID == ID; });
	if (it != objectList.end()) {
		it->animation_type = type;
		return 1;
	}
	else {
		return 0;
	}
}

int Graphics::setAnimationMotion(int ID, glm::mat4 motion)
{
	std::list<GraphicsObject>::iterator it = std::find_if(objectList.begin(), objectList.end(), [&ID](GraphicsObject const& gObj) { return gObj.ID == ID; });
	if (it != objectList.end()) {
		it->animation_motion = motion;
		return 1;
	}
	else {
		return 0;
	}
}

int Graphics::setAnimationFrameMax(int ID, int frame_max)
{
	std::list<GraphicsObject>::iterator it = std::find_if(objectList.begin(), objectList.end(), [&ID](GraphicsObject const& gObj) { return gObj.ID == ID; });
	if (it != objectList.end()) {
		it->animation_frame_max = frame_max;
		return 1;
	}
	else {
		return 0;
	}
}

int Graphics::setAnimationMultiStep(int ID, int animation_size, glm::mat4* animation_motion_array, int* animation_frame_max_array)
{
	std::list<GraphicsObject>::iterator it = std::find_if(objectList.begin(), objectList.end(), [&ID](GraphicsObject const& gObj) { return gObj.ID == ID; });
	if (it != objectList.end()) {
		it->animation_size = animation_size;
		it->animation_motion_array = animation_motion_array;
		it->animation_frame_max_array = animation_frame_max_array;
		return 1;
	}
	else {
		return 0;
	}
}

void Graphics::animateMotion(std::list<GraphicsObject>::iterator it)
{
	if (it->animation_frame >= it->animation_frame_max) {
		it->animation_type = 0;
		it->animation_frame = 0;
		it->animation_motion = { { 1.0, 0.0, 0.0, 0.0 },{ 0.0, 1.0, 0.0, 0.0 },{ 0.0, 0.0, 1.0, 0.0 },{ 0.0, 0.0, 0.0, 1.0 } };
	}
	else {
		it->animation_frame++;
		it->ctm = it->ctm * it->animation_motion;
	}
}

void Graphics::animateMotionConsume(std::list<GraphicsObject>::iterator it)
{
	if (it->animation_frame >= it->animation_frame_max) {
		eraseBuffer.push_back(it->ID);
	}
	else {
		it->animation_frame++;
		it->ctm = it->ctm * it->animation_motion;
	}
}

void Graphics::animateMultiStep(std::list<GraphicsObject>::iterator it)
{
	if (it->animation_frame >= it->animation_frame_max_array[it->animation_it]) {
		if (it->animation_it >= it->animation_size) {
			eraseBuffer.push_back(it->ID);
			return;
		}
		else {
			it->animation_frame = 0;
			it->animation_it++;
		}
	}
	else {
		it->animation_frame++;
	}
	it->ctm = it->ctm * it->animation_motion_array[it->animation_it];
}

void Graphics::animateMultiStepConsume(std::list<GraphicsObject>::iterator it)
{
	if (it->animation_frame >= it->animation_frame_max_array[it->animation_it]) {
		if (it->animation_it >= it->animation_size) {
			it->animation_type = 0;
			it->animation_frame = 0;
			it->animation_it = 0;
			free(it->animation_frame_max_array);
			free(it->animation_motion_array);
			return;
		}
		else {
			it->animation_frame = 0;
			it->animation_it++;
		}
	}
	else {
		it->animation_frame++;
	}
	it->ctm = it->ctm * it->animation_motion_array[it->animation_it];
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