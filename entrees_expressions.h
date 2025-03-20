#ifndef ENTREES_EXPRESSIONS_H
#define ENTREES_EXPRESSIONS_H

#include "ressources.h"

#define TAILLE_BANDE_DROITE 100
#define TAILLE_BANDE_DESCRIPTIONS 40 
#define TAILLE_BANDE_HAUT (TAILLE_BANDE_DESCRIPTIONS + 90)
#define TAILLE_BANDE_EXPRESSIONS_MIN (TAILLE_BANDE_HAUT - TAILLE_BANDE_DESCRIPTIONS)
#define TAILLE_BANDE_EXPRESSIONS_MAX (TAILLE_BANDE_EXPRESSIONS_MIN + 200)
#define BANDE_EXPRESSIONS_ON_SCROLL_STEP 20
#define BANDE_EXPRESSIONS_OFF_SCROLL_STEP 10
#define MAX_LEN_STR 20
#define MAX_LEN_ENTREES_BORNES 8
#define NB_ENTREES 3
#define NB_ELEMENTS_PAR_EXPRESSION 7
#define CURSOR_BLINK_TIME 500
#define NB_EXPRESSION_MAX 5 // Le nombre de fonction maximum affichables en simultané
#define RAYON_BAS_BANDE_HAUT 20
#define TAILLE_BARRE_BASSE_DE_BANDE_HAUT ((RAYON_BAS_BANDE_HAUT < 10) ? 10 : RAYON_BAS_BANDE_HAUT)
#define WIDTH_CHAMPS_BORNES 150
#define WIDTH_CHAMP_EXPRESSION 300

//extern Message message;


typedef enum { // ! L'ordre est important pour l'initialisation des champs d'entrées. Il doit correspondre à celui de l'initialisation des champs d'entrées et la SELECTION_NULL doit être en dernier
    BORNE_INF,
    BORNE_SUP,
    EXPRESSION,
    SELECTION_NULL
} SelectionEntree;

typedef struct {
    Button button_base;
    int temps_affichage; // en secondes
    int is_visible;
    time_t start_time;
} Message;

typedef struct {
    char text[MAX_LEN_STR + 1]; // +1 pour le /0 comme fin de chaine
    char display[MAX_LEN_STR + 2]; // +2 pour le /0 et le curseur
    Button* champs_texte;
    SDL_Rect position_initiale; // Position de l'entrée sans offset. L'offset est appliqué directement dans le bouton <champs_texte>
    int cursorVisible;
    int position_cursor;
    Uint32 lastCursorToggle;
    SelectionEntree type_entree;
} Entree_texte;

typedef struct {
    char fonction_str [MAX_LEN_STR]; // L'expression en chaine de caractère de la fonction
    float (*f)(float);
    float borne_inf; // Le minimum de l'intervale de définition choisi
    float borne_sup; // Le maximum de l'intervale de définition choisi
    float fx_max; // Le maximum de la fonction sur l'interval choisi
    float fx_min; // Le minimum de la fonction sur l'interval choisi
    SDL_Color color;
} Fonction;

typedef struct {
    ImageButton bt;
    SDL_Rect rect_base;
} Button_mvt;

typedef struct {
    Fonction fonction;
    Entree_texte* borne_inf;
    Entree_texte* borne_sup;
    Entree_texte* expression;
    SelectionEntree entree_selectionnee;
    Entree_texte* champs_entrees[NB_ENTREES];
    SDL_Color bg_color;
    int numero; // Le numéro unique à chaque expression et représentant l'ordre dans lequel il est dans la liste déroulante
    SDL_Rect rect_initial; // L'espace pris par la bande d'expression sans offset
    SDL_Rect rect_affiche; // L'espace pris par la bande d'expression avec offset
    bool visible;
    Color_picker* color_picker;
    Button_mvt button_deplacement;
    Button_mvt button_visibilite;
    Button_mvt button_delete;
} Expression_fonction;

typedef struct {
    int width_elements[NB_ELEMENTS_PAR_EXPRESSION + 1]; // Largeur de tous les éléments affichés de gauche à droite sur la bande d'expression avec le minimum en x de la bande haute en 0 ième position (d'ou le '+1')
    //int width_entrees_bornes;
    //int width_entree_expression;
    int height_entrees_pourcentage; // La taille en y des entrées en pourcentage de la taille en y de la bande d'expression
    int height_texte_desctriptif;
    int espace_entre_elements;
    //int marge_entree_gauche;
    int height_bande_expression;
    int taille_color_picker;
    int taille_button_deplacement;
    int taille_button_visibilite;
    int taille_button_delete;
} Parametres_bandes_entrees;

typedef struct {
    Expression_fonction* expressions[NB_EXPRESSION_MAX];
    int nb_expressions; // La taille à l'instant t du tableau <expressions>
    Button* texte_descriptif_borne_inf;
    Button* texte_descriptif_borne_sup;
    Button* texte_descriptif_expression;
    bool expanding; // Pour savoir si la bande est fermée ou ouverte (false = fermé, true = ouverte)
    int scroll_offset;
    SDL_Rect surface; // Le recantgle occupé par la bande
    Parametres_bandes_entrees params;
} Bande_entrees;


float f (float x);
float g (float x);
float h (float x);
float k (float x);
float l (float x);

/**
 * Affiche la bande haute de l'interface
 * @param ren Un pointeur sur une structure contenant l'état du rendu
 * @param bande_entrees La bande d'entrées à afficher
 * @param colors Les couleurs de l'interface
 */
void affiche_bande_haut (SDL_Renderer* ren, Bande_entrees* bande_entrees, Colors* colors);

/**
 * Initialise la bande d'entrées
 * @param ren Un pointeur sur une structure contenant l'état du rendu
 * @param bande_entrees La bande d'entrées à initialiser
 * @param colors Les couleurs de l'interface
 */
void init_bande_entrees (SDL_Renderer* ren, Bande_entrees* bande_entrees, Colors* colors);

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
 * Initialise le button déplacement de la bande d'expression
 * @param ren Un pointeur sur une structure contenant l'état du rendu
 * @param expression La bande de l'expression, donc les entrées à modifier
 * @param params Les paramètres de taille et d'espacement pour le positionnement des textes
 * @param colors Les couleurs de l'interface
 */
void init_button_deplacement (SDL_Renderer* ren, Expression_fonction* expression, Parametres_bandes_entrees params, Colors* colors);

/**
 * Initialise le button qui affiche/cache la courbe
 * @param ren Un pointeur sur une structure contenant l'état du rendu
 * @param expression La bande de l'expression, donc les entrées à modifier
 * @param params Les paramètres de taille et d'espacement pour le positionnement des textes
 * @param colors Les couleurs de l'interface
 */
void init_button_visibilite (SDL_Renderer* ren, Expression_fonction* expression, Parametres_bandes_entrees params, Colors* colors);

/**
 * Initialise le button qui supprime l'expression
 * @param ren Un pointeur sur une structure contenant l'état du rendu
 * @param expression La bande de l'expression, donc les entrées à modifier
 * @param params Les paramètres de taille et d'espacement pour le positionnement des textes
 * @param colors Les couleurs de l'interface
 */
void init_button_delete (SDL_Renderer* ren, Expression_fonction* expression, Parametres_bandes_entrees params, Colors* colors);

/**
 * Initialise les 3 champs d'entrées de la bande d'expression
 * @param ren Un pointeur sur une structure contenant l'état du rendu
 * @param expression La bande de l'expression, donc les entrées à modifier
 * @param params Les paramètres de taille et d'espacement pour le positionnement des textes
 * @param colors Les couleurs de l'interface
 */
void init_champs_entrees (SDL_Renderer* ren, Expression_fonction* expression, Parametres_bandes_entrees params, Colors* colors);

/**
 * Initialise les titres de la bande descriptive
 * @param bande_entrees La bande d'entrées dans laquelle ce trouve la bande descriptive
 * @param params Les paramètres de taille et d'espacement pour le positionnement des textes
 * @param colors Les couleurs de l'interface
 */
void init_placement_bande_descriptive (Bande_entrees* bande_entrees, Parametres_bandes_entrees params, Colors* colors);

/**
 * Initialise les positions des éléments de la bande d'expression donnée
 * @param ren Un pointeur sur une structure contenant l'état du rendu
 * @param expression La bande de l'expression, donc les entrées à modifier
 * @param params Les paramètres de taille et d'espacement pour le positionnement des textes
 * @param surface_bande_haut La surface totale de la bande haute
 * @param colors Les couleurs de l'interface
 */
void init_placement_entrees (SDL_Renderer* ren, Expression_fonction* expression, Parametres_bandes_entrees params, SDL_Rect surface_bande_haut, Colors* colors);

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

/**
 * Regarde s'il faut executer une action en quittant le focus d'un certain champs avant d'en focus un autre (ou pas)
 * @param expression La bande de l'expression
 * @param nouvelle_entree Le prochain champs focus (au aucun si SELECTION_NULL)
 */
void execute_champs_select_and_change_focus (Expression_fonction* expression, SelectionEntree nouvelle_entree);

/**
 * Met à jour la position affiché des éléments de l'expression avec l'offset actuel
 * @param expression La bande de l'expression
 * @param offset L'offset actuel
 */
void placement_pour_affichage_avec_offset (Expression_fonction* expression, int offset);

/**
 * Redimmentionne la bande haute en fonction de la taille de la fenêtre et des autres éléments de la fenêtre à l'instant t
 * @param bande_entrees La bande d'entrées dans laquelle ce trouve les bande descriptive et expressions
 */
void resize_bande_haut (Bande_entrees* bande_entrees);

/**
 * Calcule la première coordonnée x du <num_element>ème élément (de gauche à droite) de la bande d'expression
 * @param tab Le tableau avec la première position de la bande en x et les width de chaque élément dans l'ordre
 * @param espaces L'actuelle taille des espaces inter-éléments
 * @param num_element Le numéro de l'élément voulu
 * @return La position en x en pixel de la première coordonnée x du <num_element>ème élément
 */
int calcul_pos (int tab[NB_ELEMENTS_PAR_EXPRESSION + 1], int espaces, int num_element);

/**
 * Redimmentionne la bande haute en fonction de la taille de la fenêtre et des autres éléments de la fenêtre à l'instant t
 * @param text_erreur message d'erreur affiché
 * @param endroit_erreur rectangle où l'erreur a été enregestré
 */
void set_message (const char* text_erreur, SDL_Rect endroit_erreur);



#endif
