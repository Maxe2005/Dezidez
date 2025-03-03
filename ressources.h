#ifndef RESSOURCES_H
#define RESSOURCES_H

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>
#include "gestionGraphique.h"

#define FEN_X 1000
#define FEN_Y 800
#define TAILLE_BANDEAU_HAUT 80
#define MARGE_BANDEAU_HAUT 10
#define PI 3.14159265358979323846
#define HEADER_HEIGHT 100
#define BUTTON_WIDTH 200
#define BUTTON_HEIGHT 50
#define BUTTON_MARGIN 30
#define SCROLL_SPEED 30  // Vitesse du scroll
#define SCROLL_OFFSET_MIN -100

#define NB_FONTS 4
extern TTF_Font* fonts[NB_FONTS];


/**
 * Structure d'un bouton avec son rectangle, son texte et son état de survol
 */
typedef struct {
    SDL_Rect rect;
    int hovered;
    const char* label;
} Button;


/**
 * Initialise les différentes polices de caractère avec les tailles correspondantes
 * @param font Le tableau de police vide à remplire
 */
void init_font (TTF_Font* font[NB_FONTS]);

/**
 * Affiche un en-tête en début de page
 * @param renderer Un pointeur sur une structure contenant l'état du rendu
 * @param titre Le texte affiché dans le header 
 */
void renderHeader(SDL_Renderer *renderer, char *titre);

/**
 * Affiche un bouton
 * @param renderer Un pointeur sur une structure contenant l'état du rendu
 * @param button Une instance de bouton
 * @param color_text La couleur du texte du bouton
 * @param colorh0 La couleur de base du font du bouton
 * @param colorh1 La couleur quand la souris est sur le bouton
 */
void renderButton(SDL_Renderer *renderer, Button *button, SDL_Color color_text, SDL_Color colorh0, SDL_Color colorh1);


#endif
