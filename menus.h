#ifndef MENUS_H
#define MENUS_H

#include "ressources.h"
#include "grapheur.h"

#define NB_BOUTONS_ACCUEIL 3

/**
 * Début du lancement du jeu
 * @param ren Un pointeur sur une structure contenant l'état du rendu
 */
void ecran_acceuil (SDL_Renderer* ren);

/**
 * Affiche le titre de la page d'accueil
 * @param ren Un pointeur sur une structure contenant l'état du rendu
 */
void affiche_titre (SDL_Renderer* ren);

/**
 * Initialise les boutons de l'écran d'accueil
 * @param buttons Le tableau de boutons à initialiser
 * @param button_musique Le bouton musique
 * @param button_niveaux Le bouton niveaux
 * @param button_remerciements Le bouton remerciements
 * @param button_createur_map Le bouton createur de map
 */
void init_buttons_accueil(Button* buttons[], Button* button_mode_emploi, Button* button_remerciements, Button* button_grapheur);

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
 * @param running Un pointeur sur un entier qui permet de quitter la boucle de jeu
 */
void handle_events_accueil(Button* buttons[], SDL_Renderer* ren, int *running);

/**
 * Affiche une page de texte scrollable
 * @param ren Un pointeur sur une structure contenant l'état du rendu
 * @param Text Le texte à afficher
 * @param titre Le titre de la page
 */
void ecran_text (SDL_Renderer* ren, const char* Text[], char* titre);

/**
 * Tout est dans le titre !
 * @param ren Un pointeur sur une structure contenant l'état du rendu
 */
void ecran_remerciements (SDL_Renderer* ren);

/**
 * Affiche, sous forme de texte déroulant, le mode d'emploi du grapheur d'expressions fonctionnelles
 * @param ren Un pointeur sur une structure contenant l'état du rendu
 */
void ecran_mode_emploi (SDL_Renderer* ren);

#endif
