#ifndef GRAPHIQUE_3D_H
#define GRAPHIQUE_3D_H

#include "ressources.h"
#include "bande_haute.h"
#include "events.h"

#define GRID_SIZE 20
#define RENDER_DISTANCE 5.0f

typedef struct {
    float x, y, z;
} Point3D;

typedef struct {
    Point3D position;
    Point3D rotation;
    Point3D renderCenter; // Center of our rendering area
} Camera;



typedef struct {
    
} Graph_3D;

typedef struct {
    Graph_3D* graph;
    Bande_haute* bande_haute;
    Bande_droite* bande_droite;
} Grapheur_3D_elements;

/**
 * Initialise l'interface du grapheur
 * @param ren Un pointeur sur une structure contenant l'état du rendu
 * @param gr_ele Les éléments du grapheur 3D
 */
void init_totale_interface_grapheur_3D (SDL_Renderer* ren, Grapheur_3D_elements *gr_ele);

/**
 * Affiche l'interface pour le graph 3D
 * @param ren Un pointeur sur une structure contenant l'état du rendu
 * @param bande_haute La bande d'entrées à afficher
 * @param bande_droite La bande droite affichée
 */
void affiche_interface_graph_3D (SDL_Renderer* ren, Bande_haute* bande_haute, Bande_droite* bande_droite);

/**
 * Fonction principale du grapheur 3D
 * @param ren Un pointeur sur une structure contenant l'état du rendu
 * @param gr_ele Les éléments du grapheur 3D
 */
int Grapheur_3D (SDL_Renderer* ren, Grapheur_3D_elements *gr_ele);

#endif
