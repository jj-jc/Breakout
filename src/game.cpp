#include "game.hpp"
#include "resource_manager.hpp"
#include "sprite_renderer.hpp"

// Game-related State data
SpriteRenderer *Renderer;

// Constructor/Destructor
Game::Game(unsigned int width, unsigned int height)
    : _state(GAME_ACTIVE), _keys(), _width(width), _height(height)
{

}

Game::~Game()
{
    delete Renderer;
}

// Functionalities
// Initialize game state (load all resources: Shaders/textures/levels)
void Game::Init()
{
    // Load shaders
    ResourceManager::LoadShader("C:/Dev/Breakout/src/sprite.vs", "C:/Dev/Breakout/src/sprite.fs", nullptr, "sprite");
    // Configure shaders
    glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(this->_width),
        static_cast<float>(this->_height), 0.0f, -1.0f, 1.0f);
    ResourceManager::GetShader("sprite").Use().SetInteger("image", 0);
    ResourceManager::GetShader("sprite").Use().SetMatrix4("projection", 
        projection);
    // Set render-specific controls
    Renderer = new SpriteRenderer(ResourceManager::GetShader("sprite"));
    // Load textures
    ResourceManager::LoadTexture2D("C:/Dev/Breakout/resources/textures/awesomeface.png", true, "face");
}
// Process input as stored within _keys parameter
void Game::ProcessInput(double dt)
{

} 
// Update all game events: player/ball movements
void Game::Update(double dt)
{

}
// Render the game Frame
void Game::Render()
{
    Renderer->DrawSprite(ResourceManager::GetTexture2D("face"), 
        glm::vec2(200.0f, 200.0f), glm::vec2(300.0f, 400.0f), 45.0f,
        glm::vec3(0.0f, 1.0f, 0.0f));
}