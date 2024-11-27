#include "Cube.h"

Cube::Cube()
{
    setupCube();
}

Cube::~Cube()
{
    // Zwalnianie zasob�w
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
}

void Cube::setupCube()
{
    float vertices[] = {
        // Wsp�rz�dne wierzcho�k�w sze�cianu

        // Przednia �ciana
        -0.5f, -0.5f,  0.5f, // Dolny lewy
         0.5f, -0.5f,  0.5f, // Dolny prawy
         0.5f,  0.5f,  0.5f, // G�rny prawy
         0.5f,  0.5f,  0.5f, // G�rny prawy
        -0.5f,  0.5f,  0.5f, // G�rny lewy
        -0.5f, -0.5f,  0.5f, // Dolny lewy

        // Tylnia �ciana
        -0.5f, -0.5f, -0.5f, // Dolny lewy
         0.5f, -0.5f, -0.5f, // Dolny prawy
         0.5f,  0.5f, -0.5f, // G�rny prawy
         0.5f,  0.5f, -0.5f, // G�rny prawy
        -0.5f,  0.5f, -0.5f, // G�rny lewy
        -0.5f, -0.5f, -0.5f, // Dolny lewy

        // Lewa �ciana
        -0.5f,  0.5f,  0.5f, // G�rny prz�d
        -0.5f,  0.5f, -0.5f, // G�rny ty�
        -0.5f, -0.5f, -0.5f, // Dolny ty�
        -0.5f, -0.5f, -0.5f, // Dolny ty�
        -0.5f, -0.5f,  0.5f, // Dolny prz�d
        -0.5f,  0.5f,  0.5f, // G�rny prz�d

        // Prawa �ciana
         0.5f,  0.5f,  0.5f, // G�rny prz�d
         0.5f,  0.5f, -0.5f, // G�rny ty�
         0.5f, -0.5f, -0.5f, // Dolny ty�
         0.5f, -0.5f, -0.5f, // Dolny ty�
         0.5f, -0.5f,  0.5f, // Dolny prz�d
         0.5f,  0.5f,  0.5f, // G�rny prz�d

         // Dolna �ciana
         -0.5f, -0.5f, -0.5f, // Ty� lewy
          0.5f, -0.5f, -0.5f, // Ty� prawy
          0.5f, -0.5f,  0.5f, // Prz�d prawy
          0.5f, -0.5f,  0.5f, // Prz�d prawy
         -0.5f, -0.5f,  0.5f, // Prz�d lewy
         -0.5f, -0.5f, -0.5f, // Ty� lewy

         // G�rna �ciana
         -0.5f,  0.5f, -0.5f, // Ty� lewy
          0.5f,  0.5f, -0.5f, // Ty� prawy
          0.5f,  0.5f,  0.5f, // Prz�d prawy
          0.5f,  0.5f,  0.5f, // Prz�d prawy
         -0.5f,  0.5f,  0.5f, // Prz�d lewy
         -0.5f,  0.5f, -0.5f  // Ty� lewy
    };

    // Generowanie i wi�zanie VAO i VBO
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    // Wi�zanie VAO
    glBindVertexArray(VAO);

    // Wi�zanie i wype�nianie VBO
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Konfiguracja atrybut�w wierzcho�k�w
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Odwi�zanie VAO i VBO
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void Cube::Draw()
{
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
}
