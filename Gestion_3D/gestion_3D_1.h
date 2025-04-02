#ifndef GESTION_3D_1_H
#define GESTION_3D_1_H

#include <SDL2/SDL.h>
#include <math.h>

// Dimensions de la fenêtre
#define WIDTH 800
#define HEIGHT 600

// Structure pour représenter un point 3D
typedef struct {
    float x, y, z;
} Point3D;

// Structure pour représenter un point 2D
typedef struct {
    int x, y;
} Point2D;

// Structure pour représenter un quaternion
typedef struct {
    float w, x, y, z;
} Quaternion;

#endif
