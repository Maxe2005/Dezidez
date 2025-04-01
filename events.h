#ifndef EVENT_H 
#define EVENT_H

#include "ressources.h"
#include "bande_haute.h"
#include "bande_droite.h"
#include "grapheur.h"



/**
 * Gère l'évènement MOUSEWHEEL relatif au graphique
 * @param event L'événement à gérer
 * @param graph Le graphique affiché
 * @param x_souris_px La position en pixel de la souris
 * @param y_souris_px La position en pixel de la souris
 */
void handle_event_graph_MOUSEWHEEL(SDL_Event event, Graph* graph, int x_souris_px, int y_souris_px);

/**
 * Gère l'évènement MOUSEBUTTONDOWN relatif au graphique
 * @param event L'événement à gérer
 * @param graph Le graphique affiché
 * @param x_souris_px La position en pixel de la souris
 * @param y_souris_px La position en pixel de la souris
 */
void handle_event_graph_MOUSEBUTTONDOWN(SDL_Event event, Graph* graph, int x_souris_px, int y_souris_px);

/**
 * Gère l'évènement MOUSEBUTTONUP du graphique et renvoie vers MOUSEBUTTONUP_RIGHT ou MOUSEBUTTON_LEFT en fonction du besoin
 * @param ren Un pointeur sur une structure contenant l'état du rendu
 * @param event L'événement à gérer
 * @param graph Le graphique affiché
 * @param x_souris_px La position en pixel de la souris
 * @param y_souris_px La position en pixel de la souris
 * @param bande_haute La bande d'entrées du graphique
 */
void handle_event_graph_MOUSEBUTTONUP(SDL_Renderer* ren, SDL_Event event, Graph* graph, int x_souris_px, int y_souris_px, Bande_haute* bande_haute);



/**
 * Gère l'évènement MOUSEBUTTONUP_RIGHT du graphique
 * @param event L'événement à gérer
 * @param graph Le graphique affiché
 * @param x_souris_px La position en pixel de la souris
 * @param y_souris_px La position en pixel de la souris
 */
void handle_event_graph_MOUSEBUTTONUP_RIGHT (SDL_Event event, Graph* graph, int x_souris_px, int y_souris_px);


/**
 * Gère l'évènement MOUSEBUTTONUP_LEFT du graphique
 * @param ren Un pointeur sur une structure contenant l'état du rendu
 * @param event L'événement à gérer
 * @param graph Le graphique affiché
 * @param x_souris_px La position en pixel de la souris
 * @param y_souris_px La position en pixel de la souris
 * @param bande_haute La bande d'entrées du graphique
 */
void handle_event_graph_MOUSEBUTTONUP_LEFT (SDL_Renderer* ren, SDL_Event event, Graph* graph, int x_souris_px, int y_souris_px,Bande_haute* bande_haute);


/**
 * Gère l'évènement MOUSEMOTION relatif au graphique
 * @param event L'événement à gérer
 * @param graph Le graphique affiché
 * @param x_souris_px La position en pixel de la souris
 * @param y_souris_px La position en pixel de la souris
 */
void handle_event_graph_MOUSEMOTION(SDL_Event event, Graph* graph, int x_souris_px, int y_souris_px);


/**
 * Gère l'évènement MOUSEMOTION relatif à la bande haute
 * @param event L'événement à gérer
 * @param bande_haute La bande d'entrées à afficher
 * @param x_souris_px La position en pixel de la souris
 * @param y_souris_px La position en pixel de la souris
 * @return 1 si le <mouvement de la souris> à été géré, 0 sinon
 */
bool handle_event_bande_haut_MOUSEMOTION(SDL_Event event, Bande_haute* bande_haute, int x_souris_px, int y_souris_px);

/**
 * Gère l'évènement MOUSEBUTTONUP relatif à la bande haute
 * @param ren Un pointeur sur une structure contenant l'état du rendu
 * @param event L'événement à gérer
 * @param bande_haute La bande d'entrées à afficher
 * @param x_souris_px La position en pixel de la souris
 * @param y_souris_px La position en pixel de la souris
 * @return 1 si le <clic souris> à été géré, 0 sinon
 */
bool handle_event_bande_haut_MOUSEBUTTONUP(SDL_Renderer* ren, SDL_Event event, Bande_haute* bande_haute, int x_souris_px, int y_souris_px);

/**
 * Gère l'évènement MOUSEBUTTONDOWN relatif à la bande haute
 * @param event L'événement à gérer
 * @param bande_haute La bande d'entrées à afficher
 * @param x_souris_px La position en pixel de la souris
 * @param y_souris_px La position en pixel de la souris
 * @return 1 si le <clic souris> à été géré, 0 sinon
 */
bool handle_event_bande_haut_MOUSEBUTTONDOWN (SDL_Event event, Bande_haute* bande_haute, int x_souris_px, int y_souris_px);

/**
 * Gère l'évènement MOUSEWHEEL relatif à la bande haute
 * @param event L'événement à gérer
 * @param bande_haute La bande d'entrées à afficher
 * @param x_souris_px La position en pixel de la souris
 * @param y_souris_px La position en pixel de la souris
 */
void handle_event_bande_haut_MOUSEWHEEL(SDL_Event event, Bande_haute* bande_haute, int x_souris_px, int y_souris_px);

/**
 * Gère l'évènement KEYUP relatif à la bande haute
 * @param event L'événement à gérer
 * @param bande_haute La bande d'entrées à afficher
 */
void handle_event_bande_haut_KEYUP(SDL_Event event, Bande_haute* bande_haute);

/**
 * Gère l'évènement KEYDOWN relatif à la bande haute
 * @param event L'événement à gérer
 * @param bande_haute La bande d'entrées à afficher
 * @return 1 si l'event <backspace> est géré, 0 sinon
 */
bool handle_event_bande_haut_KEYDOWN(SDL_Event event, Bande_haute* bande_haute);

/**
 * Gère l'évènement TEXTINPUT relatif à la bande haute
 * @param event L'événement à gérer
 * @param bande_haute La bande d'entrées à afficher
 */
void handle_event_bande_haut_TEXTINPUT(SDL_Event event, Bande_haute* bande_haute);


/**
 * Gère l'évènement MOUSEBUTTONUP relatif à une bande d'expression de la bande haute
 * @param event L'événement à gérer
 * @param bande_haute La bande d'entrées à afficher
 * @param expression La bande de l'expression, donc les entrées à modifier
 * @param x_souris_px La position en pixel de la souris
 * @param y_souris_px La position en pixel de la souris
 * @return 1 si le <clic souris> à été géré, 0 sinon
 */
bool handle_event_entrees_expressions_MOUSEBUTTONUP(SDL_Event event, Bande_haute* bande_haute, Expression_fonction* expression, int x_souris_px, int y_souris_px);

/**
 * Gère l'évènement MOUSEMOTION relatif à une bande d'expression de la bande haute
 * @param event L'événement à gérer
 * @param bande_haute La bande d'entrées à afficher
 * @param expression La bande de l'expression, donc les entrées à modifier
 * @param x_souris_px La position en pixel de la souris
 * @param y_souris_px La position en pixel de la souris
 * @param is_MOUSEMOTION_used 1 si le <clic souris> à déjà été géré, 0 sinon
 */
void handle_event_entrees_expressions_MOUSEMOTION(SDL_Event event, Bande_haute* bande_haute, Expression_fonction* expression, int x_souris_px, int y_souris_px, int is_MOUSEMOTION_used);

/**
 * Gère l'évènement KEYUP relatif à une bande d'expression de la bande haute
 * @param event L'événement à gérer
 * @param expression La bande de l'expression, donc les entrées à modifier
 */
void handle_event_entrees_expressions_KEYUP(SDL_Event event, Expression_fonction* expression);

/**
 * Gère l'évènement KEYDOWN relatif à une bande d'expression de la bande haute
 * @param event L'événement à gérer
 * @param expression La bande de l'expression, donc les entrées à modifier
 */
void handle_event_entrees_expressions_KEYDOWN(SDL_Event event, Expression_fonction* expression);

/**
 * Gère l'évènement TEXTINPUT relatif à une bande d'expression de la bande haute
 * @param event L'événement à gérer
 * @param expression La bande de l'expression, donc les entrées à modifier
 */
void handle_event_entrees_expressions_TEXTINPUT(SDL_Event event, Expression_fonction* expression);

/**
 * Gère l'évènement MOUSEMOTION relatif à la bande droite de l'interface
 * @param event L'événement à gérer
 * @param bande_droite La bande droite affichée
 * @param x_souris_px La position en pixel de la souris
 * @param y_souris_px La position en pixel de la souris
 */
void handle_event_bande_droite_MOUSEMOTION (SDL_Event event, Bande_droite* bande_droite, int x_souris_px, int y_souris_px);

/**
 * Gère l'évènement MOUSEMOTION relatif à la bande droite de l'interface
 * @param ren Un pointeur sur une structure contenant l'état du rendu
 * @param event L'événement à gérer
 * @param bande_droite La bande droite affichée
 * @param bande_haute La bande d'entrées à afficher
 * @param graph Le graphique affiché
 * @param x_souris_px La position en pixel de la souris
 * @param y_souris_px La position en pixel de la souris
 * @return 0 si l'event n'est pas géré, -1 s'il faut revenir à l'accueil, 1 si l'event est géré
 */
int handle_event_bande_droite_MOUSEBUTTONUP (SDL_Renderer* ren, SDL_Event event, Bande_droite* bande_droite, Bande_haute* bande_haute, Graph* graph, int x_souris_px, int y_souris_px);

/**
 * Redimentionne tous les éléments de la fenêtre
 * @param bande_haute La bande d'entrées affiché
 * @param bande_droite La bande droite affichée
 * @param graph Le graphique affiché
 */
void resize_fen_2D (Bande_haute* bande_haute, Bande_droite* bande_droite, Graph* graph);

/**
 * Gère tous les évènement par type
 * @param ren Un pointeur sur une structure contenant l'état du rendu
 * @param bande_haute La bande d'entrées affiché
 * @param bande_droite La bande droite affichée
 * @param graph Le graphique affiché
 * @param x_souris_px La position en abssices actuelle de la souris 
 * @param y_souris_px La position en ordonnées actuelle de la souris 
 * @param is_event_backspace_used Permet de séparer (ou hiérarchiser) l'utilisation de la touche backspace pour plusieurs utilisation
 */
int handle_all_events (SDL_Renderer* ren, Bande_haute* bande_haute, Bande_droite* bande_droite, Graph* graph, int* x_souris_px, int* y_souris_px, bool* is_event_backspace_used);

/**
 * Gère tous les évènement par type
 * @param ren Un pointeur sur une structure contenant l'état du rendu
 * @param bande_haute La bande d'entrées affiché
 * @param bande_droite La bande droite affichée
 * @param x_souris_px La position en abssices actuelle de la souris 
 * @param y_souris_px La position en ordonnées actuelle de la souris 
 * @param is_event_backspace_used Permet de séparer (ou hiérarchiser) l'utilisation de la touche backspace pour plusieurs utilisation
 */
int handle_all_events_3D (SDL_Renderer* ren, Bande_haute* bande_haute, Bande_droite* bande_droite, int* x_souris_px, int* y_souris_px, bool* is_event_backspace_used);

#endif
