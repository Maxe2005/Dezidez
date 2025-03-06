#include "grapheur.h"

float f (float x) {
    return exp(x);
}

void resize_graph (Graph* graph){
    if (graph->nb_grad_x < 1) graph->nb_grad_x = 1;
    if (graph->nb_grad_y < 1) graph->nb_grad_y = 1;
    if (graph->min_x >= graph->max_x) graph->max_x = graph->min_x + 1;
    if (graph->min_y == graph->max_y) graph->max_y = graph->min_y + 1;

    int marge = 0; 
    if (FEN_X - TAILLE_BANDE_DROITE < FEN_Y - TAILLE_BANDE_HAUT) {
        marge = (int)((FEN_X - TAILLE_BANDE_DROITE) / 15);
    } else {
        marge = (int)((FEN_Y - TAILLE_BANDE_HAUT) / 15);
    }
    graph->origine_x = marge;
    graph->origine_y = TAILLE_BANDE_HAUT + marge;
    graph->x = FEN_X-TAILLE_BANDE_DROITE - 2*marge;
    graph->y = FEN_Y - 2*marge - TAILLE_BANDE_HAUT;
    graph->taille_grad_x = graph->x / graph->nb_grad_x;
    graph->taille_grad_y = graph->y / graph->nb_grad_y;
    graph->echelle_grad_x = (graph->max_x - graph->min_x) / graph->nb_grad_x;
    graph->echelle_grad_y = (graph->max_y - graph->min_y) / graph->nb_grad_y;
}

void affiche_quadrillage (SDL_Renderer* ren, Graph* graph){
    SDL_SetRenderDrawColor(ren, 50, 50, 50, 255);
    for (int i = 0; i<=graph->nb_grad_x; i++) {
        SDL_RenderDrawLine(ren, graph->origine_x + graph->taille_grad_x*i, graph->origine_y, graph->origine_x + graph->taille_grad_x*i, graph->origine_y + graph->nb_grad_y*graph->taille_grad_y);
    }
    for (int i = 0; i<=graph->nb_grad_y; i++) {
        SDL_RenderDrawLine(ren, graph->origine_x, graph->origine_y + graph->taille_grad_y*i, graph->origine_x + graph->taille_grad_x*graph->nb_grad_x, graph->origine_y + graph->taille_grad_y*i);
    }
}

Graph init_graph (){
    Graph graph;
    graph.nb_grad_x = 10;
    graph.nb_grad_y = 10;
    graph.min_x = -5;
    graph.max_x = 5;
    graph.min_y = -5;
    graph.max_y = 5;
    resize_graph(&graph);
    graph.font_texte_grad = NULL;
    graph.grad_text_size = 0;
    return graph;
}

void affiche_axes_graph (SDL_Renderer* ren, Graph* graph, SDL_Color color_axes){
    SDL_SetRenderDrawColor(ren, color_axes.r, color_axes.g, color_axes.b, color_axes.a);

    // Axe des abscisses
    int y_axis_pos = graph->origine_y + graph->y;
    if (graph->max_y <= 0) {
        y_axis_pos = graph->origine_y;
    }
    else if (graph->min_y <= 0 && graph->max_y >= 0) {
        y_axis_pos = graph->origine_y + graph->y - (int)((0 - graph->min_y) / graph->echelle_grad_y) * graph->taille_grad_y;
    }
    SDL_RenderDrawLine(ren, graph->origine_x, y_axis_pos, graph->origine_x + graph->x, y_axis_pos);

    // Axe des ordonnées
    int x_axis_pos = graph->origine_x;
    if (graph->max_x <= 0) {
        x_axis_pos = graph->origine_x + graph->x;
    }
    else if (graph->min_x <= 0 && graph->max_x >= 0) {
        x_axis_pos = graph->origine_x + (int)((0 - graph->min_x) / graph->echelle_grad_x) * graph->taille_grad_x;
    }
    SDL_RenderDrawLine(ren, x_axis_pos, graph->origine_y, x_axis_pos, graph->origine_y + graph->y);

    // Déterminer la taille du texte en fonction de la taille du graphique
    int text_size = (graph->taille_grad_x < 20 || graph->taille_grad_y < 20) ? 10 : 12;
    if (graph->grad_text_size != text_size) {
        graph->grad_text_size = text_size;
        if (graph->font_texte_grad != NULL) TTF_CloseFont(graph->font_texte_grad);
        graph->font_texte_grad = createFont("Ressources/DejaVuSans-Bold.ttf", text_size);
    }
    int skip_graduation_x = (graph->taille_grad_x < 20) ? 2 : 1;
    int skip_graduation_y = (graph->taille_grad_y < 20) ? 2 : 1;

    // Déterminer la précision des valeurs des graduations
    int precision_x = 0;
    int precision_y = 0;
    for (int i = 2; i >= 0; i--) {
        if (graph->echelle_grad_x < pow(10, 0-i)) {
            precision_x = i+1;
            break;
        }
    }
    for (int i = 2; i > 0; i--) {
        if (graph->echelle_grad_y < pow(10, 0-i)) {
            precision_y = i+1;
            break;
        }
    }

    // Traits perpendiculaires et valeurs des graduations en x
    for (int i = 0; i <= graph->nb_grad_x; i++) {
        if (i % skip_graduation_x == 0) {
            int x = graph->origine_x + i * graph->taille_grad_x;
            if (x_axis_pos != x) {
                SDL_RenderDrawLine(ren, x, y_axis_pos - 5, x, y_axis_pos + 5);
                // Afficher les valeurs des graduations en x
                char label[10];
                snprintf(label, 10, "%.*f", precision_x, graph->min_x + i * graph->echelle_grad_x);
                renderText(ren, (const char*)label, x, y_axis_pos + 2*text_size, color_axes, graph->font_texte_grad);
            }
        }
    }
    // Traits perpendiculaires et valeurs des graduations en y
    for (int i = 0; i <= graph->nb_grad_y; i++) {
        if (i % skip_graduation_y == 0) {
            int y = graph->origine_y + i * graph->taille_grad_y;
            if (y_axis_pos != y) {
                SDL_RenderDrawLine(ren, x_axis_pos - 5, y, x_axis_pos + 5, y);
                // Afficher les valeurs des graduations en y
                char label[10];
                snprintf(label, 10, "%.*f", precision_y, graph->max_y - i * graph->echelle_grad_y);
                renderText(ren, (const char*)label, x_axis_pos - (precision_y + 1)*text_size*2, y, color_axes, graph->font_texte_grad);
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

    SDL_Rect bande_laterale_gauche = {FEN_X - TAILLE_BANDE_DROITE, TAILLE_BANDE_HAUT, FEN_X, FEN_Y};
    SDL_SetRenderDrawColor(ren, colors->bande_droite.r, colors->bande_droite.g, colors->bande_droite.b, colors->bande_droite.a);
    SDL_RenderFillRect(ren, &bande_laterale_gauche);

    affiche_bande_haut(ren, bande_entrees, colors);
}

void Grapheur (SDL_Renderer* ren){
    Colors* colors = malloc(sizeof(Colors));
    change_color_mode(colors, 1);

    Graph* graph = malloc(sizeof(Graph));
    *graph = init_graph();

    Bande_entrees* bande_entrees = malloc(sizeof(Bande_entrees));
    init_bande_entrees(bande_entrees, colors);


    SDL_StartTextInput();
    int running = 1;
    SDL_Event event;
    time_t current_time;

    while (running) {
        current_time = time(NULL);
        
        affiche_interface(ren, graph, bande_entrees, colors);
        
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) running = 0;

            if (event.type == SDL_KEYUP) {
                if (event.key.keysym.sym == SDLK_BACKSPACE){
                    ecran_acceuil(ren);
                    running = 0;
                }
            }
        }
        updateDisplay(ren);
    }
    free(graph);
    SDL_StopTextInput();
}
