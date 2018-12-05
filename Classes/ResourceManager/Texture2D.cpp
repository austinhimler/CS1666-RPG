
#include "../../Headers/ResourceManager/Texture2D.h"

Texture2D::Texture2D() : width(0), height(0), internal_format(GL_RGB), image_format(GL_RGB), wrap_S(GL_REPEAT), wrap_T(GL_REPEAT), filter_min(GL_LINEAR_MIPMAP_LINEAR), filter_max(GL_LINEAR), data_type(GL_UNSIGNED_BYTE)
{
    glGenTextures(1, &this->ID);
}

void Texture2D::generate(GLuint width, GLuint height, unsigned char* data)
{
    this->width = width;
    this->height = height;
    // Create Texture
    glBindTexture(GL_TEXTURE_2D, this->ID);
    glTexImage2D(GL_TEXTURE_2D, 0, this->internal_format, width, height, 0, this->image_format, data_type, data);
    glGenerateMipmap(GL_TEXTURE_2D);  //Generate num_mipmaps number of mipmaps here.

    // Generate MipMaps
    glGenerateMipmap(GL_TEXTURE_2D);

    // Anisotropic Filtering
    float aniso = 0.0f;
    glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &aniso);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, aniso);

    // Set Texture wrap and filter modes
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, this->wrap_S);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, this->wrap_T);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, this->filter_min);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, this->filter_max);
    // Unbind texture
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture2D::bind() const
{
    glBindTexture(GL_TEXTURE_2D, this->ID);
}

void Texture2D::setWrapModes(GLuint wrapS, GLuint wrapT)
{
    glBindTexture(GL_TEXTURE_2D, this->ID);
    wrap_S = wrapS;
    wrap_T = wrapT;
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, this->wrap_S);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, this->wrap_T);
}

void Texture2D::setBorderColor(glm::vec4 border)
{
    glBindTexture(GL_TEXTURE_2D, this->ID);
    GLfloat borderColor[] = { border.x, border.y, border.z, border.w };
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
}