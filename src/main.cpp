#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "game.hpp"
#include "resource_manager.hpp"

#include <iostream>

// GLFW function declarations
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

// The Width AND Heigth of the screen
const unsigned int SCREEN_WIDTH = 800;
const unsigned int SCREEN_HEIGHT = 600;

// Creation of the Window
Game Breakout(SCREEN_WIDTH, SCREEN_HEIGHT);

int main( int agrc, char*argv[])
{
    glfwInit(); // create context container and set the configurations
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
    glfwWindowHint(GLFW_RESIZABLE, false);

    GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Breakout", nullptr, nullptr);
    glfwMakeContextCurrent(window);

    // glad: load all OpenGL function pointers
    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glfwSetKeyCallback(window, key_callback);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    //OpenGL configuration
    glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    //initialize game
    Breakout.Init();

    //deltaTime variables ( because of the object in movement, we need to take care about the time)
    float deltaTime = 0.0f;
    float lastFrame = 0.0f;

    while(!glfwWindowShouldClose(window))
    {
        // Calculate deltaTime
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        glfwPollEvents(); // take the inputs of keyboard and mouse

        // Manage user input
        Breakout.ProcessInput(deltaTime);

        // Update game state
        Breakout.Update(deltaTime);

        // Render
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        Breakout.Render();

        glfwSwapBuffers(window);
    }

    // delete all resources as loaded using the resource manager
    ResourceManager::Clear();

    glfwTerminate();
    return 0;
}

// Function definitions
// key_callback is a function an interrupt when one action with the keys take place. press, release etc... (less CPU intense)
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    // When a user presses the esacpe key, we set the WindowShouldClose property to true, closing the application
    if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    if(key >= 0 && key < 1024)
    {
        if(action == GLFW_PRESS)
            Breakout._keys[key] = true;
        else if(action == GLFW_RELEASE)
            Breakout._keys[key] = false;
    }
}

// callback that is called when the window size change. This implementation reegenerate the viewMatrix to project the renderings into the window size.
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}