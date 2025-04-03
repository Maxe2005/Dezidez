#ifndef MENUS_H
#define MENUS_H

#include "ressources.h"
#include "grapheur.h"
#include "Gestion_3D/grapheur_3D.h"

#define NB_BOUTONS_ACCUEIL 4
#define NB_LANGUES 2

typedef struct {
    SDL_Rect dstRect;
    int textureWidth;
    int textureHeight;
    SDL_Texture* backgroundTexture;
    bool is_filtre; // Pour savoir si un flitre doit être appliqué
    SDL_Color color_filtre;
    SDL_Rect filtre;
} Background;

typedef struct {
    ImageButton actual_lang;
    ImageButton langues_dispo[NB_LANGUES];
    SDL_Rect rect_fond;
    bool langues_dispo_visibles;
    int taille; // Le diamètre des cercles en pixels
    int espace_entre; // L'espacement entre 2 drapeaux
} Langues_bt;

typedef struct {
    Button* buttons[NB_BOUTONS_ACCUEIL];
    Background* bg;
    WrappedText *titre;
    Langues_bt* lang_bt;
} Accueil;

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
 * @param titre Le titre wrappé
 */
void affiche_titre (SDL_Renderer* ren, WrappedText *titre);

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
 * @param ren Un pointeur sur une structure contenant l'état du rendu
 * @param acceuil Les éléments de l'acceuil
 * @param running Un pointeur sur un entier qui permet de quitter la boucle de jeu
 * @param gr_ele Les éléments du grapheur
 * @param grapheur_ele_3D Les éléments du grapheur 3D
 */
void handle_events_accueil(SDL_Renderer* ren, Accueil* accueil, int *running, Grapheur_elements *gr_ele, Grapheur_3D_elements *grapheur_ele_3D);

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

/**
 * Redimentionne tout l'ecran d'accueil
 * @param acceuil Les éléments de l'acceuil
 */
void resize_ecran_acceuil (Accueil* accueil);

/**
 * Actualise tous les textes de l'interface en fonction de la langue choisie
 * @param gr_ele Les éléments du grapheur
 * @param grapheur_ele_3D Les éléments du grapheur 3D
 * @param acceuil Les éléments de l'acceuil
 */
void set_all_textes_by_lang (Grapheur_elements *gr_ele, Grapheur_3D_elements *grapheur_ele_3D, Accueil* accueil);

/**
 * Affiche le selectionneur de langues
 * @param ren Un pointeur sur une structure contenant l'état du rendu
 * @param lang_bt La structure pour la selection des langues
 */
void affiche_langues (SDL_Renderer* ren, Langues_bt* lang_bt);

/**
 * Initialise le selectionneur de langues
 * @param ren Un pointeur sur une structure contenant l'état du rendu
 * @param lang_bt La structure pour la selection des langues
 */
void init_bt_langues (SDL_Renderer* ren, Langues_bt* lang_bt);

/**
 * Redimentionne la position du séléctionneur de laugue
 * @param lang_bt La structure pour la selection des langues
 */
void resize_lang_bt (Langues_bt* lang_bt);

#endif
