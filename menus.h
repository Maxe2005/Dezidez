#ifndef MENUS_H
#define MENUS_H

#include "ressources.h"
#include "grapheur.h"
#include "grapheur_3D.h"

#define NB_BOUTONS_ACCUEIL 4

typedef struct {
    SDL_Rect dstRect;
    int textureWidth;
    int textureHeight;
    SDL_Texture* backgroundTexture;
    bool is_filtre; // Pour savoir si un flitre doit être appliqué
    SDL_Color color_filtre;
    SDL_Rect filtre;
} Background;

/**
 * Début du lancement du jeu
 * @param ren Un pointeur sur une structure contenant l'état du rendu
 * @param gr_ele Les éléments du grapheur
 * @param grapheur_ele_3D Les éléments du grapheur 3D
 */
void ecran_acceuil (SDL_Renderer* ren, Grapheur_elements *gr_ele, Grapheur_3D_elements *grapheur_ele_3D);

/**
 * Affiche le titre de la page d'accueil
 * @param ren Un pointeur sur une structure contenant l'état du rendu
 */
void affiche_titre (SDL_Renderer* ren);

/**
 * Initialise les boutons de l'écran d'accueil
 * @param buttons Le tableau de boutons à initialiser
 * @param button_mode_emploi Le bouton "Mode d'emploi"
 * @param button_remerciements Le bouton "Remerciements"
 * @param button_grapheur Le bouton "Grapheur"
 * @param button_grapheur_3D Le bouton "Grapheur 3D"
 */
void init_buttons_accueil(Button* buttons[], Button* button_mode_emploi, Button* button_remerciements, Button* button_grapheur, Button* button_grapheur_3D);

/**
 * Affiche les boutons de l'écran d'accueil
 * @param ren Un pointeur sur une structure contenant l'état du rendu
 * @param buttons Le tableau de boutons à afficher
 */
void affiche_boutons_accueil(SDL_Renderer* ren, Button* buttons[]);

/**
 * Gère les événements des boutons de l'écran d'accueil
 * @param buttons Le tableau de boutons
 * @param ren Un pointeur sur une structure contenant l'état du rendu
 * @param bg L'image de fond
 * @param running Un pointeur sur un entier qui permet de quitter la boucle de jeu
 * @param gr_ele Les éléments du grapheur
 * @param grapheur_ele_3D Les éléments du grapheur 3D
 */
void handle_events_accueil(Button* buttons[], SDL_Renderer* ren, Background* bg, int *running, Grapheur_elements *gr_ele, Grapheur_3D_elements *grapheur_ele_3D);

/**
 * Affiche une page de texte scrollable
 * @param ren Un pointeur sur une structure contenant l'état du rendu
 * @param markdown_file Le texte à afficher
 * @param titre Le titre de la page
 * @return 1 si on revient sur la page d'accueil, 0 si on ferme la fenêtre
 */
int ecran_text (SDL_Renderer* ren, const char* markdown_file, char* titre);

/**
 * Tout est dans le titre !
 * @param ren Un pointeur sur une structure contenant l'état du rendu
 * @return 1 si on revient sur la page d'accueil, 0 si on ferme la fenêtre
 */
int ecran_remerciements (SDL_Renderer* ren);

/**
 * Affiche, sous forme de texte déroulant, le mode d'emploi du grapheur d'expressions fonctionnelles
 * @param ren Un pointeur sur une structure contenant l'état du rendu
 * @return 1 si on revient sur la page d'accueil, 0 si on ferme la fenêtre
 */
int ecran_mode_emploi (SDL_Renderer* ren);

/**
 * Initialise l'image de fond contenue dans Ressources/background/
 * @param ren Un pointeur sur une structure contenant l'état du rendu
 * @param filename Le nom de l'image de fond avec son extention
 * @param bg La structure nessessaire pour l'image de fond
 */
void init_background (SDL_Renderer* ren, const char* filename, Background* bg);

/**
 * Redimentionne l'image de fond en fonction de la taille de la fenêtre
 * @param bg La structure nessessaire pour l'image de fond
 */
void resize_background (Background* bg);

/**
 * Affiche l'image de fond sur le renderer
 * @param ren Un pointeur sur une structure contenant l'état du rendu
 * @param bg La structure nessessaire pour l'image de fond
 */
void affiche_background (SDL_Renderer* ren, Background* bg);

/**
 * Libère l'espace pour l'image de fond et sa structure quand elle n'est plus utilisée
 * @param bg La structure nessessaire pour l'image de fond
 */
void free_background (Background* bg);

/**
 * Redimentione les boutons de la page d'acceuil en fonction de la taille de la fenêtre
 * @param boutons Un tableau avec tous les boutons à redimentionner
 */
void resize_boutons_acceuil (Button* boutons[]);

#endif
