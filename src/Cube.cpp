#include "Cube.h"

Cube::Cube()
{
    setupCube();
}

Cube::~Cube()
{
    // Zwalnianie zasobów
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
}

void Cube::setupCube()
{
    float vertices[] = {
        // Wspó³rzêdne wierzcho³ków szeœcianu

        // Przednia œciana
        -0.5f, -0.5f,  0.5f, // Dolny lewy
         0.5f, -0.5f,  0.5f, // Dolny prawy
         0.5f,  0.5f,  0.5f, // Górny prawy
         0.5f,  0.5f,  0.5f, // Górny prawy
        -0.5f,  0.5f,  0.5f, // Górny lewy
        -0.5f, -0.5f,  0.5f, // Dolny lewy

        // Tylnia œciana
        -0.5f, -0.5f, -0.5f, // Dolny lewy
         0.5f, -0.5f, -0.5f, // Dolny prawy
         0.5f,  0.5f, -0.5f, // Górny prawy
         0.5f,  0.5f, -0.5f, // Górny prawy
        -0.5f,  0.5f, -0.5f, // Górny lewy
        -0.5f, -0.5f, -0.5f, // Dolny lewy

        // Lewa œciana
        -0.5f,  0.5f,  0.5f, // Górny przód
        -0.5f,  0.5f, -0.5f, // Górny ty³
        -0.5f, -0.5f, -0.5f, // Dolny ty³
        -0.5f, -0.5f, -0.5f, // Dolny ty³
        -0.5f, -0.5f,  0.5f, // Dolny przód
        -0.5f,  0.5f,  0.5f, // Górny przód

        // Prawa œciana
         0.5f,  0.5f,  0.5f, // Górny przód
         0.5f,  0.5f, -0.5f, // Górny ty³
         0.5f, -0.5f, -0.5f, // Dolny ty³
         0.5f, -0.5f, -0.5f, // Dolny ty³
         0.5f, -0.5f,  0.5f, // Dolny przód
         0.5f,  0.5f,  0.5f, // Górny przód

         // Dolna œciana
         -0.5f, -0.5f, -0.5f, // Ty³ lewy
          0.5f, -0.5f, -0.5f, // Ty³ prawy
          0.5f, -0.5f,  0.5f, // Przód prawy
          0.5f, -0.5f,  0.5f, // Przód prawy
         -0.5f, -0.5f,  0.5f, // Przód lewy
         -0.5f, -0.5f, -0.5f, // Ty³ lewy

         // Górna œciana
         -0.5f,  0.5f, -0.5f, // Ty³ lewy
          0.5f,  0.5f, -0.5f, // Ty³ prawy
          0.5f,  0.5f,  0.5f, // Przód prawy
          0.5f,  0.5f,  0.5f, // Przód prawy
         -0.5f,  0.5f,  0.5f, // Przód lewy
         -0.5f,  0.5f, -0.5f  // Ty³ lewy
    };

    // Generowanie i wi¹zanie VAO i VBO
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    // Wi¹zanie VAO
    glBindVertexArray(VAO);

    // Wi¹zanie i wype³nianie VBO
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Konfiguracja atrybutów wierzcho³ków
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Odwi¹zanie VAO i VBO
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void Cube::Draw()
{
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
}
