#include "grapheur_3D.h"


void affiche_interface_graph_3D (SDL_Renderer* ren, Bande_haute* bande_haute, Bande_droite* bande_droite, Graph_3D_1* graph_3D_1){
    SDL_SetRenderDrawColor(ren, colors->bg.r, colors->bg.g, colors->bg.b, colors->bg.a);
    SDL_RenderClear(ren);

    affiche_bande_haut(ren, bande_haute);
    // Rectangle pour cacher les bande d'expression qui ne sont qu'a moitié sur la bande haute. 
    boxRGBA(ren, bande_haute->surface.x, bande_haute->surface.y + bande_haute->surface.h - RAYON_BAS_BANDE_HAUT, bande_haute->surface.x + bande_haute->surface.w, bande_haute->surface.y + bande_haute->surface.h + bande_haute->params.height_bande_expression, colors->bg.r, colors->bg.g, colors->bg.b, colors->bg.a);

    renderGraph3D_1(ren, graph_3D_1, bande_haute);

    // Dessiner le bas arrondi de la bande haute
    affiche_bande_arrondis_en_bas(ren, bande_haute->surface.x, bande_haute->surface.y + bande_haute->surface.h - TAILLE_BARRE_BASSE_DE_BANDE_HAUT, bande_haute->surface.x + bande_haute->surface.w, bande_haute->surface.y + bande_haute->surface.h, RAYON_BAS_BANDE_HAUT, colors->bande_bas_de_bande_haut);
    renderImageButton(ren, &bande_haute->button_new_expression.bt);
    // Affichage de la bande droite
    affiche_bande_droite(ren, bande_droite);

    for (int j = 0; j < bande_haute->nb_expressions; j++) {
        affiche_interface_color_picker(ren, bande_haute->expressions[j]->color_picker);
    }

    if (message.is_visible){
        if (time(NULL) - message.start_time > message.temps_affichage){
            message.is_visible = 0;
        } else {
            renderButton(ren, &message.button_base);
        }
    }
}


void init_totale_interface_grapheur_3D (SDL_Renderer* ren, Grapheur_3D_elements *gr_ele){
    dimention = _3D;
    init_bande_droite(ren, gr_ele->bande_droite);
    init_bande_haute(ren, gr_ele->bande_haute);

    gr_ele->graph_3D_1->rotation = (Quaternion){1, 0, 0, 0};
    gr_ele->graph_3D_1->zoom = 0.7f * (FEN_X - TAILLE_BANDE_DROITE < FEN_Y - TAILLE_BANDE_HAUT ? FEN_X - TAILLE_BANDE_DROITE : FEN_Y - TAILLE_BANDE_HAUT) / 10.0f;
    gr_ele->graph_3D_1->origine_y_apres_bande_haut = gr_ele->bande_haute->surface.y + gr_ele->bande_haute->surface.h;
}

int Grapheur_3D (SDL_Renderer* ren, Grapheur_3D_elements *gr_ele){
    dimention = _3D;
    Bande_haute* bande_haute = gr_ele->bande_haute;
    Bande_droite* bande_droite = gr_ele->bande_droite;
    Graph_3D_1* graph_3D_1 = gr_ele->graph_3D_1;
    

    resize_fen_3D(bande_haute, bande_droite);
    SDL_StartTextInput();
    bool is_event_backspace_used = false;
    int x_souris_px, y_souris_px;
    int mode_quitter = 0; // Les différentes façons de quitter le grapheur : 0: pas quitter, 1: quitter la fenêtre, 2:quitter et revenir au menu principal 
    bool running = true;

    while (running) {
        affiche_interface_graph_3D(ren, bande_haute, bande_droite, graph_3D_1);

        mode_quitter = handle_all_events_3D(ren, graph_3D_1, bande_haute, bande_droite, &x_souris_px, &y_souris_px, &is_event_backspace_used);
        if (mode_quitter) break;

        // Animation de l'agrandissement
        if (bande_haute->expanding && bande_haute->surface.h < TAILLE_BANDE_EXPRESSIONS_MAX) {
            bande_haute->surface.h += (TAILLE_BANDE_EXPRESSIONS_MAX - TAILLE_BANDE_EXPRESSIONS_MIN) / 3;
            graph_3D_1->origine_y_apres_bande_haut = bande_haute->surface.y + bande_haute->surface.h;
            actions_apres_resize_bande_haute(bande_haute);
        } else if (!bande_haute->expanding && bande_haute->surface.h > TAILLE_BANDE_EXPRESSIONS_MIN) {
            bande_haute->surface.h -= (TAILLE_BANDE_EXPRESSIONS_MAX - TAILLE_BANDE_EXPRESSIONS_MIN) / 3;
            graph_3D_1->origine_y_apres_bande_haut = bande_haute->surface.y + bande_haute->surface.h;
            actions_apres_resize_bande_haute(bande_haute);
        }

        updateDisplay(ren);
    }
    SDL_StopTextInput();
    return mode_quitter - 1;
}
