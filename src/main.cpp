#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "game.h"

#include <iostream>
#include <string>
#include <memory>

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
const std::string config = "config.txt";
Game game(config,"shaders/shader.vs", "shaders/shader.fs");

int main()
{

    glfwSetKeyCallback(game.m_window, key_callback);

    game.run();

    return 0;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    // when a user presses the escape key, we set the WindowShouldClose property to true, closing the application
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    if (key >= 0 && key < 1024)
    {
        if (action == GLFW_PRESS)
            game.m_player->cInput->Keys[key] = true;
           // game.Keys[key] = true;
        else if (action == GLFW_RELEASE)
            game.m_player->cInput->Keys[key] = false;
           // game.Keys[key] = false;
    }
}
