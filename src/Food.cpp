#include "Food.h"
#include <cstdlib>
#include <ctime>
#include <glm/gtc/type_ptr.hpp>

Food::Food()
{
    cube = new Cube();
    std::srand(static_cast<unsigned int>(std::time(nullptr)));
    GenerateRandomPosition({});
}

Food::~Food()
{
    delete cube;
}

void Food::Generate(const std::vector<Segment>& snakeSegments)
{
    GenerateRandomPosition(snakeSegments);
}

void Food::GenerateRandomPosition(const std::vector<Segment>& snakeSegments)
{
    const int gridSize = 10;
    bool positionIsValid = false;

    while (!positionIsValid)
    {
        // Losujemy �cian�
        int face = std::rand() % 6;

        // Losujemy pozycj� na siatce
        int x = std::rand() % gridSize;
        int y = std::rand() % gridSize;

        // Sprawdzamy, czy pozycja nie jest zaj�ta przez w�a
        positionIsValid = true;
        for (const auto& segment : snakeSegments)
        {
            if (segment.face == static_cast<Face>(face) && segment.x == x && segment.y == y)
            {
                positionIsValid = false;
                break;
            }
        }

        if (positionIsValid)
        {
            gridPosition = glm::ivec3(x, y, face);

            const float cubeSize = 1.0f;
            const float offset = cubeSize / gridSize;
            const float halfCube = cubeSize / 2.0f;

            float posX = -halfCube + offset / 2 + x * offset;
            float posY = -halfCube + offset / 2 + y * offset;

            // Ustawiamy pozycj� w zale�no�ci od �ciany
            switch (face)
            {
            case FRONT: // Przednia �ciana (z = 0.5)
                position = glm::vec3(posX, posY, halfCube);
                break;
            case BACK: // Tylna �ciana (z = -0.5)
                position = glm::vec3(posX, posY, -halfCube);
                break;
            case LEFT_FACE: // Lewa �ciana (x = -0.5)
                position = glm::vec3(-halfCube, posY, -posX);
                break;
            case RIGHT_FACE: // Prawa �ciana (x = 0.5)
                position = glm::vec3(halfCube, posY, posX);
                break;
            case BOTTOM: // Dolna �ciana (y = -0.5)
                position = glm::vec3(posX, -halfCube, posY);
                break;
            case TOP: // G�rna �ciana (y = 0.5)
                position = glm::vec3(posX, halfCube, -posY);
                break;
            default:
                position = glm::vec3(0.0f);
                break;
            }
        }
    }
}

void Food::Draw(Shader& shader, const glm::mat4& cubeModel)
{
    glm::mat4 model = cubeModel; // Rozpoczynamy od macierzy sze�cianu
    model = glm::translate(model, position);
    model = glm::scale(model, glm::vec3(0.1f)); // Skalowanie jedzenia

    shader.setMat4("model", glm::value_ptr(model));
    cube->Draw();
}

glm::vec3 Food::GetPosition() const
{
    return position;
}

glm::ivec3 Food::GetGridPosition() const
{
    return gridPosition;
}

Face Food::GetFace() const
{
    return static_cast<Face>(gridPosition.z);
}
