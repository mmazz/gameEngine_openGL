#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "game.h"

#include <iostream>
#include <string>
#include <memory>


void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void mouseButtonCallback( GLFWwindow *window, int button, int action, int mods );

static void cursorPositionCallback( GLFWwindow *window, double xpos, double ypos );

const std::string config = "config.txt";
Game game(config,"shaders/shader.vs", "shaders/shader.fs");

int main()
{

    glfwSetKeyCallback(game.getWindow(), key_callback);
    glfwSetCursorPosCallback( game.getWindow(), cursorPositionCallback );
    glfwSetMouseButtonCallback(game.getWindow(), mouseButtonCallback );
    glfwSetInputMode(game.getWindow(), GLFW_STICKY_MOUSE_BUTTONS, 1 );
    game.run();

    return 0;
}



void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    // when a user presses the escape key, we set the WindowShouldClose property to true, closing the application
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (key == GLFW_KEY_ENTER&& action == GLFW_PRESS)
        game.setPaused();
    if (key >= 0 && key < 1024)
    {
        if (action == GLFW_PRESS)
            game.getPlayer()->cInput->Keys[key] = true;
           // game.Keys[key] = true;
        else if (action == GLFW_RELEASE)
            game.getPlayer()->cInput->Keys[key] = false;
           // game.Keys[key] = false;
    }
}
static void cursorPositionCallback( GLFWwindow *window, double xpos, double ypos )
{
   // std::cout << xpos << " : " << ypos << std::endl;
}
void mouseButtonCallback( GLFWwindow *window, int key, int action, int mods )
{
    if ( key == GLFW_MOUSE_BUTTON_RIGHT)
    {
        if (action == GLFW_PRESS)
            game.getPlayer()->cInput->Keys[key] = true;
        else if (action == GLFW_RELEASE)
            game.getPlayer()->cInput->Keys[key] = false;
    }
    if ( key == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS )
    {
        if (action == GLFW_PRESS)
            game.getPlayer()->cInput->Keys[key] = true;
        else if (action == GLFW_RELEASE)
            game.getPlayer()->cInput->Keys[key] = false;
    }
}
