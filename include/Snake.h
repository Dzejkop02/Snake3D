#ifndef SNAKE_H
#define SNAKE_H

#include <vector>
#include <glm/glm.hpp>
#include "Cube.h"
#include "Shader.h"

// Enums
enum Direction
{
    UP,
    DOWN,
    LEFT,
    RIGHT
};

enum Face
{
    FRONT,      // 0
    RIGHT_FACE, // 1
    BACK,       // 2
    LEFT_FACE,  // 3
    TOP,        // 4
    BOTTOM      // 5
};

// Deklaracja wstêpna klasy Food
class Food;

// Definicja struktury Segment poza klas¹ Snake
struct Segment
{
    Face face;
    int x;
    int y;
};

class Snake
{
public:
    Snake();
    ~Snake();

    void Update(float deltaTime);
    void Draw(Shader& shader, const glm::mat4& cubeModel);

    void SetDirection(Direction dir);

    bool IsAlive() const;

    void Grow();
    bool CheckFoodCollision(const Food& food) const;
    const std::vector<Segment>& GetSegments() const;

    glm::ivec3 GetSegmentPosition(const Segment& segment) const;

    Face GetCurrentFace() const; // Dodana metoda

private:
    std::vector<Segment> segments;
    Cube* cube;

    Direction currentDirection;
    float moveTimer;
    float moveInterval; // Czas miêdzy ruchami wê¿a

    bool isAlive;
    bool shouldGrow;

    void Move();
    void HandleEdgeTransition(Segment& head);
    void CheckSelfCollision();
    void InitializeTransitions();

    // Struktura Transition i tablica transitions
    struct Transition
    {
        Face newFace;
        Direction newDirection;
        bool swapXY;
        bool invertX;
        bool invertY;

        Transition()
            : newFace(FRONT), newDirection(UP), swapXY(false), invertX(false), invertY(false)
        {
        }

        Transition(Face face, Direction dir, bool swap, bool invX, bool invY)
            : newFace(face), newDirection(dir), swapXY(swap), invertX(invX), invertY(invY)
        {
        }
    };

    Transition transitions[6][4];
};

#endif // SNAKE_H
