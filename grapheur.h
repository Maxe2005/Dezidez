#ifndef GRAPHIQUE_H
#define GRAPHIQUE_H

#include "ressources.h"
#include "entrees_expressions.h"

#define TAILLE_BANDE_DROITE 100

typedef struct {
    Button button_base;
    int temps_affichage; // en secondes
    SDL_Color couleur_message;
    SDL_Color couleur_fond;
    int is_visible;
    time_t start_time;
} Message;

typedef struct {
    int nb_grad_x; // Nombre de graduations en x
    int nb_grad_y; // Nombre de graduations en y
    float taille_grad_x; // Taille en pixels d'une graduation en x (espacement entre les graduations)
    float taille_grad_y; // Taille en pixels d'une graduation en y (espacement entre les graduations)
    float echelle_grad_x; // Echelle de graduation en x (ex: une grad tout les 0.1, 1 ou 10)
    float echelle_grad_y; // Echelle de graduation en y (ex: une grad tout les 0.1, 1 ou 10)
    float x; // Largeur du graph en pixels
    float y; // Hauteur du graph en pixels
    int origine_x; // Origine en x
    int origine_y; // Origine en y
    float min_x; // Valeur minimale en x
    float max_x; // Valeur maximale en x
    float min_y; // Valeur minimale en y
    float max_y; // Valeur maximale en y
    int grad_text_size;
    TTF_Font* font_texte_grad;
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
 * Initialise un graphique avec des valeurs par défaut
 * @return Le graphique initialisé
 */
Graph init_graph ();

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

#endif
