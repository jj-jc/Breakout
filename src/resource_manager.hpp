#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H

#include <map>
#include <string>

#include <glad/glad.h>

#include "texture.hpp"
#include "shader.hpp"

class ResourceManager
{
    public:
        // resources storage
        static std::map<std::string, Shader> _shaders_map;
        static std::map<std::string, Texture2D> _textures2D_map;
        // Singleton functions
        ResourceManager(const ResourceManager&) = delete; // avoids the copy constructor
        static ResourceManager& get() { return s_Instance;} // returns the only reference to a ResourceManager
        // Functionalities
        static Shader &LoadShader(const char* vShaderFile, const char* fShaderFile, const char* gShaderFile, std::string name); // Create Shader program
        static Texture2D &LoadTexture2D(const char* file, bool alpha, std::string name);
        static Shader &GetShader(std::string name);
        static Texture2D &GetTexture2D(std::string name);
        static void Clear();
    private:
        ResourceManager() {};
        static ResourceManager s_Instance;
        static Shader loadShaderFromFile(const char* vShaderFile, const char* fShaderFile, const char* gShader = nullptr);
        static Texture2D loadTexture2D(const char* file, bool alpha);
};
#endif