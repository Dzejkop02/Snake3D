#ifndef CUBE_H
#define CUBE_H

#include <glad/glad.h>

class Cube
{
public:
    Cube();
    ~Cube();
    void Draw();

private:
    unsigned int VAO, VBO;
    void setupCube();
};

#endif // CUBE_H
