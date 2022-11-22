#include "resource_manager.hpp"

#include <iostream>
#include <sstream>
#include <fstream>

#include "stb_image.h"

// Instantiate static members
ResourceManager ResourceManager::s_Instance;
std::map<std::string, Shader> ResourceManager::_shaders_map;
std::map<std::string, Texture2D> ResourceManager::_textures2D_map;

Shader &ResourceManager::LoadShader(const char* vShaderFile, const char* fShaderFile, const char* gShaderFile, std::string name)
{
    _shaders_map[name] = loadShaderFromFile(vShaderFile, fShaderFile, gShaderFile);
    return _shaders_map[name];
}

Texture2D &ResourceManager::LoadTexture2D(const char* file, bool alpha, std::string name)
{
    _textures2D_map[name] = loadTexture2D(file, alpha);
    return _textures2D_map[name];
}

Shader &ResourceManager::GetShader(std::string name)
{
    return _shaders_map[name];
}

Texture2D &ResourceManager::GetTexture2D(std::string name)
{
    return _textures2D_map[name];
}

void ResourceManager::Clear()
{
    for(auto it : _shaders_map)
        glDeleteProgram(it.second._ID);
    for(auto it : _textures2D_map)
        glDeleteTextures(1, &it.second._ID);
}

Shader ResourceManager::loadShaderFromFile(const char* vShaderFile,  const char* fShaderFile, const char* gShaderFile)
{
    std::string vertexCode;
    std::string fragmentCode;
    std::string geometryCode;
    try
    {
        // Open Files
        std::ifstream vertexShaderFile(vShaderFile);
        std::ifstream fragmentShaderFile(fShaderFile);
        std::stringstream vShaderStream, fShaderStream;
        // Read file's buffers content into streams
        vShaderStream << vertexShaderFile.rdbuf();
        fShaderStream << fragmentShaderFile.rdbuf();
        // Close file handlers
        vertexShaderFile.close();
        fragmentShaderFile.close();
        // Convert stream into string
        vertexCode = vShaderStream.str();
        fragmentCode = fShaderStream.str();
        // The same process to geometry file
        if(gShaderFile != nullptr)
        {
            std::ifstream geometryShaderFile(gShaderFile);
            std::stringstream gShaderStream;
            gShaderStream << geometryShaderFile.rdbuf();
            geometryShaderFile.close();
            geometryCode = gShaderStream.str();
        }
    }
    catch (std::exception e)
    {
        std::cout << "ERROR::RESOURCEMANAGER: Failed to read shader files" << std::endl;
    }
    const char* vShaderCode = vertexCode.c_str();
    const char* fShaderCode = fragmentCode.c_str();
    const char* gShaderCode = geometryCode.c_str();
    Shader shader;
    shader.Compile(vShaderCode, fShaderCode, gShaderFile != nullptr ? gShaderCode : nullptr);
    return shader;
}

Texture2D ResourceManager::loadTexture2D(const char* file, bool alpha)
{
    // Create texture object
    Texture2D texture;
    if(alpha)
    {
        texture._internal_Format = GL_RGBA;
        texture._image_Format = GL_RGBA;
    }
    // Load image
    int width, height, nrChannels;
    unsigned char* data = stbi_load(file, &width, &height, &nrChannels, 0);
    // Generate texture2D
    texture.Generate(width, height, data);
    stbi_image_free(data);
    return texture;
}