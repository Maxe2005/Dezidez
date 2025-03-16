#ifndef EVENT_H 
#define EVENT_H

#include "ressources.h"
#include "entrees_expressions.h"
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
 * Gère l'évènement MOUSEBUTTONUP relatif au graphique
 * @param event L'événement à gérer
 * @param graph Le graphique affiché
 * @param x_souris_px La position en pixel de la souris
 * @param y_souris_px La position en pixel de la souris
 */
void handle_event_graph_MOUSEBUTTONUP(SDL_Event event, Graph* graph, int x_souris_px, int y_souris_px);

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
 * @param bande_entrees La bande d'entrées à afficher
 * @param x_souris_px La position en pixel de la souris
 * @param y_souris_px La position en pixel de la souris
 */
void handle_event_bande_haut_MOUSEMOTION(SDL_Event event, Bande_entrees* bande_entrees, int x_souris_px, int y_souris_px);

/**
 * Gère l'évènement MOUSEBUTTONUP relatif à la bande haute
 * @param event L'événement à gérer
 * @param bande_entrees La bande d'entrées à afficher
 * @param x_souris_px La position en pixel de la souris
 * @param y_souris_px La position en pixel de la souris
 */
void handle_event_bande_haut_MOUSEBUTTONUP(SDL_Event event, Bande_entrees* bande_entrees, int x_souris_px, int y_souris_px);

/**
 * Gère l'évènement MOUSEWHEEL relatif à la bande haute
 * @param event L'événement à gérer
 * @param bande_entrees La bande d'entrées à afficher
 * @param x_souris_px La position en pixel de la souris
 * @param y_souris_px La position en pixel de la souris
 */
void handle_event_bande_haut_MOUSEWHEEL(SDL_Event event, Bande_entrees* bande_entrees, int x_souris_px, int y_souris_px);

/**
 * Gère l'évènement KEYUP relatif à la bande haute
 * @param event L'événement à gérer
 * @param bande_entrees La bande d'entrées à afficher
 */
void handle_event_bande_haut_KEYUP(SDL_Event event, Bande_entrees* bande_entrees);

/**
 * Gère l'évènement KEYDOWN relatif à la bande haute
 * @param event L'événement à gérer
 * @param bande_entrees La bande d'entrées à afficher
 * @return 1 si l'event <backspace> est géré, 0 sinon
 */
bool handle_event_bande_haut_KEYDOWN(SDL_Event event, Bande_entrees* bande_entrees);

/**
 * Gère l'évènement TEXTINPUT relatif à la bande haute
 * @param event L'événement à gérer
 * @param bande_entrees La bande d'entrées à afficher
 */
void handle_event_bande_haut_TEXTINPUT(SDL_Event event, Bande_entrees* bande_entrees);


/**
 * Gère l'évènement MOUSEBUTTONUP relatif à une bande d'expression de la bande haute
 * @param event L'événement à gérer
 * @param expression La bande de l'expression, donc les entrées à modifier
 * @param x_souris_px La position en pixel de la souris
 * @param y_souris_px La position en pixel de la souris
 * @return 1 si le <clic souris> à été géré, 0 sinon
 */
bool handle_event_entrees_expressions_MOUSEBUTTONUP(SDL_Event event, Expression_fonction* expression, int x_souris_px, int y_souris_px);

/**
 * Gère l'évènement MOUSEMOTION relatif à une bande d'expression de la bande haute
 * @param event L'événement à gérer
 * @param expression La bande de l'expression, donc les entrées à modifier
 * @param x_souris_px La position en pixel de la souris
 * @param y_souris_px La position en pixel de la souris
 */
void handle_event_entrees_expressions_MOUSEMOTION(SDL_Event event, Expression_fonction* expression, int x_souris_px, int y_souris_px);

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
 * Actions à effectuer après une modification de l'offset
 * @param bande_entrees La bande d'entrées à afficher
 */
void action_apres_modif_offset (Bande_entrees* bande_entrees);



#endif
