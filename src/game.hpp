
#ifndef GAME_H
#define GAME_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <vector>

#include "game_level.hpp"

// State machine for the different states of the game
enum GameState
{
    GAME_ACTIVE,
    GAME_MENU,
    GAME_WIN
};
// Axis direction to calculate the ball direction
enum Direction
{
    UP,
    RIGHT,
    DOWN,
    LEFT
};
// Information after collision
typedef std::tuple<bool, Direction, glm::vec2> Collision;

// Host class that will controls the game
class Game
{
public:
    GameState State;
    bool Keys[1024];
    unsigned int Width, Height;
    std::vector<GameLevel> Levels;
    unsigned int Current_level;

    // Game functionalities
    void DoCollisions();
    void ResetLevel();
    void ResetPaddle();
    // Constructor/Destructor
    Game(unsigned int width, unsigned int height);
    ~Game();
    // Flow functionalities
    void Init();                  // Initialize game state (load all resources: Shaders/textures/levels)
    void ProcessInput(double dt); // Process input as stored within keys parameter
    void Update(double dt);       // Update all game events: player/ball movements
    void Render();                // Render the game Frame
};
#endif