#ifndef RESSOURCES_H
#define RESSOURCES_H

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>
#include <stdbool.h>
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


/**
 * Structure d'un bouton avec son rectangle, son texte et son état de survol
 */
typedef struct {
    SDL_Rect rect;
    int hovered;
    const char* label;
    SDL_Color color_text;
    SDL_Color color_base;
    SDL_Color color_hover;
    TTF_Font * font_text;
    TTF_Font * font_text_hover;
    int taille_bonus_hover_x;
    int taille_bonus_hover_y;
    int is_survolable; // Booleen pour savoir si le bouton utilise ses param hover (= 1) ou non (= 0)
} Button;

typedef struct {
    SDL_Color bg;
    SDL_Color axes;
    SDL_Color texte_axes;
    SDL_Color bande_droite;
    SDL_Color bande_haute_champs;
    SDL_Color bande_haute_description;
    SDL_Color bg_champ_entree;
    SDL_Color bg_champ_entree_hover;
    SDL_Color texte_champ_entree;
    SDL_Color texte_descriptifs_bande_haut;

} Colors;


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
void renderButton(SDL_Renderer *renderer, Button *button);

/**
 * Affiche du texte sur le renderer autour de la position x, y
 * @param renderer Un pointeur sur une structure contenant l'état du rendu
 * @param text Le texte à afficher
 * @param x La position du centre en x du texte
 * @param y La position du centre en y du texte
 * @param color La couleur du texte
 * @param font La police du texte
 */
void renderText(SDL_Renderer *renderer, const char *text, int x, int y, SDL_Color color, TTF_Font *font);



// Fonctions définies dans menus.c mais utilisées dans plusieurs fichiers (pour éviter les inclusions circulaires)
/**
 * Début du lancement du jeu
 * @param ren Un pointeur sur une structure contenant l'état du rendu
 */
void ecran_acceuil (SDL_Renderer* ren);

#endif
