#ifndef ENTREES_EXPRESSIONS_H
#define ENTREES_EXPRESSIONS_H

#include "ressources.h"

#define TAILLE_BANDE_HAUT 120
#define TAILLE_BANDE_DESCRIPTIONS 40 // Inclus dans la bande haute
#define MAX_LEN_STR 20
#define MAX_LEN_ENTREES_BORNES 4
#define NB_ENTREES 3
#define CURSOR_BLINK_TIME 500


typedef enum { // ! L'ordre est important pour l'initialisation des champs d'entrées. Il doit correspondre à celui de l'initialisation des champs d'entrées et la SELECTION_NULL doit être en dernier
    BORNE_INF,
    BORNE_SUP,
    EXPRESSION,
    SELECTION_NULL
} SelectionEntree;

typedef struct {
    char text[MAX_LEN_STR + 1]; // +1 pour le /0 comme fin de chaine
    char display[MAX_LEN_STR + 2]; // +2 pour le /0 et le curseur
    Button* champs_texte;
    int cursorVisible;
    int position_cursor;
    Uint32 lastCursorToggle;
    SelectionEntree type_entree;
} Entree_texte;

typedef struct {
    Entree_texte* borne_inf;
    Entree_texte* borne_sup;
    Entree_texte* expression;
    SelectionEntree entree_selectionnee;
    Button* texte_descriptif_borne_inf;
    Button* texte_descriptif_borne_sup;
    Button* texte_descriptif_expression;
    Entree_texte* champs_entrees[NB_ENTREES];
    int x_souris_px;
    int y_souris_px;
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
 * Test si la souris est sur le bouton
 * @param button Le bouton
 * @param x_souris_px La position de la souris sur l'axe x en pixels
 * @param y_souris_px La position de la souris sur l'axe y en pixels
 */
int is_souris_sur_button (Button button, int x_souris_px, int y_souris_px);

/**
 * Insère un caractère dans une chaine de caractères
 * @param text La chaine de caractères
 * @param i La position où insérer le caractère
 * @param c Le caractère à insérer
 */
void insert_char(char text[], int i, char c);

/**
 * Retire un caractère dans une chaine de caractères
 * @param text La chaine de caractères
 * @param i La position du caractère à retirer
 */
void remove_char(char text[], int i);

/**
 * Initialise les positions des champs d'entrées (et des textes descrictifs des entrées) dans la bande haute
 * @param bande_entrees La bande d'entrées à initialiser
 * @param entrees Un tableau contenant toutes les entrées
 * @param colors Les couleurs de l'interface
 */
void init_placement_entrees (Bande_entrees* bande_entrees, Entree_texte* entrees[NB_ENTREES], Colors* colors);

/**
 * Gère les événements des entrées et des expressions
 * @param event L'événement à gérer
 * @param bande_entrees La bande d'entrées à modifier
 * @return 0 si l'événement <SDLK_BACKSPACE> a été géré, 1 sinon
 */
int handle_events_entrees_experssions(SDL_Event event, Bande_entrees* bande_entrees);

#endif
