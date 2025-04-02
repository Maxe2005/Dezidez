#ifndef GESTION_3D_2_H
#define GESTION_3D_2_H

#include "ressources_3D.h"

#define GRID_SIZE 20
#define RENDER_DISTANCE 5.0f

typedef struct {
    Point3D position;
    Point3D rotation;
    Point3D renderCenter; // Center of our rendering area
} Camera;


#endif
