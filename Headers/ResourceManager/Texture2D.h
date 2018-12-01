
#ifndef _____TEXTURE2D_H_____
#define _____TEXTURE2D_H_____

#include <GL/glew.h>
#include <glm/vec4.hpp>

// Texture2D is able to store and configure a texture in OpenGL.
class Texture2D
{
public:
	// Holds the ID of the texture object, used for all texture operations to reference to this particlar texture
	GLuint ID;
	// Texture image dimensions
	GLuint width, height; // Width and height of loaded image in pixels
	// Texture Format
	GLuint internal_format; // Format of texture object
	GLuint image_format; // Format of loaded image
	// Texture configuration
	GLuint wrap_S; // Wrapping mode on S axis
	GLuint wrap_T; // Wrapping mode on T axis
	GLuint filter_min; // Filtering mode if texture pixels are smaller than the screen pixels
	GLuint filter_max; // Filtering mode if texture pixels are larger than the screen pixels

	GLuint data_type;	// Data type of texture
	// Constructor (sets default texture modes)
	Texture2D();
	// Generates texture from image data
	void generate(GLuint width, GLuint height, unsigned char* data);
	// Binds the texture as the current active GL_TEXTURE_2D texture object
	void bind() const;
	void setBorderColor(glm::vec4 border);
	void setWrapModes(GLuint wrapS, GLuint wrapT);
};
#endif