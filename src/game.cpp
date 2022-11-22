#include "game.hpp"
#include "resource_manager.hpp"
#include "sprite_renderer.hpp"

// Game-related State data
SpriteRenderer *Renderer;

// Paddle Object
GameObject *Paddle;
const glm::vec2 PADDLE_SIZE(100.0f, 20.f);
const float PADDLE_VELOCITY(500.0f);

// Constructor/Destructor
Game::Game(unsigned int width, unsigned int height)
    : State(GAME_ACTIVE), Keys(), Width(width), Height(height)
{

}

Game::~Game()
{
    delete Renderer; // TODO: call to the destructor to correctly free the data
    delete Paddle;
}

// Functionalities
// Initialize game state (load all resources: Shaders/textures/levels)
void Game::Init()
{
    // Load shaders
    ResourceManager::LoadShader("src/sprite.vs", "src/sprite.fs", nullptr, "sprite");
    // Configure shaders
    glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(this->Width),
        static_cast<float>(this->Height), 0.0f, -1.0f, 1.0f);
    // ResourceManager::GetShader("sprite").Use().SetInteger("image", 0);
    ResourceManager::GetShader("sprite").Use().SetMatrix4("projection", 
        projection);
    // Set render-specific controls
    Renderer = new SpriteRenderer(ResourceManager::GetShader("sprite"));
    // Load textures
    ResourceManager::LoadTexture2D("resources/textures/awesomeface.png", true, "face");
    ResourceManager::LoadTexture2D("C:/Dev/Breakout/resources/textures/background.jpg", false, "background"); // El jpg no tiene alpha y por tanto da error si se pone a true.
    ResourceManager::LoadTexture2D("resources/textures/block.png", false, "block");
    ResourceManager::LoadTexture2D("resources/textures/block_solid.png", false, "block_solid");
    ResourceManager::LoadTexture2D("resources/textures/paddle.png", true, "paddle");
    // Load levels
    GameLevel one;
    one.Load("resources/levels/one.lvl", Width, Height / 2);
    Levels.push_back(one);
    GameLevel two;
    two.Load("resource/levels/two.lvl", Width, Height / 2);
    Levels.push_back(two);
    GameLevel three;
    three.Load("resources/levels/three.lvl", Width, Height /2);
    Levels.push_back(three);
    GameLevel four;
    four.Load("resources/levels/four.lvl", Width, Height / 2);
    Levels.push_back(four);
    Current_level = 0;
    // Configure paddle Object
    glm::vec2 paddlePos = glm::vec2(Width / 2.0f - PADDLE_SIZE.x / 2.0f, Height - PADDLE_SIZE.y);
    Paddle = new GameObject(paddlePos, PADDLE_SIZE, ResourceManager::GetTexture2D("paddle"));
}

// Process input as stored within Keys parameter
void Game::ProcessInput(double dt)
{
    if(State == GAME_ACTIVE)
    {
        float velocity = PADDLE_VELOCITY * dt;
        // Move playerboard
        if(Keys[GLFW_KEY_A])
        {
            if(Paddle->Position.x >= 0.0f)
                Paddle->Position.x -= velocity;
        }
        if(Keys[GLFW_KEY_D])
        {
            if(Paddle->Position.x <= Width - Paddle->Size.x)
                Paddle->Position.x += velocity;
        }
    }
} 

// Update all game events: player/ball movements
void Game::Update(double dt)
{

}

// Render the game Frame
void Game::Render()
{
    if(State == GAME_ACTIVE)
    {
        // Draw background
        Renderer->DrawSprite(ResourceManager::GetTexture2D("background"), glm::vec2(0.0f, 0.0f), glm::vec2(Width, Height), 0.0f, glm::vec3(0.0f, 1.0f, 0.0f));
        // Draw level
        Levels[Current_level].Draw(*Renderer);
        // Draw Paddle Object
        Paddle->Draw(*Renderer);
    }
}