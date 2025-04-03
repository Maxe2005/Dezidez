#ifndef GESTION_3D_1_H
#define GESTION_3D_1_H

#include "ressources_3D.h"


// Structure pour représenter un point 2D
typedef struct {
    int x, y;
} Point2D;

// Structure pour représenter un quaternion
typedef struct {
    float w, x, y, z;
} Quaternion;

typedef struct {
    Quaternion rotation;
    float zoom;
    int origine_y_apres_bande_haut;
} Graph_3D_1;

/**
 * Fonction pour normaliser un quaternion
 * @param q Le quaternion à normaliser
 */
void normalizeQuaternion(Quaternion* q);

/**
 * Fonction pour multiplier deux quaternions
 * @param q1 Le premier quaternion
 * @param q2 Le deuxième quaternion
 */
Quaternion multiplyQuaternions(Quaternion q1, Quaternion q2);

/**
 * Fonction pour créer un quaternion à partir d'un axe et d'un angle
 * @param x L'axe x
 * @param y L'axe y
 * @param z L'axe z
 * @param angle L'angle en radians
 * @return Le quaternion résultant
 */
Quaternion quaternionFromAxisAngle(float x, float y, float z, float angle);

/**
 * Fonction de projection 3D à 2D
 * @param p Le point 3D à projeter
 * @param graph Le graphique 3D
 * @return Le point projeté en 2D
 */
Point2D project(Point3D p, Graph_3D_1* graph);

/**
 * Trace les axes avec des graduations
 * @param renderer Un pointeur sur une structure contenant l'état du rendu
 * @param start Le point de départ de l'axe
 * @param end Le point d'arrivée de l'axe
 * @param steps Le nombre de graduations
 * @param graph Le graphique 3D
 */
void drawAxis(SDL_Renderer* renderer, Point3D start, Point3D end, int steps, Graph_3D_1* graph);

/**
 * Gère les événements pour le graphique 3D
 * @param e L'événement à gérer
 * @param graph Le graphique 3D
 * @param x_souris_px La position en pixel de la souris
 * @param y_souris_px La position en pixel de la souris
 */
void handle_event_3D_1 (SDL_Event e, Graph_3D_1* graph, int x_souris_px, int y_souris_px);

/**
 * Affiche le graphique 3D
 * @param renderer Un pointeur sur une structure contenant l'état du rendu
 * @param graph Le graphique 3D à afficher
 * @param bande_haute La bande haute avec toutes les expressions des fonctions
 */
void renderGraph3D_1(SDL_Renderer* renderer, Graph_3D_1* graph, Bande_haute* bande_haute);

#endif
