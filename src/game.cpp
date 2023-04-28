#include "game.hpp"
#include "resource_manager.hpp"
#include "sprite_renderer.hpp"
#include "ball_object.hpp"

// Game-related State data
SpriteRenderer *Renderer;

// Paddle Object
GameObject *Paddle;
const glm::vec2 PADDLE_SIZE(100.0f, 20.f);
const float PADDLE_VELOCITY(500.0f);

// Ball Object
BallObject *Ball;
const float BALL_RADIUS(12.5f);
const glm::vec2 INITIAL_BALL_VELOCITY(100.0f, -350.f);

// Function declarations
bool checkCollision(GameObject &one, GameObject &two);
Collision checkCollision(BallObject &one, GameObject &two);
Direction VectorDirection(glm::vec2 target);

// Constructor/Destructor
Game::Game(unsigned int width, unsigned int height)
    : State(GAME_ACTIVE), Keys(), Width(width), Height(height)
{
}

Game::~Game()
{
    delete Renderer; // TODO: call to the destructor to correctly free the data
    delete Paddle;
    delete Ball;
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
    three.Load("resources/levels/three.lvl", Width, Height / 2);
    Levels.push_back(three);
    GameLevel four;
    four.Load("resources/levels/four.lvl", Width, Height / 2);
    Levels.push_back(four);
    Current_level = 0; // init the curren level to level 1
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
    if (State == GAME_ACTIVE)
    {
        float velocity = PADDLE_VELOCITY * dt;
        // Move playerboard
        if (Keys[GLFW_KEY_A])
        {
            if (Paddle->Position.x >= 0.0f)
                Paddle->Position.x -= velocity;
        }
        if (Keys[GLFW_KEY_D])
        {
            if (Paddle->Position.x <= Width - Paddle->Size.x)
                Paddle->Position.x += velocity;
        }
        if (Keys[GLFW_KEY_SPACE])
            Ball->Stuck = false;
        if (Keys[GLFW_KEY_R])
            Ball->Reset();
    }
}

// Update all game events: player/ball movements
void Game::Update(double dt)
{
    Ball->Move(dt, this->Width);
    this->DoCollisions();

    if (Ball->Position.y >= Height) // The ball reachs bottom edge
    {
        ResetLevel();
        ResetPaddle();
    }
}

// Render the game Frame
void Game::Render()
{
    if (State == GAME_ACTIVE)
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
    for (GameObject &box : Levels[Current_level].Bricks)
    {
        if (!box.Destroyed)
        {
            Collision collision = checkCollision(*Ball, box);
            if (std::get<0>(collision))
            {
                // destroy block is not solid
                if (!box.Solid)
                    box.Destroyed = true;
                // collision resolution
                Direction dir = std::get<1>(collision);
                glm::vec2 diff_vector = std::get<2>(collision);
                if (dir == LEFT || dir == RIGHT) // horizontal collision
                {
                    Ball->Velocity.x = -Ball->Velocity.x; // reverse
                    // relocate
                    float penetration = Ball->Radius - std::abs(diff_vector.x);
                    if (dir == LEFT)
                        Ball->Position.x += penetration; // move right
                    else
                        Ball->Position.x -= penetration; // move left
                }
                else
                {
                    Ball->Velocity.y = -Ball->Velocity.y;
                    float penetration = Ball->Radius - std::abs(diff_vector.y);
                    if (dir == UP)
                        Ball->Position.y -= penetration;
                    if (dir == DOWN)
                        Ball->Position.y += penetration;
                }
            }
        }
    }

    Collision result = checkCollision(*Ball, *Paddle);
    if (!Ball->Stuck && std::get<0>(result))
    {
        float centerBoard = Paddle->Position.x + Paddle->Size.x / 2.0f;
        float distance = (Ball->Position.x + Ball->Radius) - centerBoard;
        float percentage = distance / (Paddle->Size.x / 2.0f);
        float strength = 2.0f;
        glm::vec2 oldVelocity = Ball->Velocity;
        Ball->Velocity.x = INITIAL_BALL_VELOCITY.x * percentage * strength;
        Ball->Velocity.y = -1.0f * abs(Ball->Velocity.y);
        Ball->Velocity = glm::normalize(Ball->Velocity) * glm::length(oldVelocity);
    }
}

// Reset the values of bricks eleinated
void Game::ResetLevel()
{
    Levels[Current_level].Reset();
}

// Put the paddle in the same place as the beginning
void Game::ResetPaddle()
{
    glm::vec2 paddlePos = glm::vec2(Width / 2.0f - PADDLE_SIZE.x / 2.0f, Height - PADDLE_SIZE.y);
    glm::vec2 ballPos = paddlePos + glm::vec2(PADDLE_SIZE.x / 2.0f - BALL_RADIUS, -BALL_RADIUS * 2.0f);
    Paddle->SetPosition(paddlePos);
    Ball->SetPosition(ballPos);
    Ball->Stuck = true;
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

Collision checkCollision(BallObject &one, GameObject &two) // AABB -Circle
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
    if (glm::length(difference) < one.Radius)
        return std::make_tuple(true, VectorDirection(difference), difference);
    else
        return std::make_tuple(false, UP, glm::vec2(0.0f, 0.0f));
}

Direction VectorDirection(glm::vec2 target)
{
    glm::vec2 compass[] =
        {
            glm::vec2(0.0f, 1.0f),  // up
            glm::vec2(1.0f, 0.0f),  // rigth
            glm::vec2(0.0f, -1.0f), // down
            glm::vec2(-1.0f, 0.0f), // left
        };
    float max = 0.0f;
    unsigned int best_match = -1;
    for (unsigned int i = 0; i < 4; i++)
    {
        float dot_product = glm::dot(glm::normalize(target), compass[i]);
        if (dot_product > max)
        {
            max = dot_product;
            best_match = i;
        }
    }
    return (Direction)best_match;
}
