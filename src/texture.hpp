#ifndef TEXTURE_H
#define TEXTURE_H

#include <glad/glad.h>

class Texture2D
{
    public:
        unsigned int _ID; // ID of the texture object
        unsigned int _width, _height; // Texture image dimensions
        unsigned int _internal_Format; // Format of the texture object
        unsigned int _image_Format; // Format of the loaded image
        unsigned int _wrap_S; // Wrapping mode on S axis 
        unsigned int _wrap_T; // Wrapping mode on T axis
        unsigned int _filter_Min; // Filtering mode if texture pixels < screen pixels
        unsigned int _filter_Max; // Filtering mode if texture pixels > screen pixels
        // Constructor
        Texture2D();
        // Functionalities
        void Generate(unsigned int width, unsigned int height, unsigned char* data); // Generates texture from image data
        void Bind() const; // Binds the texture as the current active GL_TEXTURE_2D target.
};

#endif