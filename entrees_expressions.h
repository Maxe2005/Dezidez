#ifndef ENTREES_EXPRESSIONS_H
#define ENTREES_EXPRESSIONS_H

#include "ressources.h"

#define TAILLE_BANDE_HAUT 120
#define TAILLE_BANDE_DESCRIPTIONS 40 // Inclus dans la bande haute
#define MAX_LEN_STR 20
#define MAX_LEN_ENTREES_BORNES 8
#define NB_ENTREES 3
#define CURSOR_BLINK_TIME 500
#define NB_EXPRESSION_MAX 5 // Le nombre de fonction maximum affichables en simultané


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
    char fonction_str [MAX_LEN_STR]; // L'expression en chaine de caractère de la fonction
    float borne_inf; // Le minimum de l'intervale de définition choisi
    float borne_sup; // Le maximum de l'intervale de définition choisi
    float fx_max; // Le maximum de la fonction sur l'interval choisi
    float fx_min; // Le minimum de la fonction sur l'interval choisi
    SDL_Color color;
} Fonction;

typedef struct {
    Fonction fonction;
    Entree_texte* borne_inf;
    Entree_texte* borne_sup;
    Entree_texte* expression;
    SelectionEntree entree_selectionnee;
    Entree_texte* champs_entrees[NB_ENTREES];
} Expression_fonction;

typedef struct {
    Expression_fonction* expressions[NB_EXPRESSION_MAX];
    Button* texte_descriptif_borne_inf;
    Button* texte_descriptif_borne_sup;
    Button* texte_descriptif_expression;
    int x_souris_px;
    int y_souris_px;
} Bande_entrees;

typedef struct {
    int width_entrees_bornes;
    int width_entree_expression;
    int height_entrees;
    int height_texte_desctriptif;
    int espace_entre_entrees;
    int marge_entree_gauche;
} Parametres_bandes_entrees;

/**
 * Affiche la bande haute de l'interface
 * @param ren Un pointeur sur une structure contenant l'état du rendu
 * @param bande_entrees La bande d'entrées à afficher
 * @param colors Les couleurs de l'interface
 */
void affiche_bande_haut (SDL_Renderer* ren, Bande_entrees* bande_entrees, Expression_fonction* expression, Colors* colors);

/**
 * Initialise la bande d'entrées
 * @param bande_entrees La bande d'entrées à initialiser
 * @param colors Les couleurs de l'interface
 */
void init_bande_entrees (Bande_entrees* bande_entrees, Colors* colors);

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
 * Initialise les titres de la bande descriptive
 * @param bande_entrees La bande d'entrées dans laquelle ce trouve la bande descriptive
 * @param params Les paramètres de taille et d'espacement pour le positionnement des textes
 * @param colors Les couleurs de l'interface
 */
void init_placement_bande_descriptive (Bande_entrees* bande_entrees, Parametres_bandes_entrees params, Colors* colors);

/**
 * Initialise les positions des champs d'entrées (et des textes descrictifs des entrées) dans la bande haute
 * @param expression La bande de l'expression, donc les entrées à modifier
 * @param params Les paramètres de taille et d'espacement pour le positionnement des textes
 * @param colors Les couleurs de l'interface
 */
void init_placement_entrees (Expression_fonction* expression, Parametres_bandes_entrees params, Colors* colors);

/**
 * Gère les événements des entrées et des expressions
 * @param event L'événement à gérer
 * @param expression La bande de l'expression, donc les entrées à modifier
 * @param x_souris_px La position en pixel de la souris
 * @param y_souris_px La position en pixel de la souris
 * @return 0 si l'événement <SDLK_BACKSPACE> a été géré, 1 sinon
 */
int handle_events_entrees_experssions(SDL_Event event, Expression_fonction* expression, int x_souris_px, int y_souris_px);

/**
 * Charge la valeur présente dans le champs de saisie dans la valeur de la borne inférieur de la fonction
 * et gère des erreurs d'entrée s'il y en a
 * @param expression La bande de l'expression
 */
void charge_valeur_borne_inf (Expression_fonction* expression);

/**
 * Charge la valeur présente dans le champs de saisie dans la valeur de la borne supérieur de la fonction
 * et gère des erreurs d'entrée s'il y en a
 * @param expression La bande de l'expression
 */
void charge_valeur_borne_sup (Expression_fonction* expression);

#endif
