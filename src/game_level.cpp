#include "game_level.hpp"

#include <fstream>
#include <sstream>

void GameLevel::Load(const char* file, unsigned int levelWidth, unsigned int levelHeight)
{
    // A number of 0: no brick.
    // A number of 1: a solid brick,
    // A number higher than 1: a destoyable brick, the difference is the color.

    // Clear the data
    this->Bricks.clear();
    // Load from file
    unsigned int tileCode;
    GameLevel level;
    std::string line;
    std::ifstream fstream(file);
    std::vector<std::vector<unsigned int>> tileData;
    if(fstream)
    {
        while(std::getline(fstream, line)) // read each line from level file
        {
            std::istringstream sstream(line);
            std::vector<unsigned int> row;
            while(sstream >> tileCode) // read each word separated by spaces
                row.push_back(tileCode);
            tileData.push_back(row);
        }
        if(tileData.size() > 0)
            this->init(tileData, levelWidth, levelHeight);
    }
}

void GameLevel::init(std::vector<std::vector<unsigned int>> tileData, unsigned int levelWidth, unsigned int levelHeight)
{
    // Calculate dimensions for every brick ( everyone is the same dimension)
    unsigned int height = static_cast<unsigned int>(tileData.size());
    unsigned int width = static_cast<unsigned int>(tileData[0].size());
    float unit_width = levelWidth / static_cast<float>(width);
    float unit_height = levelHeight / static_cast<float>(height);
    //TODO: change doble loop to a simple one
    unsigned int brick_value = 0;
    glm::vec3 color = glm::vec3(1.0f);
    for(unsigned int y = 0; y < height; ++y)
    {
        for(unsigned int x = 0; x < width; ++x)
        {
            // Check block type from level data. 
            brick_value = tileData[y][x];
            if(brick_value == 1) // Solid
            {
                glm::vec2 pos(unit_width * x, unit_height * y);
                glm::vec2 size(unit_width, unit_height);
                GameObject brick(pos, size, ResourceManager::GetTexture2D("block_solid"), glm::vec3(0.8f, 0.8f, 0.7f));
                brick.Solid = true;
                Bricks.push_back(brick);
            }
            else if(brick_value > 1)
            {
                switch (brick_value)
                {
                case 2:
                    color = glm::vec3(0.2f, 0.6f, 1.0f);
                    break;
                case 3:
                    color = glm::vec3(0.0f, 0.7f, 0.0f);
                    break;
                case 4:
                    color = glm::vec3(0.8f, 0.8f, 0.4f);
                    break;
                case 5:
                    color = glm::vec3(1.0f, 0.5f, 0.0f);
                    break;                
                default:
                    color = glm::vec3(1.0f);
                    break;
                }
                glm::vec2 pos(unit_width * x, unit_height * y);
                glm::vec2 size(unit_width, unit_height);
                Bricks.push_back(GameObject(pos, size, ResourceManager::GetTexture2D("block"), color));
            }
        }
    }
}

void GameLevel::Draw(SpriteRenderer &renderer)
{
    for(GameObject &tile : this->Bricks)
    {
        if(!tile.Destroyed)
            tile.Draw(renderer);
    }
}

bool GameLevel::IsCompleted()
{
    for(GameObject &tile : this->Bricks)
        if(!tile.Solid && !tile.Destroyed)
            return false;
    return true;
}