#ifndef PARTICLE_GENERATOR_HPP
#define PARTICLE_GENERATOR_HPP

#include <vector>

#include <glad/glad.h>
#include <glm/glm.hpp>

#include "shader.hpp"
#include "texture.hpp"
#include "game_object.hpp"

// Store particle information
struct Particle
{
    glm::vec2 Position, Velocity;
    glm::vec4 Color;
    float Life;

    Particle() : Position(0.0f), Velocity(0.0f), Color(1.0f), Life(1.0f) {}
};

// ParticleGenerator acts as a container for rendering temporal particles
class ParticleGenerator
{
public:
    // Constructor
    ParticleGenerator(Shader shader, Texture2D texture, unsigned int nr_particles);
    // Change the state of the particles
    void Update(float dt, GameObject &object, unsigned int newParticles,
                glm::vec2 offset = glm::vec2(0.0f, 0.0f));
    // Render all particles
    void Draw();

private:
    // State
    std::vector<Particle> particles;
    unsigned int nr_particles;
    // Render state
    Shader shader;
    Texture2D texture;
    unsigned int VAO;
    // Initializes buffer and vertex attributes
    void init();
    // Returns the first Particle index that's currently unused e.g.
    // Line <= 0.0f or 0 if no particle is currently inactive
    unsigned int firstUnusedParticle();
    // Repawns particle
    void respawnParticle(Particle &particle, GameObject &object,
                         glm::vec2 offset = glm::vec2(0.0f, 0.0f));
};

#endif /* PARTICLE_GENERATOR_HPP */