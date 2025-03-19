#include "color_picker.h"

SDL_Color classic_colors[] = {
    // ! A faire correspondre avec <NB_COULEURS_CLASSIQUES>
    {255, 0, 0, 255},   // Rouge
    {0, 255, 0, 255},   // Vert
    {0, 0, 255, 255},   // Bleu
    {255, 255, 0, 255}, // Jaune
    {0, 255, 255, 255}, // Cyan
    {255, 0, 255, 255}, // Magenta
    {255, 255, 255, 255}, // Blanc
    {0, 0, 0, 255}      // Noir
};

void drawPaletteColorPicker(SDL_Renderer *renderer, Color_picker* picker) {    
    // Dessiner la palette RGB
    for (int x = 0; x < TAILLE_PICKER; x++) {
        for (int y = 0; y < TAILLE_PICKER; y++) {
            SDL_SetRenderDrawColor(renderer, x, y, 255 - x, 255);
            SDL_RenderDrawPoint(renderer, picker->rect_moving.x + MARGES_INTERIEURS + x, picker->rect_moving.y + TAILLE_BANDE_HAUTE + MARGES_INTERIEURS + y);
        }
    }
    
    // Dessiner les couleurs classiques
    int startY = picker->rect_moving.y + TAILLE_BANDE_HAUTE + MARGES_INTERIEURS + TAILLE_PICKER + ESPACE_ENTRE_PICKER_ET_COULEURS_CLASSIQUES;
    picker->espace_entre_classic_colors = (TAILLE_PICKER - NB_COULEURS_CLASSIQUES * TAILLE_COULEURS_CLASSIQUES) / (NB_COULEURS_CLASSIQUES-1);
    if (picker->espace_entre_classic_colors < 0) {
        printf("Pas assez de place pour les couleurs classiques");
        return;
    }
    for (int i = 0; i < NB_COULEURS_CLASSIQUES; i++) {
        SDL_Rect rect = {picker->rect_moving.x + MARGES_INTERIEURS + i * (TAILLE_COULEURS_CLASSIQUES + picker->espace_entre_classic_colors), startY, TAILLE_COULEURS_CLASSIQUES, TAILLE_COULEURS_CLASSIQUES};
        SDL_SetRenderDrawColor(renderer, classic_colors[i].r, classic_colors[i].g, classic_colors[i].b, 255);
        SDL_RenderFillRect(renderer, &rect);
    }
}

void init_placement_color_picker (Color_picker* cp){
    // Le placement à droite est-il possible ?
    if (cp->rect_initial.w <= FEN_X - cp->boutton_x - cp->boutton_taille/2 - MARGE_AVEC_BOUTON - MARGE_AVEC_FENETRE){
        cp->rect_initial.x = cp->boutton_x + cp->boutton_taille/2 + MARGE_AVEC_BOUTON;
        cp->rect_initial.y = cp->boutton_y_affiche - cp->rect_initial.h/2;
    } else { // Le placement en bas est-il possible ?
    if (cp->rect_initial.h <= FEN_Y - cp->boutton_y_affiche - cp->boutton_taille/2 - MARGE_AVEC_BOUTON - MARGE_AVEC_FENETRE){
        cp->rect_initial.y = cp->boutton_y_affiche + cp->boutton_taille/2 + MARGE_AVEC_BOUTON;
        cp->rect_initial.x = cp->boutton_x - cp->rect_initial.w/2;
    } else { // Le placement à gauche est-il possible ?
    if (cp->rect_initial.w <= cp->boutton_x - cp->boutton_taille/2 - MARGE_AVEC_BOUTON - MARGE_AVEC_FENETRE){
        cp->rect_initial.x = cp->boutton_x - cp->boutton_taille/2 - MARGE_AVEC_BOUTON - cp->rect_initial.w;
        cp->rect_initial.y = cp->boutton_y_affiche - cp->rect_initial.h/2;
    } else { // Le placement en haut est-il possible ?
    if (cp->rect_initial.h <= cp->boutton_y_affiche - cp->boutton_taille/2 - MARGE_AVEC_BOUTON - MARGE_AVEC_FENETRE){
        cp->rect_initial.y = cp->boutton_y_affiche - cp->boutton_taille/2 - MARGE_AVEC_BOUTON - cp->rect_initial.h;
        cp->rect_initial.x = cp->boutton_x - cp->rect_initial.w/2;
    } else {
        cp->rect_initial.x = cp->boutton_x + cp->boutton_taille/2 + MARGE_AVEC_BOUTON;
        cp->rect_initial.y = cp->boutton_y_affiche - cp->rect_initial.h/2;
    }}}}
    if (cp->rect_initial.x < MARGE_AVEC_FENETRE) cp->rect_initial.x = MARGE_AVEC_FENETRE;
    if (cp->rect_initial.y < MARGE_AVEC_FENETRE) cp->rect_initial.y = MARGE_AVEC_FENETRE;
    if (cp->rect_initial.x + cp->rect_initial.w > FEN_X - MARGE_AVEC_FENETRE) cp->rect_initial.x = FEN_X - MARGE_AVEC_FENETRE - cp->rect_initial.w;
    if (cp->rect_initial.y + cp->rect_initial.h > FEN_Y - MARGE_AVEC_FENETRE) cp->rect_initial.y = FEN_Y - MARGE_AVEC_FENETRE - cp->rect_initial.h;
    cp->boutton_quitter_rect_base.y = cp->rect_initial.y + TAILLE_BANDE_HAUTE * (100 - cp->boutton_quitter_pourcentage_du_header) / 200;
    cp->boutton_quitter_rect_base.x = cp->rect_initial.x + cp->rect_initial.w - cp->boutton_quitter_rect_base.w - TAILLE_BANDE_HAUTE/3;
}

Color_picker init_color_picker (SDL_Renderer *renderer, int x, int y, int taille, SDL_Color* color_dest, SDL_Color default_color){
    Color_picker cp;
    cp.boutton_x = x;
    cp.boutton_y = y;
    cp.boutton_y_affiche = y;
    cp.boutton_taille = taille;
    cp.boutton_taille_hover = taille * 1.1;
    cp.selected_color = default_color;
    cp.color_dest = color_dest;
    *cp.color_dest = default_color;
    cp.hovered = false;
    cp.show_picker = false;
    cp.dragging_picker = false;
    cp.picker_clicked = false;
    cp.rect_initial.w = 2*MARGES_INTERIEURS + TAILLE_PICKER;
    cp.rect_initial.h = TAILLE_BANDE_HAUTE + TAILLE_PICKER + ESPACE_ENTRE_PICKER_ET_COULEURS_CLASSIQUES + TAILLE_COULEURS_CLASSIQUES + 2*MARGES_INTERIEURS;

    cp.boutton_quitter_pourcentage_du_header = 80;
    cp.boutton_quitter.image = load_image(renderer, "Icons/croix.png");
    cp.boutton_quitter_rect_base.h = TAILLE_BANDE_HAUTE * cp.boutton_quitter_pourcentage_du_header/100;
    cp.boutton_quitter_rect_base.w = cp.boutton_quitter_rect_base.h;
    cp.boutton_quitter.is_survolable = 1;
    cp.boutton_quitter.color_base = cp.color_bande_haute;
    cp.boutton_quitter.color_hover = (SDL_Color){255, 0, 0, 255};
    cp.boutton_quitter.hovered = 0;
    cp.boutton_quitter.radius = cp.boutton_quitter_rect_base.w / 3;
    cp.boutton_quitter.pourcentage_place = 50;
    cp.boutton_quitter.taille_bonus_hover_x = 0;
    cp.boutton_quitter.taille_bonus_hover_y = 0;

    init_placement_color_picker(&cp);
    cp.boutton_quitter.rect = cp.boutton_quitter_rect_base;
    cp.rect_moving = cp.rect_initial;
    cp.color_bande_haute = (SDL_Color){150, 150, 150, 255};
    cp.bg_color = (SDL_Color){220, 220, 220, 255};
    return cp;
}

void affiche_boutton_color_picker (SDL_Renderer *renderer, Color_picker* cp){
    if (cp->hovered){
        filledCircleRGBA(renderer, cp->boutton_x, cp->boutton_y_affiche, cp->boutton_taille_hover/2, 0, 0, 0, 255);
        filledCircleRGBA(renderer, cp->boutton_x, cp->boutton_y_affiche, cp->boutton_taille_hover*(1 - 1/10.0)/2.0, cp->selected_color.r, cp->selected_color.g, cp->selected_color.b, cp->selected_color.a);
    } else {
        filledCircleRGBA(renderer, cp->boutton_x, cp->boutton_y_affiche, cp->boutton_taille/2, 0, 0, 0, 255);
        filledCircleRGBA(renderer, cp->boutton_x, cp->boutton_y_affiche, cp->boutton_taille*(1 - 1/10.0)/2.0, cp->selected_color.r, cp->selected_color.g, cp->selected_color.b, cp->selected_color.a);
    }
}

void affiche_interface_color_picker (SDL_Renderer *renderer, Color_picker* cp){
    if (cp->show_picker){
    // Contour du picker
    int arrondi = TAILLE_BANDE_HAUTE/2;
    roundedBoxRGBA(renderer, cp->rect_moving.x, cp->rect_moving.y, cp->rect_moving.x + cp->rect_moving.w, cp->rect_moving.y + cp->rect_moving.h, arrondi, cp->bg_color.r, cp->bg_color.g, cp->bg_color.b, cp->bg_color.a);
    affiche_bande_arrondis_en_haut(renderer, cp->rect_moving.x, cp->rect_moving.y, cp->rect_moving.x + cp->rect_moving.w, cp->rect_moving.y + TAILLE_BANDE_HAUTE, arrondi, cp->color_bande_haute);
    roundedRectangleRGBA(renderer, cp->rect_moving.x, cp->rect_moving.y, cp->rect_moving.x + cp->rect_moving.w, cp->rect_moving.y + cp->rect_moving.h, arrondi, 0, 0, 0, 255);
    drawPaletteColorPicker(renderer, cp);
    renderText(renderer, "Color Picker", cp->rect_moving.x + cp->rect_moving.w/2, cp->rect_moving.y + TAILLE_BANDE_HAUTE/2, cp->bg_color, fonts[5]);
    renderImageButton(renderer, &cp->boutton_quitter);
    }
}

void choix_color_in_picker (Color_picker* cp, int x_souris_px, int y_souris_px){
    if (x_souris_px >= cp->rect_moving.x + MARGES_INTERIEURS && x_souris_px <= cp->rect_moving.x + cp->rect_moving.w - MARGES_INTERIEURS &&
            y_souris_px >= cp->rect_moving.y + TAILLE_BANDE_HAUTE + MARGES_INTERIEURS && y_souris_px <= cp->rect_moving.y + TAILLE_PICKER + TAILLE_BANDE_HAUTE){
        cp->selected_color.r = x_souris_px - cp->rect_moving.x - MARGES_INTERIEURS;
        cp->selected_color.g = y_souris_px - cp->rect_moving.y - TAILLE_BANDE_HAUTE - MARGES_INTERIEURS;
        cp->selected_color.b = 255 - cp->selected_color.r;
        *cp->color_dest = cp->selected_color;
        cp->picker_clicked = true;
    }
}

bool handle_event_color_picker_MOUSEBUTTONDOWN (Color_picker* cp, int x_souris_px, int y_souris_px){
    bool is_MOUSEBUTTONDOWN_used = false;
    if (cp->show_picker){
        choix_color_in_picker(cp, x_souris_px, y_souris_px);
        if (y_souris_px > cp->rect_moving.y + TAILLE_BANDE_HAUTE + MARGES_INTERIEURS + TAILLE_PICKER + ESPACE_ENTRE_PICKER_ET_COULEURS_CLASSIQUES && y_souris_px < cp->rect_moving.y + cp->rect_moving.h - MARGES_INTERIEURS) {
            int index = (x_souris_px - cp->rect_moving.x - MARGES_INTERIEURS) / (TAILLE_COULEURS_CLASSIQUES + cp->espace_entre_classic_colors);
            if (index >= 0 && index < 8 && TAILLE_COULEURS_CLASSIQUES >= (x_souris_px - cp->rect_moving.x - MARGES_INTERIEURS) % (TAILLE_COULEURS_CLASSIQUES + cp->espace_entre_classic_colors)) {
                cp->selected_color = classic_colors[index];
                *cp->color_dest = cp->selected_color;
            }
        }
        if (x_souris_px >= cp->rect_moving.x && x_souris_px <= cp->rect_moving.x + cp->rect_moving.w && 
                y_souris_px >= cp->rect_moving.y && y_souris_px <= cp->rect_moving.y + TAILLE_BANDE_HAUTE) {
            cp->dragging_picker = true;
        }
        if (is_souris_sur_rectangle(cp->rect_moving, x_souris_px, y_souris_px)){
            is_MOUSEBUTTONDOWN_used = true;
        }
    }
    return is_MOUSEBUTTONDOWN_used;
}

bool handle_event_color_picker_MOUSEBUTTONUP (Color_picker* cp, int x_souris_px, int y_souris_px){
    bool clic_utile = false;
    if (cp->dragging_picker || cp->picker_clicked) clic_utile = true;
    cp->dragging_picker = false;
    cp->picker_clicked = false;
    
    if (is_souris_sur_rectangle(cp->boutton_quitter.rect, x_souris_px, y_souris_px)){
        cp->show_picker = 0;
        clic_utile = true;
    }
    return clic_utile;
}

bool handle_event_color_picker_SDL_MOUSEMOTION (SDL_Event event, Color_picker* cp, int x_souris_px, int y_souris_px){
    bool is_MOUSEMOTION_used = false;
    if (cp->dragging_picker) {
        if (x_souris_px >= cp->rect_moving.x && x_souris_px <= cp->rect_moving.x + cp->rect_moving.w && 
                y_souris_px >= cp->rect_moving.y && y_souris_px <= cp->rect_moving.y + TAILLE_BANDE_HAUTE) {
            cp->rect_moving.x += event.motion.xrel;
            cp->rect_moving.y += event.motion.yrel;
            cp->boutton_quitter.rect.x += event.motion.xrel;
            cp->boutton_quitter.rect.y += event.motion.yrel;
        } else cp->dragging_picker = false;
    } else if (cp->picker_clicked){
        choix_color_in_picker(cp, event.button.x, event.button.y);
    }
    if (is_souris_sur_rectangle(cp->boutton_quitter.rect, x_souris_px, y_souris_px)){
        cp->boutton_quitter.hovered = 1;
    } else cp->boutton_quitter.hovered = 0;
    if (is_souris_sur_rectangle(cp->rect_moving, x_souris_px, y_souris_px)){
        is_MOUSEMOTION_used = true;
    }
    return is_MOUSEMOTION_used;
}

