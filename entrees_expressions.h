#ifndef ENTREES_EXPRESSIONS_H
#define ENTREES_EXPRESSIONS_H

#include "ressources.h"

#define TAILLE_BANDE_HAUT 120
#define TAILLE_BANDE_DESCRIPTIONS 40 // Inclus dans la bande haute
#define MAX_LEN_STR 100
#define MAX_LEN_ENTREES_BORNES 4
#define NB_ENTREES 3
#define CURSOR_BLINK_TIME 500

typedef struct {
    char text[MAX_LEN_STR + 1]; // +1 pour le /0 comme fin de chaine
    char display[MAX_LEN_STR + 2]; // +2 pour le /0 et le curseur
    Button* champs_texte;
    int cursorVisible;
    Uint32 lastCursorToggle;
} Entree_texte;

typedef enum {
    BORNE_SUP,
    BORNE_INF,
    EXPRESSION,
    SELECTION_NULL
} SelectionEntree;

typedef struct {
    Entree_texte borne_inf;
    Entree_texte borne_sup;
    Entree_texte expression;
    SelectionEntree entree_selectionnee;
    Button* texte_descriptif_borne_inf;
    Button* texte_descriptif_borne_sup;
    Button* texte_descriptif_expression;
    Button* champs_entrees[NB_ENTREES];
} Bande_entrees;

/**
 * Affiche la bande haute de l'interface
 * @param ren Un pointeur sur une structure contenant l'état du rendu
 * @param bande_entrees La bande d'entrées à afficher
 * @param colors Les couleurs de l'interface
 */
void affiche_bande_haut (SDL_Renderer* ren, Bande_entrees* bande_entrees, Colors* colors);

/**
 * Initialise la bande d'entrées
 * @param bande_entrees La bande d'entrées à initialiser
 * @param colors Les couleurs de l'interface
 */
void init_bande_entrees (Bande_entrees* bande_entrees, Colors* colors);

/**
 * Initialise les positions des champs d'entrées (et des textes descrictifs des entrées) dans la bande haute
 * @param bande_entrees La bande d'entrées à initialiser
 * @param entrees Un tableau contenant toutes les entrées
 * @param colors Les couleurs de l'interface
 */
void init_placement_entrees (Bande_entrees* bande_entrees, Button* entrees[NB_ENTREES], Colors* colors);

#endif
