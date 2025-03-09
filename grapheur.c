#include "grapheur.h"

float f (float x) {
    return sin(x);
}

void resize_graph (Graph* graph){
    int marge = 0; 
    if (FEN_X - TAILLE_BANDE_DROITE < FEN_Y - TAILLE_BANDE_HAUT) {
        marge = (int)((FEN_X - TAILLE_BANDE_DROITE) / 15);
    } else {
        marge = (int)((FEN_Y - TAILLE_BANDE_HAUT) / 15);
    }
    int presision_max = 10;
    int text_size;
    graph->origine_x = marge;
    graph->origine_y = TAILLE_BANDE_HAUT + marge;
    graph->x = FEN_X-TAILLE_BANDE_DROITE - 2*marge;
    graph->y = FEN_Y - 2*marge - TAILLE_BANDE_HAUT;
    Axe* axes[] = {graph->axe_x, graph->axe_y};
    int taille[] = {graph->x, graph->y};
    for (int i = 0; i < 2; i++) {
        if (axes[i]->nb_grad < 1) axes[i]->nb_grad = 1;
        if (axes[i]->min >= axes[i]->max) axes[i]->max = axes[i]->min + 1;
        axes[i]->taille_grad = taille[i] / axes[i]->nb_grad;
        axes[i]->echelle_grad = (axes[i]->max - axes[i]->min) / axes[i]->nb_grad;
        // Précision des graduations (nombre de chiffres après la virgule)
        axes[i]->precision = 0;
        for (int j = presision_max; j >= 0; j--) {
            if (axes[i]->echelle_grad < pow(10, 0-j)) {
                axes[i]->precision = j+1;
                break;
            }
        }
        // Déterminer la taille du texte en fonction de la taille du graphique
        text_size = (axes[i]->taille_grad < 20) ? 10 : 12;
        if (axes[i]->grad_text_size != text_size) {
            axes[i]->grad_text_size = text_size;
            if (axes[i]->font_texte_grad != NULL) TTF_CloseFont(axes[i]->font_texte_grad);
            axes[i]->font_texte_grad = createFont("Ressources/DejaVuSans-Bold.ttf", text_size);
        }
        axes[i]->skip_graduation = (axes[i]->taille_grad < 20) ? 2 : 1;
    }
}

void affiche_quadrillage (SDL_Renderer* ren, Graph* graph){
    SDL_SetRenderDrawColor(ren, 50, 50, 50, 255);
    int nb_trait_x = graph->axe_x->nb_grad + 2*(graph->origine_x / (int)graph->axe_x->taille_grad);
    int nb_trait_y = graph->axe_y->nb_grad + 2*(graph->origine_y / (int)graph->axe_y->taille_grad);
    int marge_x = graph->origine_x % (int)graph->axe_x->taille_grad;
    int marge_y = (graph->origine_y - TAILLE_BANDE_HAUT) % (int)graph->axe_y->taille_grad;
    for (int i = 0; i<=nb_trait_x; i++) {
        SDL_RenderDrawLine(ren, marge_x + graph->axe_x->taille_grad*i, TAILLE_BANDE_HAUT, marge_x + graph->axe_x->taille_grad*i, FEN_Y);
    }
    for (int i = 0; i<=nb_trait_y; i++) {
        SDL_RenderDrawLine(ren, 0, TAILLE_BANDE_HAUT + marge_y + graph->axe_y->taille_grad*i, FEN_X - TAILLE_BANDE_DROITE, TAILLE_BANDE_HAUT + marge_y + graph->axe_y->taille_grad*i);
    }
}

Graph init_graph (Fonction* fonction_defaut){
    Graph graph;
    find_min_max(fonction_defaut, 1000);
    // Axe x
    graph.axe_x = malloc(sizeof(Axe));
    graph.axe_x->nb_grad = 10;
    graph.axe_x->min = fonction_defaut->borne_inf - ((fonction_defaut->borne_sup - fonction_defaut->borne_inf)/(graph.axe_x->nb_grad-2));
    graph.axe_x->max = fonction_defaut->borne_sup + ((fonction_defaut->borne_sup - fonction_defaut->borne_inf)/(graph.axe_x->nb_grad-2));
    graph.axe_x->font_texte_grad = NULL;

    // Axe y
    graph.axe_y = malloc(sizeof(Axe));
    graph.axe_y->nb_grad = 10;
    graph.axe_y->min = fonction_defaut->fx_min - ((fonction_defaut->fx_max - fonction_defaut->fx_min)/(graph.axe_y->nb_grad-2));
    graph.axe_y->max = fonction_defaut->fx_max + ((fonction_defaut->fx_max - fonction_defaut->fx_min)/(graph.axe_y->nb_grad-2));
    graph.axe_y->font_texte_grad = NULL;

    resize_graph(&graph);
    return graph;
}

void affiche_axes_graph (SDL_Renderer* ren, Graph* graph, SDL_Color color_axes){
    SDL_SetRenderDrawColor(ren, color_axes.r, color_axes.g, color_axes.b, color_axes.a);

    // Axe des abscisses
    int y_axis_pos = graph->origine_y + graph->y;
    if (graph->axe_y->max <= 0) {
        y_axis_pos = graph->origine_y;
    }
    else if (graph->axe_y->min <= 0 && graph->axe_y->max >= 0) {
        y_axis_pos = graph->origine_y + graph->y - (int)((0 - graph->axe_y->min) / graph->axe_y->echelle_grad) * graph->axe_y->taille_grad;
    }
    SDL_RenderDrawLine(ren, 0, y_axis_pos, FEN_X - TAILLE_BANDE_DROITE, y_axis_pos);

    // Axe des ordonnées
    int x_axis_pos = graph->origine_x;
    if (graph->axe_x->max <= 0) {
        x_axis_pos = graph->origine_x + graph->x;
    }
    else if (graph->axe_x->min <= 0 && graph->axe_x->max >= 0) {
        x_axis_pos = graph->origine_x + (int)((0 - graph->axe_x->min) / graph->axe_x->echelle_grad) * graph->axe_x->taille_grad;
    }
    SDL_RenderDrawLine(ren, x_axis_pos, TAILLE_BANDE_HAUT, x_axis_pos, FEN_Y);

    // Traits perpendiculaires et valeurs des graduations en x
    for (int i = 0; i <= graph->axe_x->nb_grad; i++) {
        if (i % graph->axe_x->skip_graduation == 0) {
            int x = graph->origine_x + i * graph->axe_x->taille_grad;
            if (x_axis_pos != x) {
                SDL_RenderDrawLine(ren, x, y_axis_pos - 5, x, y_axis_pos + 5);
                // Afficher les valeurs des graduations en x
                char label[10];
                snprintf(label, 10, "%.*f", graph->axe_x->precision, graph->axe_x->min + i * graph->axe_x->echelle_grad);
                renderText(ren, (const char*)label, x, y_axis_pos + 2*graph->axe_x->grad_text_size, color_axes, graph->axe_x->font_texte_grad);
            }
        }
    }

    for (int i = 0; i <= graph->axe_y->nb_grad; i++) {
        if (i % graph->axe_y->skip_graduation == 0) {
            int y = graph->origine_y + i * graph->axe_y->taille_grad;
            if (y_axis_pos != y) {
                SDL_RenderDrawLine(ren, x_axis_pos - 5, y, x_axis_pos + 5, y);
                // Afficher les valeurs des graduations en y
                char label[10];
                snprintf(label, 10, "%.*f", graph->axe_y->precision, graph->axe_y->max - i * graph->axe_y->echelle_grad);
                renderText(ren, (const char*)label, x_axis_pos - (graph->axe_y->precision + 1)*2*graph->axe_x->grad_text_size, y, color_axes, graph->axe_y->font_texte_grad);
            }
        }
    }
}

void change_color_mode (Colors* colors, int color_mode){
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
    colors->bande_haute_champs = (SDL_Color){200, 200, 200, 255};
    colors->bande_haute_description = (SDL_Color){150, 150, 150, 255};
    colors->bg_champ_entree = (SDL_Color){150, 150, 150, 255};
    colors->bg_champ_entree_hover = (SDL_Color){150, 150, 150, 150};
    colors->texte_champ_entree = (SDL_Color){0, 0, 0, 255};
    colors->texte_descriptifs_bande_haut = (SDL_Color){255, 255, 255, 255};
}

void affiche_interface (SDL_Renderer* ren, Graph* graph, Bande_entrees* bande_entrees, Colors* colors){
    SDL_SetRenderDrawColor(ren, colors->bg.r, colors->bg.g, colors->bg.b, colors->bg.a);
    SDL_RenderClear(ren);

    affiche_quadrillage(ren, graph);
    affiche_axes_graph(ren, graph, colors->axes);

    SDL_Rect bande_laterale_gauche = {FEN_X - TAILLE_BANDE_DROITE, TAILLE_BANDE_HAUT, TAILLE_BANDE_DROITE, FEN_Y - TAILLE_BANDE_HAUT};
    SDL_SetRenderDrawColor(ren, colors->bande_droite.r, colors->bande_droite.g, colors->bande_droite.b, colors->bande_droite.a);
    SDL_RenderFillRect(ren, &bande_laterale_gauche);

    affiche_bande_haut(ren, bande_entrees, bande_entrees->expressions[0], colors);

    tracer_fonction(ren, graph, bande_entrees->expressions[0]->fonction);
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

void tracer_fonction (SDL_Renderer* ren, Graph* graph, Fonction fonction){
    if (fonction.borne_sup > graph->axe_x->min && fonction.borne_inf < graph->axe_x->max){
        SDL_SetRenderDrawColor(ren, fonction.color.r, fonction.color.g, fonction.color.b, fonction.color.a);
        int nb_pts = graph->x;
        float borne_sup = (fonction.borne_sup > graph->axe_x->max) ? graph->axe_x->max : fonction.borne_sup;
        float borne_inf = (fonction.borne_inf < graph->axe_x->min) ? graph->axe_x->min : fonction.borne_inf;;
        float step_size = (borne_sup - borne_inf) / nb_pts;
        float x, y_sur_graph, x_sur_graph;
        for (int i = 0; i < nb_pts; i++) {
            x = borne_inf + i * step_size;
            if (f(x) >= graph->axe_y->min && f(x) <= graph->axe_y->max){
                y_sur_graph = graph->origine_y + (graph->axe_y->max - f(x)) / graph->axe_y->echelle_grad * graph->axe_y->taille_grad;
                x_sur_graph = graph->origine_x + (0-graph->axe_x->min + x) / graph->axe_x->echelle_grad * graph->axe_x->taille_grad;
                draw_thick_point(ren, x_sur_graph, y_sur_graph, 3);
            }
        }
    }
}

void Grapheur (SDL_Renderer* ren){
    Colors* colors = malloc(sizeof(Colors));
    change_color_mode(colors, 1);

    Bande_entrees* bande_entrees = malloc(sizeof(Bande_entrees));
    init_bande_entrees(bande_entrees, colors);

    Graph* graph = malloc(sizeof(Graph));
    *graph = init_graph(&bande_entrees->expressions[0]->fonction);


    SDL_StartTextInput();
    int is_event_backspace = 1;
    int running = 1;
    SDL_Event event;
    time_t current_time;

    while (running) {
        current_time = time(NULL);
        
        affiche_interface(ren, graph, bande_entrees, colors);
        
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) running = 0;

            if (event.type == SDL_MOUSEMOTION) {
                bande_entrees->x_souris_px = event.motion.x;
                bande_entrees->y_souris_px = event.motion.y;
            }

            is_event_backspace = handle_events_entrees_experssions(event, bande_entrees->expressions[0], bande_entrees->x_souris_px, bande_entrees->y_souris_px);

            if (event.type == SDL_KEYUP) {
                if (is_event_backspace && event.key.keysym.sym == SDLK_BACKSPACE){
                    ecran_acceuil(ren);
                    running = 0;
                }
            }
        }
        updateDisplay(ren);
    }
    free(graph);
    free(colors);
    SDL_StopTextInput();
}
