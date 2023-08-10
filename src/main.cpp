#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "game.h"

#include <iostream>
#include <string>
#include <memory>


int main()
{
    const std::string config = "config.txt";
    Game game(config);
    game.run();
    return 0;
}


