#include "texture.hpp"

Texture2D::Texture2D()
    : _width(0), _height(0), _internal_Format(GL_RGB), _image_Format(GL_RGB), _wrap_S(GL_REPEAT),
      _wrap_T(GL_REPEAT), _filter_Min(GL_LINEAR), _filter_Max(GL_LINEAR)
{
    // Create a texture container, return the ID
    glGenTextures(1, &this->_ID);
}

// Functionalities
void Texture2D::Generate(unsigned int width, unsigned int height, unsigned char *data)
{
    this->_width = width;
    this->_height = height;
    //  Generate the texture Object
    glBindTexture(GL_TEXTURE_2D, this->_ID);
    glTexImage2D(GL_TEXTURE_2D, 0, this->_internal_Format, width, height, 0, this->_image_Format,
                 GL_UNSIGNED_BYTE, data);
    // Set texture configurations
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, this->_wrap_S);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, this->_wrap_T);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, this->_filter_Min);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, this->_filter_Max);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture2D::Bind() const
{
    glBindTexture(GL_TEXTURE_2D, this->_ID);
}