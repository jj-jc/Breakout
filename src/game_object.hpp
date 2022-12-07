#ifndef GAME_OBJECT_H_
#define GAME_OBJECT_H_

#include <glad/glad.h>
#include <glm/glm.hpp>

#include "texture.hpp"
#include "sprite_renderer.hpp"

class GameObject
{
    public:
        // Object status
        glm::vec2 Position, Size, Velocity;
        glm::vec3 Color;
        float Rotation;
        bool Solid, Destroyed;
        // Render state
        Texture2D Sprite;
        
        // Constructors
        GameObject();
        GameObject(glm::vec2 pos, glm::vec2 size, Texture2D sprite, glm::vec3 color = glm::vec3(1.0), glm::vec2 vel = glm::vec2(0.0f, 0.0f));
        // Draw Sprite
        virtual void Draw(SpriteRenderer &renderer);
        virtual void SetPosition(glm::vec2 Position);
        // virtual void Reset(glm::vec2 Position);
};

#endif /* GAME_OBJECT_H_ */