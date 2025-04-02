#ifndef GESTION_3D_2_H
#define GESTION_3D_2_H

#include <SDL2/SDL.h>
#include <math.h>
#include <stdbool.h>

#define WIDTH 800
#define HEIGHT 600
#define GRID_SIZE 20
#define RENDER_DISTANCE 5.0f

// Structure pour représenter un point 3D
typedef struct {
    float x, y, z;
} Point3D_;

typedef struct {
    Point3D_ position;
    Point3D_ rotation;
    Point3D_ renderCenter; // Center of our rendering area
} Camera;


#endif
