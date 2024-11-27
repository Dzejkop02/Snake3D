#ifndef FOOD_H
#define FOOD_H

#include <glm/glm.hpp>
#include "Cube.h"
#include "Shader.h"
#include <vector>
#include "Snake.h" // Upewniamy siê, ¿e Snake.h jest do³¹czony

class Food
{
public:
    Food();
    ~Food();

    void Draw(Shader& shader, const glm::mat4& cubeModel);

    void Generate(const std::vector<Segment>& snakeSegments);

    glm::vec3 GetPosition() const;
    glm::ivec3 GetGridPosition() const;
    Face GetFace() const;

private:
    glm::vec3 position;
    glm::ivec3 gridPosition; // x, y, face
    Cube* cube;

    void GenerateRandomPosition(const std::vector<Segment>& snakeSegments);
};

#endif // FOOD_H
