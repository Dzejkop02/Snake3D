#ifndef GAME_H
#define GAME_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Shader.h"
#include "Cube.h"
#include "Snake.h"
#include "Food.h"
#include "InputManager.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <map>

enum GameState
{
    GAME_ACTIVE,
    GAME_OVER
};

class Game
{
public:
    Game(unsigned int width, unsigned int height);
    ~Game();

    void Run();

private:
    // Wymiary okna
    unsigned int Width, Height;

    // Okno GLFW
    GLFWwindow* Window;

    // Obiekty gry
    Shader* shader;
    Cube* cube;
    Snake* snake;
    Food* food;
    InputManager* inputManager;

    // Zmienne czasu
    float deltaTime;
    float lastFrame;

    // Stan gry
    GameState State;

    // Rotacja szeœcianu
    glm::quat currentRotation;
    glm::quat targetRotation;
    float rotationSpeed;

    // Mapowanie œcian na rotacje
    std::map<Face, glm::quat> faceRotations;

    // Prywatne metody
    void Init();
    void Update();
    void Render();

    // Funkcje callback
    static void FramebufferSizeCallback(GLFWwindow* window, int width, int height);
};

#endif // GAME_H
