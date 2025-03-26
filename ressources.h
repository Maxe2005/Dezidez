#ifndef RESSOURCES_H
#define RESSOURCES_H

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>
#include <stdbool.h>
#include <ctype.h>
#include "gestionGraphique.h"

#define HEADER_HEIGHT 100
#define BUTTON_WIDTH 200
#define BUTTON_HEIGHT 50
#define BUTTON_MARGIN 30
#define SCROLL_SPEED 30  // Vitesse du scroll
#define SCROLL_OFFSET_MIN -100

#define NB_FONTS 6
extern TTF_Font* fonts[NB_FONTS];

extern int FEN_X;
extern int FEN_Y;


typedef struct {
    SDL_Color bg;
    SDL_Color axes;
    SDL_Color texte_axes;
    SDL_Color bande_droite;
    SDL_Color bande_haute_expressions;
    SDL_Color bande_haute_description;
    SDL_Color texte_champ_entree;
    SDL_Color texte_descriptifs_bande_haut;
    SDL_Color bg_bandes_expression_1;
    SDL_Color bg_bandes_expression_2;
    SDL_Color bande_bas_de_bande_haut;
    SDL_Color button_new_expression;
    SDL_Color button_new_expression_hover;
} Colors;
extern Colors* colors;

/**
 * Initialise les différentes polices de caractère avec les tailles correspondantes
 * @param font Le tableau de police vide à remplire
 */
void init_font (TTF_Font* font[NB_FONTS]);

/**
 * Génère un nombre aléatoire compris entre les valeurs min et max (incluses).
 * @param min La valeur minimale possible (borne incluse).
 * @param max La valeur maximale possible (borne incluse).
 * @return Un entier aléatoire compris entre min et max.
 */
int nb_alea(int min, int max);


#include "color_picker.h"

#endif
