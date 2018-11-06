#include "../Headers/Graphics.h"

void Graphics::init(void)
{
	GLuint program = Shader::initShader("../vshader.glsl", "../fshader.glsl");
	glUseProgram(program);

	glm::vec4 *shape_vertices;
	glm::vec4 *shape_colors;

	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	GLuint buffer;
	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec4) * 2 * num_vertices, NULL, GL_STATIC_DRAW);

	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(glm::vec4) * num_vertices, shape_vertices);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(glm::vec4) * num_vertices, sizeof(glm::vec4) * num_vertices, shape_colors);

	GLuint vPosition = glGetAttribLocation(program, "vPosition");
	glEnableVertexAttribArray(vPosition);
	glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

	GLuint vColor = glGetAttribLocation(program, "vColor");
	glEnableVertexAttribArray(vColor);
	glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_FALSE, 0, (GLvoid *)(sizeof(glm::vec4) * num_vertices));

	ctm_location = glGetUniformLocation(program, "ctm");

	glEnable(GL_DEPTH_TEST);
	glClearColor(1.0, 1.0, 1.0, 1.0);
	glDepthRange(1, 0);

}