#ifndef RESSOURCES_H
#define RESSOURCES_H

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>
#include <stdbool.h>
#include "gestionGraphique.h"
#include <SDL2/SDL_image.h>
#include "SDL2_gfx/SDL2_gfxPrimitives.h"
#include <ctype.h>

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
    int radius; // Rayon des coins arrondis
} Button;

typedef struct {
    SDL_Rect rect;
    int hovered;
    SDL_Texture *image;
    SDL_Color color_base;
    SDL_Color color_hover;
    int taille_bonus_hover_x;
    int taille_bonus_hover_y;
    int is_survolable; // Booleen pour savoir si le bouton utilise ses param hover (= 1) ou non (= 0)
    int radius; // Rayon des coins arrondis
    int pourcentage_place; // Le pourcentage de place utilisé par l'image dans le bouton
} ImageButton;

typedef struct {
    SDL_Color bg;
    SDL_Color axes;
    SDL_Color texte_axes;
    SDL_Color bande_droite;
    SDL_Color bande_haute_expressions;
    SDL_Color bande_haute_description;
    SDL_Color bg_champ_entree;
    SDL_Color bg_champ_entree_hover;
    SDL_Color texte_champ_entree;
    SDL_Color texte_descriptifs_bande_haut;
    SDL_Color bg_bandes_expression_1;
    SDL_Color bg_bandes_expression_2;
    SDL_Color bande_bas_de_bande_haut;
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
 * Affiche un bouton avec du texte
 * @param renderer Un pointeur sur une structure contenant l'état du rendu
 * @param button Une instance de bouton
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

/**
 * Affiche un bouton avec une image
 * @param renderer Un pointeur sur une structure contenant l'état du rendu
 * @param button Une instance de bouton
 */
void renderImageButton(SDL_Renderer *renderer, ImageButton *button);

/**
 * Affiche une image de bouton dans le rectangle rect en prenant en compte le paramètre du bouton pourcentage_place
 * @param renderer Un pointeur sur une structure contenant l'état du rendu
 * @param button Une instance de bouton
 * @param rect Le rectangle dans lequel placer l'image
 */
void draw_image (SDL_Renderer *renderer, ImageButton *button, SDL_Rect rect);

/**
 * Charge l'image (avec le nom <filename> se trouvant dans le répèrtoire Ressources/) et la renvoie
 * @param renderer Un pointeur sur une structure contenant l'état du rendu
 * @param filename Le nom de l'image avec son extention ou le path pour accèder à l'image depuis Ressources/
 * @return La texture de l'image chargée
 */
SDL_Texture* load_image (SDL_Renderer *renderer, const char * filename);

/**
 * Test si la souris est dans le rectangle
 * @param rect Le rectangle
 * @param x_souris_px La position de la souris sur l'axe x en pixels
 * @param y_souris_px La position de la souris sur l'axe y en pixels
 */
int is_souris_sur_rectangle (SDL_Rect rect, int x_souris_px, int y_souris_px);

/**
 * Affiche un rectangle avec les bord bas arrondis
 * @param ren Un pointeur sur une structure contenant l'état du rendu
 * @param x1 Le point en haut à gauche du rectangle
 * @param y1 Le point en haut à gauche du rectangle
 * @param x2 Le point en bas à droite du rectangle
 * @param y2 Le point en bas à droite du rectangle
 * @param radius Le rayon de la courbure des coins bas
 * @param color La couleur du rectangle
 */
void affiche_bande_arrondis_en_bas (SDL_Renderer* renderer, int x1, int y1, int x2, int y2, int radius, SDL_Color color);

/**
 * Affiche un rectangle avec les bord bas arrondis
 * @param ren Un pointeur sur une structure contenant l'état du rendu
 * @param x1 Le point en haut à gauche du rectangle
 * @param y1 Le point en haut à gauche du rectangle
 * @param x2 Le point en bas à droite du rectangle
 * @param y2 Le point en bas à droite du rectangle
 * @param radius Le rayon de la courbure des coins bas
 * @param color La couleur du rectangle
 */
void affiche_bande_arrondis_en_haut (SDL_Renderer* renderer, int x1, int y1, int x2, int y2, int radius, SDL_Color color);

/**
 * Génère un nombre aléatoire compris entre les valeurs min et max (incluses).
 * @param min La valeur minimale possible (borne incluse).
 * @param max La valeur maximale possible (borne incluse).
 * @return Un entier aléatoire compris entre min et max.
 */
int nb_alea(int min, int max);


// Fonctions définies dans menus.c mais utilisées dans plusieurs fichiers (pour éviter les inclusions circulaires)
/**
 * Début du lancement du jeu
 * @param ren Un pointeur sur une structure contenant l'état du rendu
 */
void ecran_acceuil (SDL_Renderer* ren);

#include "color_picker.h"

#endif
