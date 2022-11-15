#include "Game.hpp"

// Constructor/Destructor
Game::Game(unsigned int width, unsigned int height)
    : _state(GAME_ACTIVE), _keys(), _width(width), _height(height)
{

}

Game::~Game()
{

}

// Functionalities
// Initialize game state (load all resources: Shaders/textures/levels)
void Game::Init()
{
    
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
    
}