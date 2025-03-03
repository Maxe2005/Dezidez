#ifndef GRAPHIQUE_H
#define GRAPHIQUE_H

#include "ressources.h"

typedef struct {
    Button button_base;
    int temps_affichage; // en secondes
    SDL_Color couleur_message;
    SDL_Color couleur_fond;
    int is_visible;
    time_t start_time;
} Message;



/**
 * Fonction mathématique f(x) pour du test
 * @param x La valeur de x
 * @return La valeur de f(x)
 */
float f (float x);

/**
 * Fonction principale du grapheur
 * @param ren Un pointeur sur une structure contenant l'état du rendu
 */
void Grapheur (SDL_Renderer* ren);

#endif
