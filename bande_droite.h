#ifndef BANDE_DROITE_H
#define BANDE_DROITE_H

#include "ressources.h"

#define TAILLE_BANDE_DROITE 150

typedef struct {
    ImageButton bouton_retour;
    Button bouton_centrer;
} Bande_droite;

/**
 * Affiche la bande droite de l'interface
 * @param ren Un pointeur sur une structure contenant l'état du rendu
 * @param bande_droite La bande droite à afficher
 */
void affiche_bande_droite (SDL_Renderer* ren, Bande_droite* bande_droite);

/**
 * Initialise la bande droite
 * @param ren Un pointeur sur une structure contenant l'état du rendu
 * @param bande_droite La bande droite à initialiser
 */
void init_bande_droite (SDL_Renderer* ren, Bande_droite* bande_droite);

/**
 * Initialise le boutton retour maison
 * @param ren Un pointeur sur une structure contenant l'état du rendu
 * @param bande_droite La bande droite à initialiser
 */
void init_boutton_home (SDL_Renderer* ren, Bande_droite* bande_droite);

/**
 * Redimentionne la bande droite avec les dimmention actuelle de la fenêtre
 * @param bande_droite La bande droite à initialiser
 */
void resize_bande_droite (Bande_droite* bande_droite);

/**
 * Initialise le boutton pour centrer la courbe
 * @param ren Un pointeur sur une structure contenant l'état du rendu
 * @param bande_droite La bande droite à initialiser
 */
void init_boutton_centrer (SDL_Renderer* ren, Bande_droite* bande_droite);

#endif
