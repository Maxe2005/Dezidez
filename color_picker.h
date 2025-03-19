#ifndef COLOR_PICKER 
#define COLOR_PICKER

#include "ressources.h"

#define NB_COULEURS_CLASSIQUES 8
#define MARGE_AVEC_FENETRE 10
#define MARGE_AVEC_BOUTON 10
#define MARGES_INTERIEURS 10
#define TAILLE_BANDE_HAUTE 30 // La taille de la bande "attrappable" du haut
#define TAILLE_PICKER 256
#define ESPACE_ENTRE_PICKER_ET_COULEURS_CLASSIQUES 10
#define TAILLE_COULEURS_CLASSIQUES 25
#define TRANSPARANCE_HOVERED 150


typedef struct {
    SDL_Rect rect_moving;
    SDL_Rect rect_initial;
    int boutton_x;
    int boutton_y;
    int boutton_y_affiche;
    int boutton_taille;
    int boutton_taille_hover;
    bool hovered;
    bool show_picker;
    bool dragging_picker;
    bool picker_clicked;
    SDL_Color selected_color;
    SDL_Color* color_dest; // La destination du choix de couleur
    SDL_Color bg_color;
    SDL_Color color_bande_haute;
    int espace_entre_classic_colors;
    ImageButton boutton_quitter;
    SDL_Rect boutton_quitter_rect_base;
} Color_picker;

extern SDL_Color classic_colors[];

/**
 * Affiche la palette color picker
 * @param renderer Un pointeur sur une structure contenant l'état du rendu
 * @param picker Le color picker à afficher
 */
void drawPaletteColorPicker(SDL_Renderer *renderer, Color_picker* picker);

/**
 * Initialise un color picker
 * @param renderer Un pointeur sur une structure contenant l'état du rendu
 * @param x La position centrale du bouton d'activation du color picker
 * @param y La position centrale du bouton d'activation du color picker
 * @param taille La taille (en x et y) du bouton (carré) d'activation du color picker
 * @param default_color La première couleur pické par le color picker
 * @return Un color picker initialisé
 */
Color_picker init_color_picker (SDL_Renderer *renderer, int x, int y, int taille, SDL_Color* color_dest, SDL_Color default_color);

/**
 * Affiche le boutton d'activation du color picker
 * @param renderer Un pointeur sur une structure contenant l'état du rendu
 * @param picker Le color picker à afficher
 */
void affiche_boutton_color_picker (SDL_Renderer *renderer, Color_picker* picker);

/**
 * Affiche tous les éléments de l'interface du color picker
 * @param renderer Un pointeur sur une structure contenant l'état du rendu
 * @param picker Le color picker à afficher
 */
void affiche_interface_color_picker (SDL_Renderer *renderer, Color_picker* picker);

/**
 * Initialise le placement du module color picker dans l'espace entre le bouton et dans la fenêtre
 * @param cp Le color picker à afficher
 */
void init_placement_color_picker (Color_picker* cp);

/**
 * Séléctionne la couleur adaptée en fonction de la position de la souris
 * @param cp Le color picker affiché
 * @param x_souris_px La position en pixel de la souris
 * @param y_souris_px La position en pixel de la souris
 */
void choix_color_in_picker (Color_picker* cp, int x_souris_px, int y_souris_px);

/**
 * Gère l'évènement MOUSEBUTTONDOWN relatif au color picker
 * @param event L'événement à gérer
 * @param cp Le color picker affiché
 * @param x_souris_px La position en pixel de la souris
 * @param y_souris_px La position en pixel de la souris
 */
void handle_event_color_picker_MOUSEBUTTONDOWN (Color_picker* cp, int x_souris_px, int y_souris_px);

/**
 * Gère l'évènement MOUSEBUTTONUP relatif au color picker
 * @param cp Le color picker affiché
 * @param x_souris_px La position en pixel de la souris
 * @param y_souris_px La position en pixel de la souris
 * @return true si le clic à été utile, false sinon
 */
bool handle_event_color_picker_MOUSEBUTTONUP (Color_picker* cp, int x_souris_px, int y_souris_px);

/**
 * Gère l'évènement MOUSEMOTION relatif au color picker
 * @param event L'événement à gérer
 * @param cp Le color picker affiché
 * @param x_souris_px La position en pixel de la souris
 * @param y_souris_px La position en pixel de la souris
 */
void handle_event_color_picker_SDL_MOUSEMOTION (SDL_Event event, Color_picker* cp, int x_souris_px, int y_souris_px);

#endif
