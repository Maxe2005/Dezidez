#ifndef GRAPHIQUE_H
#define GRAPHIQUE_H

#include "ressources.h"
#include "entrees_expressions.h"
// Il y a aussi l'include de "events.h" mais il est fait après les déclarations de structures car il en a besoins

#define NB_GRAD_MIN 5
#define NB_GRAD_MAX 20
#define ZOOM_SPEED 5
#define TAILLE_GRADUATION_MIN 40
#define TAILLE_GRADUATION_MAX 100
#define MARGE_EXT_GRAPH 5 // Marge autorisé en dehors du cadre du graph pour avoir des frontières fluides 
#define NB_EVALUATEUR_MAX 10

extern Message message;

typedef struct {
    int precision; // Précision des graduations (nombre de chiffres après la virgule)
    int nb_grad; // Nombre de graduations
    int pos_premiere_grad; // La position en pixel de la première graduation dans le sens de l'axe
    float taille_grad; // Taille en pixels d'une graduation (espacement entre les graduations)
    float echelle_grad; // Echelle de graduation (ex: une grad tout les 0.1, 1 ou 10)
    float min; // Valeur minimale visible
    float max; // Valeur maximale visible
    int grad_text_size;
    TTF_Font* font_texte_grad;
} Axe;

typedef struct {
    Button bouton_evaluateur;
    ImageButton boutton_quitter;
}Evaluateur;


typedef struct {
    Axe* axe_x;
    Axe* axe_y;
    float x; // Largeur du graph en pixels
    float y; // Hauteur du graph en pixels
    int origine_x; // Origine du graphique en x (coin en haut à gauche)
    int origine_y; // Origine du graphique en y (coin en haut à gauche)
    int origine_y_apres_bande_haut; // Origine du graphique en y (coin en haut à gauche)
    int centre_x; // Position en pixel du centre (ou de l'origine du graphique)
    int centre_y; // Position en pixel du centre (ou de l'origine du graphique)
    bool souris_pressee;
    int mode_clic_souris;
    Evaluateur liste_evaluateurs[NB_EVALUATEUR_MAX];
    int nombre_evaluateur;
} Graph;

typedef struct {
    Graph* graph;
    Bande_haute* bande_haute;
} Grapheur_elements;

#include "events.h"

/**
 * Initialise l'interface du grapheur
 * @param ren Un pointeur sur une structure contenant l'état du rendu
 * @param gr_ele Les éléments du grapheur
 */
void init_totale_interface_grapheur (SDL_Renderer* ren, Grapheur_elements *gr_ele);

/**
 * Fonction principale du grapheur
 * @param ren Un pointeur sur une structure contenant l'état du rendu
 * @param gr_ele Les éléments du grapheur
 * @return 1 s'il faut fermer la fenêtre, 2 s'il faut retourner à l'écran d'accueil
 */
int Grapheur (SDL_Renderer* ren, Grapheur_elements *gr_ele);

/**
 * Redimensionne le graphique et le centre sur la fonction donnée
 * @param graph Le graphique à redimensionner
 * @param fonction_defaut La fonction par défaut sur laquelle sera centrée le graphique
 */
void resize_recentrer (Graph* graph, Fonction* fonction_defaut);

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
 * @param bande_haute La bande d'entrées à afficher
 */
void affiche_interface (SDL_Renderer* ren, Graph* graph, Bande_haute* bande_haute);

/**
 * Change le mode de couleur
 * @param color_mode Le mode de couleur à appliquer
 */
void change_color_mode (int color_mode);

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

/**
 * Permet de trouver la meilleur échelle de graduation en fonction du min et du max pour obtenir un nombre de graduation voulues (paramétrés dans la fonction recherche_meilleur_echelle_grad)
 * @param max Le maximum de la fonction f(x) considérée
 * @param min Le minimum de la fonction f(x) considérée
 * @return la meilleur échelle de graduation en fonction du min et du max
 */
float recherche_meilleur_echelle_grad (float max, float min);

/**
 * Arrondi un float en fonction de son ordre de grandeur (ex : 0.1245 -> 0.12 et 0.00037843 -> 0.00038)
 * @param x Le nombre à arrondir
 * @return Le nombre correctement arrondi
 */
float arrondir_ordre_grandeur(float x);

/**
 * Redimentionne les éléments nécessaires après avoir naviger sur le graphique : 
 * pos_premiere_grad (La position de la première graduation),
 * nb_grad (Le nombre de graduation visibles) et 
 * le point-centre en x et y
 * @param graph Le graphique à afficher
 */
void resize_navigation (Graph* graph);

/**
 * Redimentionne les contours du graphique donc son origine et sa taille
 * @param graph Le graphique à afficher
 */
void resize_contours_graph (Graph* graph);

/**
 * Redimentionne la précision des graduation (nb de chiffres après la virgule)
 * @param graph Le graphique à afficher
 */
void resize_precision_grad (Graph* graph);

/**
 * Modifie la taille des graduation, ajuste l'echelle des graduation si necessaire et modifie le min et le max de chaque axe
 * @param event L'événement à gérer
 * @param graph Le graphique affiché
 * @param x_souris_px La position en pixel de la souris
 * @param y_souris_px La position en pixel de la souris
 */
void zoomer (SDL_Event event, Graph* graph, int x_souris_px, int y_souris_px);

/**
 * Actions à effectuer après avoir chager la taille de la bande des entrées
 * @param graph Le graphique à afficher
 * @param bande_haute La bande d'entrées à afficher
 */
void actions_apres_resize_bande_haute (Graph* graph, Bande_haute* bande_haute);

/**
 * Initialisation des constantes pour la structure message
 */
void init_const_message();


#endif
