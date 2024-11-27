#include "Snake.h"
#include "Food.h"
#include <glm/gtc/type_ptr.hpp>

Snake::Snake()
{
    // Inicjalizacja wê¿a z jednym segmentem (g³ow¹) na œrodku przedniej œciany
    Segment head;
    head.face = FRONT;
    head.x = 5;
    head.y = 5;
    segments.push_back(head);

    cube = new Cube();

    currentDirection = UP;
    moveTimer = 0.0f;
    moveInterval = 0.5f; // W¹¿ porusza siê co 0.5 sekundy

    isAlive = true;
    shouldGrow = false;

    InitializeTransitions();
}

Snake::~Snake()
{
    delete cube;
}

bool Snake::IsAlive() const
{
    return isAlive;
}

void Snake::Grow()
{
    shouldGrow = true;
}

bool Snake::CheckFoodCollision(const Food& food) const
{
    const Segment& head = segments.front();
    glm::ivec3 headPos = GetSegmentPosition(head);

    glm::ivec3 foodPos = food.GetGridPosition();

    return head.face == food.GetFace() && headPos.x == foodPos.x && headPos.y == foodPos.y;
}

void Snake::SetDirection(Direction dir)
{
    // Zapobiegamy zawracaniu wê¿a na przeciwny kierunek
    if ((currentDirection == UP && dir != DOWN) ||
        (currentDirection == DOWN && dir != UP) ||
        (currentDirection == LEFT && dir != RIGHT) ||
        (currentDirection == RIGHT && dir != LEFT))
    {
        currentDirection = dir;
    }
}

void Snake::Update(float deltaTime)
{
    if (!isAlive)
        return;

    moveTimer += deltaTime;
    if (moveTimer >= moveInterval)
    {
        Move();
        moveTimer = 0.0f;
    }
}

void Snake::Move()
{
    // Tworzymy now¹ g³owê na podstawie obecnej pozycji i kierunku
    Segment newHead = segments.front();

    // Aktualizujemy pozycjê g³owy w zale¿noœci od kierunku
    switch (currentDirection)
    {
    case UP:
        newHead.y += 1;
        break;
    case DOWN:
        newHead.y -= 1;
        break;
    case LEFT:
        newHead.x -= 1;
        break;
    case RIGHT:
        newHead.x += 1;
        break;
    }

    // Sprawdzamy przejœcia miêdzy œcianami
    HandleEdgeTransition(newHead);

    // Dodajemy now¹ g³owê do wê¿a
    segments.insert(segments.begin(), newHead);

    // Usuwamy ostatni segment (ogon) wê¿a tylko wtedy, gdy nie ma rosn¹æ
    if (!shouldGrow)
    {
        segments.pop_back();
    }
    else
    {
        shouldGrow = false;
    }

    // Sprawdzamy kolizjê z samym sob¹
    CheckSelfCollision();
}

void Snake::CheckSelfCollision()
{
    // Sprawdzamy, czy g³owa koliduje z którymkolwiek segmentem cia³a
    if (segments.size() < 2)
        return;

    const Segment& head = segments.front();

    for (size_t i = 1; i < segments.size(); ++i)
    {
        const Segment& segment = segments[i];
        if (head.face == segment.face && head.x == segment.x && head.y == segment.y)
        {
            isAlive = false;
            break;
        }
    }
}

const std::vector<Segment>& Snake::GetSegments() const
{
    return segments;
}

glm::ivec3 Snake::GetSegmentPosition(const Segment& segment) const
{
    return glm::ivec3(segment.x, segment.y, static_cast<int>(segment.face));
}

Face Snake::GetCurrentFace() const
{
    return segments.front().face;
}

void Snake::InitializeTransitions()
{
    // Pomocnicze flagi
    const bool NO_SWAP = false;
    const bool SWAP = true;
    const bool NO_INVERT = false;
    const bool INVERT = true;

    // Inicjalizacja wszystkich przejœæ na domyœlne wartoœci
    for (int face = 0; face < 6; ++face)
    {
        for (int dir = 0; dir < 4; ++dir)
        {
            transitions[face][dir] = Transition(static_cast<Face>(face), static_cast<Direction>(dir), NO_SWAP, NO_INVERT, NO_INVERT);
        }
    }

    // Definiowanie przejœæ

    // FRONT face transitions
    transitions[FRONT][UP] = Transition(TOP, UP, NO_SWAP, NO_INVERT, NO_INVERT);
    transitions[FRONT][DOWN] = Transition(BOTTOM, DOWN, NO_SWAP, NO_INVERT, NO_INVERT);
    transitions[FRONT][LEFT] = Transition(LEFT_FACE, RIGHT, NO_SWAP, INVERT, NO_INVERT);
    transitions[FRONT][RIGHT] = Transition(RIGHT_FACE, LEFT, NO_SWAP, INVERT, NO_INVERT);

    // RIGHT_FACE face transitions
    transitions[RIGHT_FACE][UP] = Transition(TOP, UP, NO_SWAP, NO_INVERT, NO_INVERT);
    transitions[RIGHT_FACE][DOWN] = Transition(BOTTOM, DOWN, NO_SWAP, NO_INVERT, NO_INVERT);
    transitions[RIGHT_FACE][LEFT] = Transition(FRONT, RIGHT, NO_SWAP, INVERT, NO_INVERT);
    transitions[RIGHT_FACE][RIGHT] = Transition(BACK, LEFT, NO_SWAP, INVERT, NO_INVERT);

    // BACK face transitions
    transitions[BACK][UP] = Transition(TOP, UP, NO_SWAP, NO_INVERT, NO_INVERT);
    transitions[BACK][DOWN] = Transition(BOTTOM, DOWN, NO_SWAP, NO_INVERT, NO_INVERT);
    transitions[BACK][LEFT] = Transition(RIGHT_FACE, RIGHT, NO_SWAP, INVERT, NO_INVERT);
    transitions[BACK][RIGHT] = Transition(LEFT_FACE, LEFT, NO_SWAP, INVERT, NO_INVERT);

    // LEFT_FACE face transitions
    transitions[LEFT_FACE][UP] = Transition(TOP, UP, NO_SWAP, NO_INVERT, NO_INVERT);
    transitions[LEFT_FACE][DOWN] = Transition(BOTTOM, DOWN, NO_SWAP, NO_INVERT, NO_INVERT);
    transitions[LEFT_FACE][LEFT] = Transition(BACK, RIGHT, NO_SWAP, INVERT, NO_INVERT);
    transitions[LEFT_FACE][RIGHT] = Transition(FRONT, LEFT, NO_SWAP, INVERT, NO_INVERT);

    // TOP face transitions
    transitions[TOP][UP] = Transition(BACK, UP, NO_SWAP, NO_INVERT, NO_INVERT);
    transitions[TOP][DOWN] = Transition(FRONT, DOWN, NO_SWAP, NO_INVERT, NO_INVERT);
    transitions[TOP][LEFT] = Transition(LEFT_FACE, UP, SWAP, NO_INVERT, INVERT);
    transitions[TOP][RIGHT] = Transition(RIGHT_FACE, UP, SWAP, NO_INVERT, NO_INVERT);

    // BOTTOM face transitions
    transitions[BOTTOM][UP] = Transition(FRONT, UP, NO_SWAP, NO_INVERT, NO_INVERT);
    transitions[BOTTOM][DOWN] = Transition(BACK, DOWN, NO_SWAP, NO_INVERT, NO_INVERT);
    transitions[BOTTOM][LEFT] = Transition(LEFT_FACE, DOWN, SWAP, NO_INVERT, NO_INVERT);
    transitions[BOTTOM][RIGHT] = Transition(RIGHT_FACE, DOWN, SWAP, NO_INVERT, INVERT);
}


void Snake::HandleEdgeTransition(Segment& head)
{
    const int gridSize = 10;

    Direction dir = currentDirection;
    bool crossedEdge = false;

    if (head.x < 0)
    {
        dir = LEFT;
        crossedEdge = true;
    }
    else if (head.x >= gridSize)
    {
        dir = RIGHT;
        crossedEdge = true;
    }
    else if (head.y < 0)
    {
        dir = DOWN;
        crossedEdge = true;
    }
    else if (head.y >= gridSize)
    {
        dir = UP;
        crossedEdge = true;
    }

    if (crossedEdge)
    {
        Transition t = transitions[head.face][dir];

        // Ustawiamy now¹ œcianê
        head.face = t.newFace;

        // Ustawiamy nowy kierunek
        currentDirection = t.newDirection;

        // Mapowanie pozycji
        int tempX = head.x;
        int tempY = head.y;

        if (head.x < 0)
            tempX = gridSize - 1;
        else if (head.x >= gridSize)
            tempX = 0;

        if (head.y < 0)
            tempY = gridSize - 1;
        else if (head.y >= gridSize)
            tempY = 0;

        if (t.swapXY)
        {
            std::swap(tempX, tempY);
        }

        if (t.invertX)
        {
            tempX = gridSize - 1 - tempX;
        }

        if (t.invertY)
        {
            tempY = gridSize - 1 - tempY;
        }

        head.x = tempX;
        head.y = tempY;
    }
}


void Snake::Draw(Shader& shader, const glm::mat4& cubeModel)
{
    const int gridSize = 10;
    const float cubeSize = 1.0f; // Rozmiar g³ównego szeœcianu
    const float offset = cubeSize / gridSize;
    const float halfCube = cubeSize / 2.0f;

    for (auto& segment : segments)
    {
        glm::mat4 model = cubeModel; // Rozpoczynamy od macierzy szeœcianu

        // Przeskalowujemy pozycjê do zakresu (-0.5, 0.5)
        float posX = -halfCube + offset / 2 + segment.x * offset;
        float posY = -halfCube + offset / 2 + segment.y * offset;
        float posZ = halfCube; // Domyœlnie przednia œciana

        // Ustawiamy pozycjê w zale¿noœci od œciany
        switch (segment.face)
        {
        case FRONT:
            model = glm::translate(model, glm::vec3(posX, posY, halfCube));
            break;
        case BACK:
            model = glm::translate(model, glm::vec3(-posX, posY, -halfCube));
            model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0, 1, 0));
            break;
        case LEFT_FACE:
            model = glm::translate(model, glm::vec3(-halfCube, posY, -posX));
            model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0, 1, 0));
            break;
        case RIGHT_FACE:
            model = glm::translate(model, glm::vec3(halfCube, posY, posX));
            model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0, 1, 0));
            break;
        case TOP:
            model = glm::translate(model, glm::vec3(posX, halfCube, -posY));
            model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1, 0, 0));
            break;
        case BOTTOM:
            model = glm::translate(model, glm::vec3(posX, -halfCube, posY));
            model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1, 0, 0));
            break;
        }

        model = glm::scale(model, glm::vec3(0.1f)); // Skalowanie segmentu

        shader.setMat4("model", glm::value_ptr(model));
        cube->Draw();
    }
}

