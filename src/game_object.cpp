#include "game_object.hpp"

GameObject::GameObject()
    : Position(0.0f, 0.0f), Size(1.0f, 1.0f), Velocity(0.0f, 0.0f), Color(1.0f), Rotation(0.0f), Sprite(),
      Solid(false), Destroyed(false)
{
}

GameObject::GameObject(glm::vec2 pos, glm::vec2 size, Texture2D sprite, glm::vec3 color, glm::vec2 vel)
    : Position(pos), Size(size), Velocity(vel), Color(color), Rotation(0.0f), Sprite(sprite), Solid(false), Destroyed(false)
{
}

void GameObject::Draw(SpriteRenderer &renderer)
{
    renderer.DrawSprite(this->Sprite, this->Position, this->Size, this->Rotation, this->Color);
}

void GameObject::SetPosition(glm::vec2 Position)
{
    this->Position = Position;
}