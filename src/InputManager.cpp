#include "InputManager.h"

InputManager::InputManager()
{
    // Konstruktor (jeœli potrzebujemy inicjalizacji)
}

InputManager::~InputManager()
{
    // Destruktor (jeœli potrzebujemy sprz¹tania)
}

void InputManager::ProcessInput(GLFWwindow* window, Snake& snake)
{
    // Obs³uga zamkniêcia okna
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    // Sterowanie wê¿em
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        snake.SetDirection(UP);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        snake.SetDirection(DOWN);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        snake.SetDirection(LEFT);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        snake.SetDirection(RIGHT);
}
