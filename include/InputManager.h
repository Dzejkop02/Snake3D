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
    // Mo�emy doda� tutaj dodatkowe metody lub zmienne w przysz�o�ci
};

#endif // INPUTMANAGER_H
