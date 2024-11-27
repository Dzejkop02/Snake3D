#ifndef INPUTMANAGER_H
#define INPUTMANAGER_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Snake.h"

class InputManager
{
public:
    InputManager();
    ~InputManager();

    void ProcessInput(GLFWwindow* window, Snake& snake);

private:
    // Mo¿emy dodaæ tutaj dodatkowe metody lub zmienne w przysz³oœci
};

#endif // INPUTMANAGER_H
