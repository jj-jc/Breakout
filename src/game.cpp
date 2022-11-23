#include "game.hpp"
#include "resource_manager.hpp"
#include "sprite_renderer.hpp"
#include "ball_object.hpp"

// Game-related State data
SpriteRenderer* Renderer;

// Paddle Object
GameObject* Paddle;
const glm::vec2 PADDLE_SIZE(100.0f, 20.f);
const float PADDLE_VELOCITY(500.0f);

// Ball Object
BallObject* Ball;
const float BALL_RADIUS(12.5f);
const glm::vec2 INITIAL_BALL_VELOCITY(100.0f, -350.f);

// Function declarations
bool checkCollision(GameObject& one, GameObject& two);
bool checkCollision(BallObject& one, GameObject& two);


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
    // Createing BallObject
    glm::vec2 ballPos = paddlePos + glm::vec2(PADDLE_SIZE.x / 2.0f - BALL_RADIUS, -BALL_RADIUS * 2.0f);
    Ball = new BallObject(ballPos, BALL_RADIUS, INITIAL_BALL_VELOCITY, ResourceManager::GetTexture2D("face"));
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
        if(Keys[GLFW_KEY_SPACE])
            Ball->Stuck = false;
        // if(Keys[GLFW_KEY_R])
            // Ball->Reset();
    }
} 

// Update all game events: player/ball movements
void Game::Update(double dt)
{
    Ball->Move(dt, this->Width);
    this->DoCollisions();
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
        // Draw Ball Object
        Ball->Draw(*Renderer);
    }
}

void Game::DoCollisions()
{
    for(GameObject& box : Levels[Current_level].Bricks)
    {
        if(!box.Destroyed)
        {
            if(checkCollision(*Ball, box))
            {
                if(!box.Solid)
                {
                    box.Destroyed = true;

                }
            }
        }
    }
}

// Checking for an AABB collision
bool checkCollision(GameObject &one, GameObject &two)
{   
    // Overlaps in X axis 
    bool collisionX = (one.Position.x + one.Size.x) >= (two.Position.x) && (two.Position.x + two.Size.x) >= (one.Position.x);
    // Overlaps in Y in axis
    bool collisionY = (one.Position.y + one.Size.y) >= (two.Position.y) && (two.Position.y + two.Size.y) >= (one.Position.y);
    // Overlaping X and Y is when the collision is produced
    return collisionX && collisionY;
}

bool checkCollision(BallObject& one, GameObject& two) // AABB -Circle
{
    // get center point circle first
    glm::vec2 circle_center(one.Position + one.Radius);
    // get ABB center
    glm::vec2 aabb_half_dimensions(two.Size.x / 2.0f, two.Size.y / 2.0f);
    glm::vec2 aabb_center(two.Position + aabb_half_dimensions);
    // get the difference vector between both centers
    glm::vec2 difference = circle_center - aabb_center;
    glm::vec2 clamped = glm::clamp(difference, -aabb_half_dimensions, aabb_half_dimensions);
    // add clamped value to aabb_center and get the value closest to circle
    glm::vec2 closest = aabb_center + clamped;
    difference = closest - circle_center;
    return glm::length(difference) < one.Radius;
}

