#pragma once

unsigned int indicesArray[] = {
    // Face avant
    0, 1, 2,
    2, 3, 0,

    // Face arrière
    4, 5, 6,
    6, 7, 4,

    // Face gauche
    0, 3, 7,
    7, 4, 0,

    // Face droite
    1, 5, 6,
    6, 2, 1,

    // Face supérieure
    0, 4, 5,
    5, 1, 0,

    // Face inférieure
    2, 6, 7,
    7, 3, 2
};

float verticesArray[] = {
    // Sommets du cube
    -0.5f, -0.5f, -0.5f,  // Sommet 0
     0.5f, -0.5f, -0.5f,  // Sommet 1
     0.5f,  0.5f, -0.5f,  // Sommet 2
    -0.5f,  0.5f, -0.5f,  // Sommet 3
    -0.5f, -0.5f,  0.5f,  // Sommet 4
     0.5f, -0.5f,  0.5f,  // Sommet 5
     0.5f,  0.5f,  0.5f,  // Sommet 6
    -0.5f,  0.5f,  0.5f,  // Sommet 7
};
