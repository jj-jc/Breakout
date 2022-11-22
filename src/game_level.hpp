#ifndef GAME_LEVEL_H
#define GAME_LEVEL_H

#include <vector>

#include <glad/glad.h>
#include <glm/glm.hpp>

#include "game_object.hpp"
#include "sprite_renderer.hpp"
#include "resource_manager.hpp"

class GameLevel
{
    public:
        // Level state
        std::vector<GameObject> Bricks;
        // Conestructor
        GameLevel() {}
        // Loads level from file
        void Load(const char* file, unsigned int levelWidth, unsigned int levelHeight);
        // Render level
        void Draw(SpriteRenderer &renderer);
        // Check if the level is completed (all non-solid bricks are destoyed)
        bool IsCompleted();
    private:
        // Initialize level from bricks data. levelWidth and Leveheight are from the enterily world 
        void init(std::vector<std::vector<unsigned int>> tileData, unsigned int levelWidth, unsigned int levelHeight);
};

#endif /*GAME_LEVEL_H*/