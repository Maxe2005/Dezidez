#ifndef ENTREES_EXPRESSIONS_H
#define ENTREES_EXPRESSIONS_H

#include "ressources.h"
#include "bande_droite.h"

#define TAILLE_BANDE_EXPRESSIONS_MIN (TAILLE_BANDE_HAUT - TAILLE_BANDE_DESCRIPTIONS)
#define TAILLE_BANDE_EXPRESSIONS_MAX (TAILLE_BANDE_EXPRESSIONS_MIN + 200)
#define BANDE_EXPRESSIONS_ON_SCROLL_STEP 20
#define BANDE_EXPRESSIONS_OFF_SCROLL_STEP 10
#define MAX_LEN_STR 20
#define MAX_LEN_ENTREES_BORNES 8
#define NB_ENTREES 3
#define NB_ELEMENTS_PAR_EXPRESSION 7
#define CURSOR_BLINK_TIME 500
#define NB_EXPRESSIONS_MAX 20 // Le nombre de fonction maximum affichables en simultané
#define RAYON_BAS_BANDE_HAUT 20
#define TAILLE_BARRE_BASSE_DE_BANDE_HAUT ((RAYON_BAS_BANDE_HAUT < 10) ? 10 : RAYON_BAS_BANDE_HAUT)
#define WIDTH_CHAMPS_BORNES 150
#define WIDTH_CHAMP_EXPRESSION 300

extern Message message;
extern Probleme probleme;

typedef enum { // ! L'ordre est important pour l'initialisation des champs d'entrées. Il doit correspondre à celui de l'initialisation des champs d'entrées et la SELECTION_NULL doit être en dernier
    BORNE_INF,
    BORNE_SUP,
    EXPRESSION,
    SELECTION_NULL
} SelectionEntree;

typedef struct {
    char text[MAX_LEN_STR + 1]; // +1 pour le /0 comme fin de chaine
    char text_backup[MAX_LEN_STR + 1]; // La dernière chaine exécutée fonctionnelle
    char display[MAX_LEN_STR + 2]; // +2 pour le /0 et le curseur
    Button* champs_texte;
    SDL_Rect position_initiale; // Position de l'entrée sans offset. L'offset est appliqué directement dans le bouton <champs_texte>
    int cursorVisible;
    int position_cursor;
    Uint32 lastCursorToggle;
    SelectionEntree type_entree;
} Entree_texte;

typedef struct {
    Node * fonction_arbre; // L'arbre de la fonction
    float borne_inf; // Le minimum de l'intervale de définition choisi
    float borne_sup; // Le maximum de l'intervale de définition choisi
    float fx_max; // Le maximum de la fonction sur l'interval choisi
    float fx_min; // Le minimum de la fonction sur l'interval choisi
    bool visible; // Pour savoir si la courbe est affichée ou non sur le graph
    SDL_Color color;
    bool is_erreur;
} Fonction;

// Un bouton en mouvement
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
    SDL_Color bg_color_oppo; // La couleur opposée à celle de bg_color
    int numero; // Le numéro unique à chaque expression et représentant l'ordre dans lequel il est dans la liste déroulante
    SDL_Rect rect_initial; // L'espace pris par la bande d'expression sans offset
    SDL_Rect rect_affiche; // L'espace pris par la bande d'expression avec offset
    bool visible;
    Color_picker* color_picker;
    Button_mvt button_deplacement;
    Button_mvt button_visibilite;
    Button_mvt button_delete;
    SDL_Texture* image_button_visible;
    SDL_Texture* image_button_invisible;
    bool is_moving; // Pour savoir si la bande d'expression est en train d'être déplacée
    int moving_offset; // Pour savoir de combien la bande d'expression a été déplacée
} Expression_fonction;

typedef struct {
    int width_elements[NB_ELEMENTS_PAR_EXPRESSION + 1]; // Largeur de tous les éléments affichés de gauche à droite sur la bande d'expression avec le minimum en x de la bande haute en 0 ième position (d'ou le '+1')
    int height_entrees_pourcentage; // La taille en y des entrées en pourcentage de la taille en y de la bande d'expression
    int height_texte_desctriptif;
    float espace_entre_elements;
    int height_bande_expression;
    int taille_color_picker;
    int taille_button_deplacement;
    int taille_button_visibilite;
    int taille_button_delete;
    int taille_button_new_expression;
} Parametres_bandes_entrees;

typedef struct {
    Expression_fonction* expressions[NB_EXPRESSIONS_MAX];
    int nb_expressions; // La taille à l'instant t du tableau <expressions>
    Button* texte_descriptif_borne_inf;
    Button* texte_descriptif_borne_sup;
    Button* texte_descriptif_expression;
    bool expanding; // Pour savoir si la bande est fermée ou ouverte (false = fermé, true = ouverte)
    int scroll_offset;
    SDL_Rect surface; // Le recantgle occupé par la bande
    Parametres_bandes_entrees params;
    Button_mvt button_new_expression; // Le bouton pour ajouter une nouvelle expression à tracer sur le graphique
} Bande_haute;


typedef struct {
    const char* **interval;
    Node* *arbres;
    const char* *nom_f;
    int nb_exemples;
} Exemples;


/**
 * Affiche la bande haute de l'interface
 * @param ren Un pointeur sur une structure contenant l'état du rendu
 * @param bande_haute La bande d'entrées à afficher
 */
void affiche_bande_haut (SDL_Renderer* ren, Bande_haute* bande_haute);

/**
 * Affiche la bande d'expression
 * @param ren Un pointeur sur une structure contenant l'état du rendu
 * @param expression La bande à afficher
 */
void affiche_bande_expression (SDL_Renderer* ren, Expression_fonction* expression);

/**
 * Initialise la bande d'entrées
 * @param ren Un pointeur sur une structure contenant l'état du rendu
 * @param bande_haute La bande d'entrées à initialiser
 */
void init_bande_haute (SDL_Renderer* ren, Bande_haute* bande_haute);

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
 */
void init_button_deplacement (SDL_Renderer* ren, Expression_fonction* expression, Parametres_bandes_entrees params);

/**
 * Initialise le button qui affiche/cache la courbe
 * @param ren Un pointeur sur une structure contenant l'état du rendu
 * @param expression La bande de l'expression, donc les entrées à modifier
 * @param params Les paramètres de taille et d'espacement pour le positionnement des textes
 */
void init_button_visibilite (SDL_Renderer* ren, Expression_fonction* expression, Parametres_bandes_entrees params);

/**
 * Initialise le button qui supprime l'expression
 * @param ren Un pointeur sur une structure contenant l'état du rendu
 * @param expression La bande de l'expression, donc les entrées à modifier
 * @param params Les paramètres de taille et d'espacement pour le positionnement des textes
 */
void init_button_delete (SDL_Renderer* ren, Expression_fonction* expression, Parametres_bandes_entrees params);

/**
 * Initialise le button pour ajouter une nouvelle expression
 * @param ren Un pointeur sur une structure contenant l'état du rendu
 * @param bande_haute La bande d'entrées dans laquelle ce trouve la bande d'expression
 */
void init_button_new_expression (SDL_Renderer* ren, Bande_haute* bande_haute);

/**
 * Initialise les 3 champs d'entrées de la bande d'expression
 * @param ren Un pointeur sur une structure contenant l'état du rendu
 * @param expression La bande de l'expression, donc les entrées à modifier
 * @param params Les paramètres de taille et d'espacement pour le positionnement des textes
 */
void init_champs_entrees (SDL_Renderer* ren, Expression_fonction* expression, Parametres_bandes_entrees params);

/**
 * Initialise les titres de la bande descriptive
 * @param bande_haute La bande d'entrées dans laquelle ce trouve la bande descriptive
 * @param params Les paramètres de taille et d'espacement pour le positionnement des textes
 */
void init_placement_bande_descriptive (Bande_haute* bande_haute, Parametres_bandes_entrees params);

/**
 * Initialise les positions des éléments de la bande d'expression donnée
 * @param ren Un pointeur sur une structure contenant l'état du rendu
 * @param expression La bande de l'expression, donc les entrées à modifier
 * @param params Les paramètres de taille et d'espacement pour le positionnement des textes
 * @param surface_bande_haut La surface totale de la bande haute
 */
void init_placement_entrees (SDL_Renderer* ren, Expression_fonction* expression, Parametres_bandes_entrees params, SDL_Rect surface_bande_haut);

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
 * Transforme l'expression str en fonction et l'assigne à la fonction de l'expression
 * @param expression La bande de l'expression
 */
void execute_expression (Expression_fonction* expression);

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
 * @param bande_haute La bande d'entrées dans laquelle ce trouve les bande descriptive et expressions
 */
void resize_bande_haut (Bande_haute* bande_haute);

/**
 * Calcule la première coordonnée x du <num_element>ème élément (de gauche à droite) de la bande d'expression
 * @param tab Le tableau avec la première position de la bande en x et les width de chaque élément dans l'ordre
 * @param espaces L'actuelle taille des espaces inter-éléments
 * @param num_element Le numéro de l'élément voulu
 * @return La position en x en pixel de la première coordonnée x du <num_element>ème élément
 */
int calcul_pos (int tab[NB_ELEMENTS_PAR_EXPRESSION + 1], int espaces, int num_element);

/**
 * Permet de nettoyer la mémoire allouée pour une bande d'expression
 * @param expression La bande d'expression à nettoyer
 */
void free_bande_expression (Expression_fonction* expression);

/**
 * Supprime la bande d'expression donnée
 * @param bande_haute La bande d'entrées dans laquelle ce trouve la bande d'expression
 * @param num_expression Le numéro de l'expression à supprimer
 */
void suppr_bande_expression (Bande_haute* bande_haute, int num_expression);

/**
 * Actions à effectuer après une modification de l'offset
 * @param bande_haute La bande d'entrées à afficher
 */
void action_apres_modif_offset (Bande_haute* bande_haute);

/**
 * Cache la bande d'expression et tous ses composants si elle ne doit plus être visible
 * @param bande_haute La bande d'entrées à afficher
 * @param expression La bande de l'expression, donc les entrées à modifier
 */
void cacher_expression_si_nessessaire (Bande_haute* bande_haute, Expression_fonction* expression);

/**
 * Ajoute une nouvelle bande d'expression à la bande haute
 * @param ren Un pointeur sur une structure contenant l'état du rendu
 * @param bande_haute La bande d'entrées dans laquelle ce trouve la bande d'expression
 */
void ajout_bande_expression (SDL_Renderer* ren, Bande_haute* bande_haute);

/**
 * Actions à effectuer après avoir changé la taille de la bande des entrées
 * @param bande_haute La bande d'entrées à afficher
 */
void actions_apres_resize_bande_haute (Bande_haute* bande_haute);

/**
 * Génère des exemples de fonctions 2D pour l'utilisateur
 * @return Une structure <Exemples> contenant les intervals, nom et définition des fonctions exemples
 */
Exemples exemples_fonctions_2D ();

/**
 * Génère des exemples de fonctions 3D pour l'utilisateur
 * @return Une structure <Exemples> contenant les intervals, nom et définition des fonctions exemples
 */
Exemples exemples_fonctions_3D ();

/**
 * Gère le mouvement de la bande d'expression
 * @param bande_haute La bande haute qui contient la bande d'expression
 * @param expression La bande de l'expression qui bouge
 * @param motion La valeur du déplacement à effectuer.
 */
void moving_bande_expression (Bande_haute* bande_haute, Expression_fonction* expression, int motion);

/**
 * Permet de faire tous les changement néssecaires lorque l'ordre de position d'une bande d'expression est modifiée
 * @param bande_haute La bande haute qui contient la bande d'expression
 * @param i L'index de la nouvelle position de la bande d'expression à aclimater (elle doit déjà y avoir été déplacée)
 * @param diection La direction du changement : 1 si l'index à augmenté, -1 si l'index à diminué. 
 */
void aclimater_une_bande_expression_a_sa_nouvelle_position (Bande_haute* bande_haute, int i, int direction);


#endif
