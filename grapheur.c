#include "grapheur.h"

// TODO ajouter les free et les SDL_DestroyTexture (pour les boutons image et fonts)

void resize_navigation (Graph* graph){
    graph->axe_x->pos_premiere_grad = graph->origine_x + fmodf(0-graph->axe_x->min, graph->axe_x->echelle_grad) * graph->axe_x->taille_grad / graph->axe_x->echelle_grad;
    graph->axe_x->nb_grad = (graph->x - graph->axe_x->pos_premiere_grad + graph->origine_x) / graph->axe_x->taille_grad;

    graph->axe_y->pos_premiere_grad = graph->origine_y + fmodf(graph->axe_y->max, graph->axe_y->echelle_grad) * graph->axe_y->taille_grad / graph->axe_y->echelle_grad;
    graph->axe_y->nb_grad = (graph->y - graph->axe_y->pos_premiere_grad + graph->origine_y) / graph->axe_y->taille_grad;

    graph->centre_x = graph->origine_x + (0-graph->axe_x->min) * graph->axe_x->taille_grad / graph->axe_x->echelle_grad;
    graph->centre_y = graph->origine_y + graph->axe_y->max * graph->axe_y->taille_grad / graph->axe_y->echelle_grad;
}

void resize_contours_graph (Graph* graph){
    graph->origine_x = 0;
    graph->origine_y = TAILLE_BANDE_HAUT;
    graph->x = FEN_X - TAILLE_BANDE_DROITE;
    graph->y = FEN_Y - TAILLE_BANDE_HAUT;
}

void resize_precision_grad (Graph* graph){
    int presision_max = 10;

    Axe* axes[] = {graph->axe_x, graph->axe_y};
    for (int i = 0; i < 2; i++) {
        // Précision des graduations (nombre de chiffres après la virgule)
        axes[i]->precision = 0;
        for (int j = presision_max; j >= 0; j--) {
            if (axes[i]->echelle_grad < pow(10, 0-j)) {
                axes[i]->precision = j+1;
                break;
            }
        }
    }
}

void resize_recentrer (Graph* graph, Fonction* fonction_defaut){
    find_min_max(fonction_defaut, 1e7);

    graph->axe_x->echelle_grad = recherche_meilleur_echelle_grad(fonction_defaut->borne_sup, fonction_defaut->borne_inf);
    graph->axe_x->min = fonction_defaut->borne_inf - graph->axe_x->echelle_grad;
    graph->axe_x->max = fonction_defaut->borne_sup + graph->axe_x->echelle_grad;

    graph->axe_y->echelle_grad = recherche_meilleur_echelle_grad(fonction_defaut->fx_max, fonction_defaut->fx_min);
    graph->axe_y->min = fonction_defaut->fx_min - graph->axe_y->echelle_grad;
    graph->axe_y->max = fonction_defaut->fx_max + graph->axe_y->echelle_grad;

    if (graph->axe_x->min >= graph->axe_x->max) printf("Erreur : min > max");//graph->axe_x->max = graph->axe_x->min + 1;
    graph->axe_x->taille_grad = graph->x * graph->axe_x->echelle_grad / (graph->axe_x->max - graph->axe_x->min);
    
    if (graph->axe_y->min >= graph->axe_y->max) printf("Erreur : min > max");//graph->axe_y->max = graph->axe_y->min + 1;
    graph->axe_y->taille_grad = graph->y * graph->axe_y->echelle_grad / (graph->axe_y->max - graph->axe_y->min);

    resize_precision_grad(graph);
    resize_navigation(graph);
}

void affiche_quadrillage (SDL_Renderer* ren, Graph* graph){
    SDL_SetRenderDrawColor(ren, 50, 50, 50, 255);
    for (int i = 0; i<=graph->axe_x->nb_grad; i++) {
        SDL_RenderDrawLine(ren, graph->axe_x->pos_premiere_grad + graph->axe_x->taille_grad*i, graph->origine_y_apres_bande_haut, graph->axe_x->pos_premiere_grad + graph->axe_x->taille_grad*i, graph->origine_y + graph->y);
    }
    for (int i = 0; i<=graph->axe_y->nb_grad; i++) {
        if (graph->axe_y->pos_premiere_grad + graph->axe_y->taille_grad*i > graph->origine_y_apres_bande_haut) {
            SDL_RenderDrawLine(ren, graph->origine_x, graph->axe_y->pos_premiere_grad + graph->axe_y->taille_grad*i, graph->origine_x + graph->x, graph->axe_y->pos_premiere_grad + graph->axe_y->taille_grad*i);
        }
    }
}

void affiche_axes_graph (SDL_Renderer* ren, Graph* graph, SDL_Color color_axes){
    SDL_SetRenderDrawColor(ren, color_axes.r, color_axes.g, color_axes.b, color_axes.a);

    // Axe des abscisses
    int y_axis_pos = graph->origine_y + graph->y - 5;
    graph->y_axis_pos = y_axis_pos;
    if (graph->axe_y->max <= 0) {
        y_axis_pos = graph->origine_y + 5;
    }
    else if (graph->axe_y->min <= 0 && graph->axe_y->max >= 0) {
        y_axis_pos = graph->centre_y;
    }
    if (y_axis_pos < graph->origine_y_apres_bande_haut + 5){
        y_axis_pos = graph->origine_y_apres_bande_haut + 5;
    }
    SDL_RenderDrawLine(ren, graph->origine_x, y_axis_pos, graph->origine_x + graph->x, y_axis_pos);

    // Axe des ordonnées
    int x_axis_pos = graph->origine_x + 5;
    if (graph->axe_x->max <= 0) {
        x_axis_pos = graph->origine_x + graph->x - 5;
    }
    else if (graph->axe_x->min <= 0 && graph->axe_x->max >= 0) {
        x_axis_pos = graph->centre_x;
    }
    SDL_RenderDrawLine(ren, x_axis_pos, graph->origine_y_apres_bande_haut, x_axis_pos, graph->origine_y + graph->y);

    // Traits perpendiculaires et valeurs des graduations en x
    for (int i = 0; i <= graph->axe_x->nb_grad; i++) {
        int x = graph->axe_x->pos_premiere_grad + i * graph->axe_x->taille_grad;
        if (abs(x_axis_pos - x) > 3 && x > graph->origine_x && x < graph->origine_x + graph->x) {
            SDL_RenderDrawLine(ren, x, y_axis_pos - 5, x, y_axis_pos + 5);
            // Afficher les valeurs des graduations en x
            char label[10];
            snprintf(label, 10, "%.*f", graph->axe_x->precision, graph->axe_x->min + fmodf(0-graph->axe_x->min, graph->axe_x->echelle_grad) + i * graph->axe_x->echelle_grad);
            renderText(ren, (const char*)label, x, y_axis_pos + 2*graph->axe_x->grad_text_size, color_axes, graph->axe_x->font_texte_grad);
        }
    }

    for (int i = 0; i <= graph->axe_y->nb_grad; i++) {
        int y = graph->axe_y->pos_premiere_grad + i * graph->axe_y->taille_grad;
        if (abs(y_axis_pos - y) > 3 && y > graph->origine_y_apres_bande_haut && y < graph->origine_y + graph->y) {
            SDL_RenderDrawLine(ren, x_axis_pos - 5, y, x_axis_pos + 5, y);
            // Afficher les valeurs des graduations en y
            char label[10];
            snprintf(label, 10, "%.*f", graph->axe_y->precision, graph->axe_y->max - fmodf(graph->axe_y->max, graph->axe_y->echelle_grad) - i * graph->axe_y->echelle_grad);
            renderText(ren, (const char*)label, x_axis_pos - (graph->axe_y->precision + 1)*2*graph->axe_x->grad_text_size, y, color_axes, graph->axe_y->font_texte_grad);
        }
    }
}

void tracer_fonction (SDL_Renderer* ren, Graph* graph, Fonction fonction){
    if (fonction.borne_sup > graph->axe_x->min && fonction.borne_inf < graph->axe_x->max){
        SDL_SetRenderDrawColor(ren, fonction.color.r, fonction.color.g, fonction.color.b, fonction.color.a);
        int nb_pts = graph->x;
        float borne_sup = (fonction.borne_sup > graph->axe_x->max) ? graph->axe_x->max : fonction.borne_sup;
        float borne_inf = (fonction.borne_inf < graph->axe_x->min) ? graph->axe_x->min : fonction.borne_inf;;
        float step_size = (borne_sup - borne_inf) / nb_pts;
        float x, y_sur_graph, x_sur_graph;
        float valeur;
        for (int i = 0; i < nb_pts; i++) {
            x = borne_inf + i * step_size;
            float valeur = fonction.f(x);
            if (valeur >= graph->axe_y->min && valeur <= graph->axe_y->max){
                y_sur_graph = graph->origine_y + (graph->axe_y->max - valeur) / graph->axe_y->echelle_grad * graph->axe_y->taille_grad;
                if (y_sur_graph > graph->origine_y_apres_bande_haut){
                    x_sur_graph = graph->origine_x + (0-graph->axe_x->min + x) / graph->axe_x->echelle_grad * graph->axe_x->taille_grad;
                    draw_thick_point(ren, x_sur_graph, y_sur_graph, 3);
                }
            }
        }
    }
}

Graph init_graph (Fonction* fonction_defaut){
    Graph graph;
    
    // Axe x
    graph.axe_x = malloc(sizeof(Axe));
    graph.axe_x->font_texte_grad = NULL;

    // Axe y
    graph.axe_y = malloc(sizeof(Axe));
    graph.axe_y->font_texte_grad = NULL;

    resize_contours_graph(&graph);
    resize_recentrer(&graph, fonction_defaut);

    // Déterminer la taille du texte en fonction de la taille du graphique
    int text_size;
    Axe* axes[] = {graph.axe_x, graph.axe_y};
    for (int i = 0; i < 2; i++) {
        text_size = (axes[i]->taille_grad < 20) ? 10 : 12;
        if (axes[i]->grad_text_size != text_size) {
            axes[i]->grad_text_size = text_size;
            if (axes[i]->font_texte_grad != NULL) TTF_CloseFont(axes[i]->font_texte_grad);
            axes[i]->font_texte_grad = createFont("Ressources/Fonts/DejaVuSans-Bold.ttf", text_size);
        }
    }
    graph.nombre_evaluateur = 0;
    return graph;
}

void change_color_mode (int color_mode){
    if (color_mode == 0){
        colors->bg = (SDL_Color){255, 255, 255, 255};
        colors->axes = (SDL_Color){0, 0, 0, 255};
        colors->texte_axes = (SDL_Color){0, 0, 0, 255};
    } else {
        colors->bg = (SDL_Color){0, 0, 0, 255};
        colors->axes = (SDL_Color){255, 255, 255, 255};
        colors->texte_axes = (SDL_Color){255, 255, 255, 255};
    }
    colors->bande_droite = (SDL_Color){100, 100, 100, 255};
    colors->bande_haute_expressions = (SDL_Color){200, 200, 200, 255};
    colors->bande_haute_description = (SDL_Color){150, 150, 150, 255};
    colors->texte_champ_entree = (SDL_Color){0, 0, 0, 255};
    colors->texte_descriptifs_bande_haut = (SDL_Color){255, 255, 255, 255};
    colors->bg_bandes_expression_1 = colors->bande_haute_expressions;
    colors->bg_bandes_expression_2 = (SDL_Color){150, 150, 150, 255};
    colors->bande_bas_de_bande_haut = (SDL_Color){200, 200, 200, 255};
    colors->button_new_expression = (SDL_Color){125, 125, 125, 255};
    colors->button_new_expression_hover = (SDL_Color){150, 150, 150, 255};
}

float arrondir_ordre_grandeur(float x) {
    if (x == 0.0f) return 0.0f; // Éviter log10(0)

    int exp = floor(log10(fabsf(x))); // Trouver l'ordre de grandeur
    float facteur = powf(10, exp-1);    // Facteur d'échelle pour avoir 2 chiffres significatifs

    return roundf(x / facteur) * facteur; // Arrondir et remettre à l'échelle
}

float recherche_meilleur_echelle_grad (float max, float min){
    int nb_essai_avant_erreur = 10e5;
    
    int direction_recherche; // Prend les valeurs 1 ou -1 et permet de savoir si on cherche une valeur plus élevée ou moins élevée.
    int nb_grad_max = 15;
    int nb_grad_min = 8;
    float echelle_grad_max = 1;
    float echelle_grad_min = 1;
    float echelle_grad = (echelle_grad_max + echelle_grad_min) / 2;
    int nb_grad = fabsf(max - min) / echelle_grad;
    if (nb_grad >= nb_grad_min && nb_grad <= nb_grad_max){
        return echelle_grad;
    } else {
        if (nb_grad < nb_grad_min){
            echelle_grad_min /= 2;
            echelle_grad = echelle_grad_min;
            direction_recherche = -1;
        } else {
            echelle_grad_max *= 2;
            echelle_grad = echelle_grad_max;
            direction_recherche = 1;
        }
    }

    for (int i = 0; i < nb_essai_avant_erreur; i++) {
        nb_grad = fabsf(max - min) / echelle_grad;
        if (nb_grad >= nb_grad_min && nb_grad <= nb_grad_max){
            float echelle_grad_arrondi = arrondir_ordre_grandeur(echelle_grad);
            nb_grad = fabsf(max - min) / echelle_grad_arrondi;
            if (nb_grad >= nb_grad_min && nb_grad <= nb_grad_max) return echelle_grad_arrondi;
            else {
                printf("nb graduations : %d\n", nb_grad);
                return echelle_grad;
            }
        } else {
            if (nb_grad < nb_grad_min){
                if (echelle_grad == echelle_grad_min && direction_recherche == -1){
                    echelle_grad_max = echelle_grad_min;
                    echelle_grad_min /= 2;
                    echelle_grad = echelle_grad_min;
                } else {
                    if (echelle_grad == echelle_grad_min){
                        direction_recherche = 0;
                    } else {
                        echelle_grad_max = echelle_grad;
                    }
                    echelle_grad = (echelle_grad_max + echelle_grad_min) / 2;
                }
            } else {
                if (echelle_grad == echelle_grad_max && direction_recherche == 1){
                    echelle_grad_min = echelle_grad_max;
                    echelle_grad_max *= 2;
                    echelle_grad = echelle_grad_max;
                } else {
                    if (echelle_grad == echelle_grad_max){
                        direction_recherche = 0;
                    } else {
                        echelle_grad_min = echelle_grad;
                    }
                    echelle_grad = (echelle_grad_max + echelle_grad_min) / 2;
                }
            }
        }
    }
    printf("Trop d'itérations réalisés dans 'recherche_meilleur_echelle_grad'\n");
    return 1;
}

void affichage_graph_evaluateur(SDL_Renderer* ren, Graph* graph){
    for (int i=0; i<graph->nombre_evaluateur; i++) {
        renderButton(ren, &graph->liste_evaluateurs[i].bouton_evaluateur);
}
}

void affiche_interface (SDL_Renderer* ren, Graph* graph, Bande_haute* bande_haute){
    SDL_SetRenderDrawColor(ren, colors->bg.r, colors->bg.g, colors->bg.b, colors->bg.a);
    SDL_RenderClear(ren);

    affiche_bande_haut(ren, bande_haute);
    // Rectangle pour cacher les bande d'expression qui ne sont qu'a moitié sur la bande haute. 
    boxRGBA(ren, bande_haute->surface.x, bande_haute->surface.y + bande_haute->surface.h - RAYON_BAS_BANDE_HAUT, bande_haute->surface.x + bande_haute->surface.w, bande_haute->surface.y + bande_haute->surface.h + bande_haute->params.height_bande_expression, colors->bg.r, colors->bg.g, colors->bg.b, colors->bg.a);

    affiche_quadrillage(ren, graph);
    affiche_axes_graph(ren, graph, colors->axes);

    for (int i = 0; i < bande_haute->nb_expressions; i++) {
        if (bande_haute->expressions[i]->fonction.visible && bande_haute->expressions[i]->expression->text[0] != '\0') {
            tracer_fonction(ren, graph, bande_haute->expressions[i]->fonction);
        }
    }
    affichage_graph_evaluateur(ren,graph);
    // Dessiner le bas arrondi de la bande haute
    affiche_bande_arrondis_en_bas(ren, bande_haute->surface.x, bande_haute->surface.y + bande_haute->surface.h - TAILLE_BARRE_BASSE_DE_BANDE_HAUT, bande_haute->surface.x + bande_haute->surface.w, bande_haute->surface.y + bande_haute->surface.h, RAYON_BAS_BANDE_HAUT, colors->bande_bas_de_bande_haut);
    renderImageButton(ren, &bande_haute->button_new_expression.bt);
    // Affichage de la bande droite
    boxRGBA(ren, FEN_X - TAILLE_BANDE_DROITE, 0, FEN_X, FEN_Y, colors->bande_droite.r, colors->bande_droite.g, colors->bande_droite.b, colors->bande_droite.a);
    
    if (message.is_visible){
        if (time(NULL) - message.start_time > message.temps_affichage){
            message.is_visible = 0;
        } else {
            renderButton(ren, &message.button_base);
        }
    }

    for (int j = 0; j < bande_haute->nb_expressions; j++) {
        affiche_interface_color_picker(ren, bande_haute->expressions[j]->color_picker);
    }
}

void draw_thick_point(SDL_Renderer *renderer, int x, int y, int size) {
    if (size < 1) size = 1; // Assurer une taille valide

    SDL_Rect rect = {x - size / 2, y - size / 2, size, size};
    SDL_RenderFillRect(renderer, &rect);
}

void find_min_max(Fonction* fonction, int steps) {
    if (fonction->borne_inf >= fonction->borne_sup) {
        printf("Erreur: borne inférieure plus grande que borne supérieure dans 'find_min_max'\n");
        return;
    }

    float step_size = (fonction->borne_sup - fonction->borne_inf) / steps;
    fonction->fx_min = fonction->fx_max = f(fonction->borne_inf); // Initialisation

    for (int i = 1; i <= steps; i++) {
        float x = fonction->borne_inf + i * step_size;
        float y = f(x);

        if (y < fonction->fx_min) fonction->fx_min = y;
        if (y > fonction->fx_max) fonction->fx_max = y;
    }
}

void zoomer (SDL_Event event, Graph* graph, int x_souris_px, int y_souris_px){
    float old_taille_grad_x = graph->axe_x->taille_grad;
    float old_taille_grad_y = graph->axe_y->taille_grad;
    graph->axe_x->taille_grad += event.wheel.y * ZOOM_SPEED;
    graph->axe_y->taille_grad += event.wheel.y * ZOOM_SPEED;

    graph->axe_x->min +=  ((x_souris_px - graph->origine_x) / old_taille_grad_x) * event.wheel.y * ZOOM_SPEED * graph->axe_x->echelle_grad / graph->axe_x->taille_grad;
    graph->axe_x->max -=  ((graph->x - x_souris_px + graph->origine_x) / old_taille_grad_x) * event.wheel.y * ZOOM_SPEED * graph->axe_x->echelle_grad / graph->axe_x->taille_grad;
    graph->axe_y->max -=  ((y_souris_px - graph->origine_y) / old_taille_grad_y) * event.wheel.y * ZOOM_SPEED * graph->axe_y->echelle_grad / graph->axe_y->taille_grad;
    graph->axe_y->min +=  ((graph->y - y_souris_px + graph->origine_y) / old_taille_grad_y) * event.wheel.y * ZOOM_SPEED * graph->axe_y->echelle_grad / graph->axe_y->taille_grad;
    
    if (graph->axe_x->taille_grad < TAILLE_GRADUATION_MIN){
        graph->axe_x->echelle_grad *= 2;
        graph->axe_x->taille_grad *= 2;
    }
    if (graph->axe_x->taille_grad > TAILLE_GRADUATION_MAX){
        graph->axe_x->echelle_grad /= 2;
        graph->axe_x->taille_grad /= 2;
    }
    if (graph->axe_y->taille_grad < TAILLE_GRADUATION_MIN){
        graph->axe_y->echelle_grad *= 2;
        graph->axe_y->taille_grad *= 2;
    }
    if (graph->axe_y->taille_grad > TAILLE_GRADUATION_MAX){
        graph->axe_y->echelle_grad /= 2;
        graph->axe_y->taille_grad /= 2;
    }

    resize_navigation(graph);
    resize_precision_grad(graph);
}

void actions_apres_resize_bande_haute (Graph* graph, Bande_haute* bande_haute){
    graph->origine_y_apres_bande_haut = bande_haute->surface.y + bande_haute->surface.h;
    for (int i = 0; i < bande_haute->nb_expressions; i++) {
        cacher_expression_si_nessessaire(bande_haute, bande_haute->expressions[i]);
    }
    bande_haute->button_new_expression.bt.rect.y = bande_haute->surface.y + bande_haute->surface.h - 1.15*bande_haute->button_new_expression.bt.rect.h;
}

void init_const_message(){
    message.temps_affichage = 5;
    message.button_base.is_survolable = 0;
    message.button_base.color_base = (SDL_Color) {255,0,0,255};
    message.button_base.radius = 15;
    message.button_base.font_text = fonts[5];
    message.button_base.color_text = (SDL_Color) {255,255,255,255};

}

void init_totale_interface_grapheur (SDL_Renderer* ren, Grapheur_elements *gr_ele){
    colors = malloc(sizeof(Colors));
    change_color_mode(1);

    init_bande_haute(ren, gr_ele->bande_haute);

    *gr_ele->graph = init_graph(&gr_ele->bande_haute->expressions[0]->fonction);
    gr_ele->graph->souris_pressee = false;
    actions_apres_resize_bande_haute(gr_ele->graph, gr_ele->bande_haute);

    init_const_message();
    message.is_visible = 0;
}

int Grapheur (SDL_Renderer* ren, Grapheur_elements *gr_ele){
    Graph* graph = gr_ele->graph;
    Bande_haute* bande_haute = gr_ele->bande_haute;

    SDL_StartTextInput();
    bool is_event_backspace_used = false;
    int x_souris_px, y_souris_px;
    int mode_quitter = 0; // Les différentes façons de quitter le grapheur : 0: pas quitter, 1: quitter la fenêtre, 2:quitter et revenir au menu principal 
    bool running = true;

    while (running) {
        affiche_interface(ren, graph, bande_haute);

        mode_quitter = handle_all_events(ren, bande_haute, graph, &x_souris_px, &y_souris_px, &is_event_backspace_used);
        if (mode_quitter) break;

        // Animation de l'agrandissement
        if (bande_haute->expanding && bande_haute->surface.h < TAILLE_BANDE_EXPRESSIONS_MAX) {
            bande_haute->surface.h += (TAILLE_BANDE_EXPRESSIONS_MAX - TAILLE_BANDE_EXPRESSIONS_MIN) / 3;
            actions_apres_resize_bande_haute(graph, bande_haute);
        } else if (!bande_haute->expanding && bande_haute->surface.h > TAILLE_BANDE_EXPRESSIONS_MIN) {
            bande_haute->surface.h -= (TAILLE_BANDE_EXPRESSIONS_MAX - TAILLE_BANDE_EXPRESSIONS_MIN) / 3;
            actions_apres_resize_bande_haute(graph, bande_haute);
        }

        updateDisplay(ren);
    }
    SDL_StopTextInput();
    return mode_quitter - 1;
}
