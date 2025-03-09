#ifndef GRAPHIQUE_H
#define GRAPHIQUE_H

#include "ressources.h"
#include "entrees_expressions.h"

#define TAILLE_BANDE_DROITE 100
#define NB_GRAD_MIN 5
#define NB_GRAD_MAX 20

typedef struct {
    Button button_base;
    int temps_affichage; // en secondes
    SDL_Color couleur_message;
    SDL_Color couleur_fond;
    int is_visible;
    time_t start_time;
} Message;

typedef struct {
    int precision; // Précision des graduations (nombre de chiffres après la virgule)
    int nb_grad; // Nombre de graduations
    float taille_grad; // Taille en pixels d'une graduation (espacement entre les graduations)
    float echelle_grad; // Echelle de graduation (ex: une grad tout les 0.1, 1 ou 10)
    float min; // Valeur minimale visible
    float max; // Valeur maximale visible
    int skip_graduation; // Il faut écrire la valeur de la graduation tout les <skip_graduation> graduations
    int grad_text_size;
    TTF_Font* font_texte_grad;
} Axe;



typedef struct {
    Axe* axe_x;
    Axe* axe_y;
    float x; // Largeur du graph en pixels
    float y; // Hauteur du graph en pixels
    int origine_x; // Origine en x
    int origine_y; // Origine en y
} Graph;


/**
 * Fonction mathématique f(x) pour du test
 * @param x La valeur de x
 * @return La valeur de f(x)
 */
float f (float x);

/**
 * Fonction principale du grapheur
 * @param ren Un pointeur sur une structure contenant l'état du rendu
 */
void Grapheur (SDL_Renderer* ren);

/**
 * Redimensionne le graphique
 * @param graph Le graphique à redimensionner
 */
void resize_graph (Graph* graph);

/**
 * Affiche le quadrillage du graphique au niveau des graduations
 * @param ren Un pointeur sur une structure contenant l'état du rendu
 * @param graph Le graphique à afficher
 */
void affiche_quadrillage (SDL_Renderer* ren, Graph* graph);

/**
 * Initialise un graphique avec des valeurs par défaut, centré autour le la fonction par défaut
 * @param fonction_defaut La fonction par défaut sur laquelle sera centrée le graphique
 * @return Le graphique initialisé
 */
Graph init_graph (Fonction* fonction_defaut);

/**
 * Affiche les axes du graphique avec les graduations
 * @param ren Un pointeur sur une structure contenant l'état du rendu
 * @param graph Le graphique à afficher
 */
void affiche_axes_graph (SDL_Renderer* ren, Graph* graph, SDL_Color color_axes);

/**
 * Affiche l'interface utilisateur
 * @param ren Un pointeur sur une structure contenant l'état du rendu
 * @param graph Le graphique à afficher
 */
void affiche_interface (SDL_Renderer* ren, Graph* graph, Bande_entrees* bande_entrees, Colors* colors);

/**
 * Change le mode de couleur
 * @param colors Les couleurs à modifier
 * @param color_mode Le mode de couleur à appliquer
 */
void change_color_mode (Colors* colors, int color_mode);

/**
 * @param renderer Un pointeur sur une structure contenant l'état du rendu
 * @param x La position en x du point
 * @param y La position en y du point
 * @param size L'épaisseur du point
 */
void draw_thick_point(SDL_Renderer *renderer, int x, int y, int size);

/**
 * Trace la fonction f(x) sur le graphique
 * @param ren Un pointeur sur une structure contenant l'état du rendu
 * @param graph Le graphique à afficher
 * @param fonction La fonction à afficher
 */
void tracer_fonction (SDL_Renderer* ren, Graph* graph, Fonction fonction);

/**
 * Cherche et trouve le maximum et le minimum de la fonction. Et affecte les valeurs trouvés aux paramètres de la fonction
 * @param fonction La fonction à étudier
 * @param steps Le nombre de points à étudier
 */
void find_min_max(Fonction* fonction, int steps);

#endif
