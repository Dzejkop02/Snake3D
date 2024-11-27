// src/Game.cpp

#include "Game.h"
#include <iostream>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>

Game::Game(unsigned int width, unsigned int height)
    : Width(width), Height(height), Window(nullptr), shader(nullptr), cube(nullptr), snake(nullptr), food(nullptr), inputManager(nullptr), State(GAME_ACTIVE)
{
    Init();
}

Game::~Game()
{
    // Zwalnianie zasobów
    delete shader;
    delete cube;
    delete snake;
    delete food;
    delete inputManager;
    glfwDestroyWindow(Window);
    glfwTerminate();
}

void Game::Init()
{
    // Inicjalizacja GLFW
    if (!glfwInit())
    {
        std::cerr << "Nie uda³o siê zainicjalizowaæ GLFW" << std::endl;
        exit(EXIT_FAILURE);
    }

    // Ustawienia GLFW
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Tworzenie okna
    Window = glfwCreateWindow(Width, Height, "Snake3D", nullptr, nullptr);
    if (Window == nullptr)
    {
        std::cerr << "Nie uda³o siê utworzyæ okna GLFW" << std::endl;
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
    glfwMakeContextCurrent(Window);

    // Rejestracja callbacka
    glfwSetFramebufferSizeCallback(Window, FramebufferSizeCallback);

    // Inicjalizacja GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cerr << "Nie uda³o siê zainicjalizowaæ GLAD" << std::endl;
        exit(EXIT_FAILURE);
    }

    // Ustawienie viewportu
    glViewport(0, 0, Width, Height);

    // W³¹czenie testu g³êbi
    glEnable(GL_DEPTH_TEST);

    // Inicjalizacja shaderów i obiektów gry
    shader = new Shader("shaders/vertex_shader.glsl", "shaders/fragment_shader.glsl");
    cube = new Cube();
    snake = new Snake();
    food = new Food();
    inputManager = new InputManager();

    // Inicjalizacja rotacji szeœcianu
    currentRotation = glm::quat(); // Brak rotacji
    targetRotation = glm::quat();
    rotationSpeed = 2.0f; // Prêdkoœæ interpolacji rotacji

    // Mapowanie œcian na rotacje
    faceRotations[FRONT] = glm::quat(); // Brak rotacji

    faceRotations[BACK] = glm::angleAxis(glm::radians(180.0f), glm::vec3(0, 1, 0));
    faceRotations[LEFT_FACE] = glm::angleAxis(glm::radians(90.0f), glm::vec3(0, 1, 0));
    faceRotations[RIGHT_FACE] = glm::angleAxis(glm::radians(-90.0f), glm::vec3(0, 1, 0));
    faceRotations[TOP] = glm::angleAxis(glm::radians(-90.0f), glm::vec3(1, 0, 0));
    faceRotations[BOTTOM] = glm::angleAxis(glm::radians(90.0f), glm::vec3(1, 0, 0));
}

void Game::Run()
{
    // Inicjalizacja czasu
    lastFrame = 0.0f;

    // G³ówna pêtla gry
    while (!glfwWindowShouldClose(Window))
    {
        // Obliczanie deltaTime
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // Obs³uga wejœcia
        inputManager->ProcessInput(Window, *snake);

        // Aktualizacja stanu gry
        Update();

        // Renderowanie
        Render();

        // Wymiana buforów i obs³uga zdarzeñ
        glfwSwapBuffers(Window);
        glfwPollEvents();
    }
}

void Game::Update()
{
    if (State == GAME_ACTIVE)
    {
        // Aktualizacja logiki wê¿a
        snake->Update(deltaTime);

        // Sprawdzenie, czy w¹¿ ¿yje
        if (!snake->IsAlive())
        {
            State = GAME_OVER;
        }

        // Sprawdzenie kolizji wê¿a z jedzeniem
        if (snake->CheckFoodCollision(*food))
        {
            // W¹¿ zjad³ jedzenie
            snake->Grow();
            food->Generate(snake->GetSegments());
        }

        // Aktualizacja docelowej rotacji na podstawie pozycji wê¿a
        Face snakeFace = snake->GetCurrentFace();
        targetRotation = faceRotations[snakeFace];
    }
    else if (State == GAME_OVER)
    {
        // Mo¿liwoœæ restartu gry
        if (glfwGetKey(Window, GLFW_KEY_ENTER) == GLFW_PRESS)
        {
            // Restart gry
            delete snake;
            delete food;
            snake = new Snake();
            food = new Food();
            State = GAME_ACTIVE;
            currentRotation = glm::quat();
            targetRotation = glm::quat();
        }
    }

    // Interpolacja rotacji
    currentRotation = glm::slerp(currentRotation, targetRotation, rotationSpeed * deltaTime);
}

void Game::Render()
{
    // Czyszczenie buforów koloru i g³êbi
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Aktywacja shaderów
    shader->use();

    // Tworzenie macierzy widoku i projekcji
    glm::mat4 view = glm::mat4(1.0f);
    glm::mat4 projection = glm::mat4(1.0f);

    // Przesuniêcie kamery
    view = glm::translate(view, glm::vec3(0.0f, 0.0f, -5.0f));

    // Ustawienie projekcji perspektywicznej
    projection = glm::perspective(glm::radians(45.0f), (float)Width / (float)Height, 0.1f, 100.0f);

    // Przekazanie macierzy widoku i projekcji do shaderów
    shader->setMat4("view", glm::value_ptr(view));
    shader->setMat4("projection", glm::value_ptr(projection));

    // Renderowanie g³ównego szeœcianu
    glm::mat4 cubeModel = glm::mat4(1.0f);

    // Zastosowanie rotacji
    cubeModel = cubeModel * glm::mat4_cast(currentRotation);

    // Ustawienie macierzy modelu dla szeœcianu
    shader->setMat4("model", glm::value_ptr(cubeModel));
    shader->setVec4("objectColor", glm::vec4(1.0f)); // Bia³y kolor
    cube->Draw();

    // Renderowanie wê¿a i jedzenia tylko, jeœli gra jest aktywna
    if (State == GAME_ACTIVE)
    {
        // Renderowanie wê¿a
        shader->setVec4("objectColor", glm::vec4(1.0f, 0.0f, 0.0f, 1.0f)); // Czerwony kolor
        snake->Draw(*shader, cubeModel);

        // Renderowanie jedzenia
        shader->setVec4("objectColor", glm::vec4(0.0f, 1.0f, 0.0f, 1.0f)); // Zielony kolor
        food->Draw(*shader, cubeModel);
    }
    else if (State == GAME_OVER)
    {
        // Mo¿emy wyœwietliæ komunikat "Game Over"
        // W tym przyk³adzie nie mamy implementacji tekstu, ale mo¿na to dodaæ w przysz³oœci
    }

    // Reset koloru
    shader->setVec4("objectColor", glm::vec4(1.0f)); // Bia³y kolor
}

void Game::FramebufferSizeCallback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}
