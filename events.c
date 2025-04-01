# include "events.h"


void handle_event_graph_MOUSEWHEEL(SDL_Event event, Graph* graph, int x_souris_px, int y_souris_px) {
    if (x_souris_px > graph->origine_x && x_souris_px < graph->origine_x + graph->x &&
            y_souris_px > graph->origine_y_apres_bande_haut && y_souris_px < graph->origine_y + graph->y){
        zoomer(event, graph, x_souris_px, y_souris_px);
    }
}

void handle_event_graph_MOUSEMOTION (SDL_Event event, Graph* graph, int x_souris_px, int y_souris_px) {
    if (graph->souris_pressee){
        if (x_souris_px > graph->origine_x && x_souris_px < graph->origine_x + graph->x &&
                y_souris_px > graph->origine_y_apres_bande_haut && y_souris_px < graph->origine_y + graph->y){
            graph->axe_x->min -= event.motion.xrel * graph->axe_x->echelle_grad / graph->axe_x->taille_grad;
            graph->axe_x->max -= event.motion.xrel * graph->axe_x->echelle_grad / graph->axe_x->taille_grad;
            graph->axe_y->min += event.motion.yrel * graph->axe_y->echelle_grad / graph->axe_y->taille_grad;
            graph->axe_y->max += event.motion.yrel * graph->axe_y->echelle_grad / graph->axe_y->taille_grad;
            resize_navigation(graph);
            for (int i=0; i<graph->nombre_evaluateur; i++) {
                graph->liste_evaluateurs[i].x_px += event.motion.xrel;
                graph->liste_evaluateurs[i].y_px += event.motion.yrel;
                graph->liste_evaluateurs[i].bouton_evaluateur.rect.x += event.motion.xrel;
                graph->liste_evaluateurs[i].bouton_evaluateur.rect.y += event.motion.yrel;
                graph->liste_evaluateurs[i].boutton_quitter.rect.x += event.motion.xrel;
                graph->liste_evaluateurs[i].boutton_quitter.rect.y += event.motion.yrel;
            }
        } else {
            graph->souris_pressee = false;
        }
    }
    for (int i=0; i<graph->nombre_evaluateur; i++){
        if (is_souris_sur_rectangle(graph->liste_evaluateurs[i].boutton_quitter.rect, x_souris_px, y_souris_px)){
            graph->liste_evaluateurs[i].boutton_quitter.hovered = 1;
        } else {
            graph->liste_evaluateurs[i].boutton_quitter.hovered = 0;
        }
    }
}

void handle_event_graph_MOUSEBUTTONUP (SDL_Renderer* ren, SDL_Event event, Graph* graph, int x_souris_px, int y_souris_px, Bande_haute* bande_haute) {
    if (x_souris_px > graph->origine_x && x_souris_px < graph->origine_x + graph->x &&
        y_souris_px > graph->origine_y_apres_bande_haut && y_souris_px < graph->origine_y + graph->y){
            if (event.button.button == SDL_BUTTON_RIGHT){
                handle_event_graph_MOUSEBUTTONUP_RIGHT(event, graph, x_souris_px, y_souris_px);
            }
            if (event.button.button == SDL_BUTTON_LEFT){
                handle_event_graph_MOUSEBUTTONUP_LEFT(ren, event, graph, x_souris_px, y_souris_px, bande_haute);
            }
    }
}

void handle_event_graph_MOUSEBUTTONUP_RIGHT (SDL_Event event, Graph* graph, int x_souris_px, int y_souris_px) {
    graph->souris_pressee = false;
}

void handle_event_graph_MOUSEBUTTONUP_LEFT (SDL_Renderer* ren, SDL_Event event, Graph* graph, int x_souris_px, int y_souris_px, Bande_haute* bande_haute) {
    bool clic_utile = false;
    for (int i=0; i<graph->nombre_evaluateur; i++){
        if (is_souris_sur_rectangle(graph->liste_evaluateurs[i].boutton_quitter.rect, x_souris_px, y_souris_px)){
            suppr_evaluateur_x(graph, i);
            clic_utile = true;
            break;
        }
    }
    if (!clic_utile){
        ajout_evaluateur_x (ren, event,  graph,  x_souris_px,  y_souris_px,  bande_haute);
    }
    // TODO ajouter un mode au graph pour changer l'evaluateur x y et les autres modes
}

void handle_event_graph_MOUSEBUTTONDOWN (SDL_Event event, Graph* graph, int x_souris_px, int y_souris_px) {
    if (x_souris_px > graph->origine_x && x_souris_px < graph->origine_x + graph->x &&
            y_souris_px > graph->origine_y_apres_bande_haut && y_souris_px < graph->origine_y + graph->y){
        graph->souris_pressee = true;
    }
}




void handle_event_bande_haut_TEXTINPUT (SDL_Event event, Bande_haute* bande_haute){
    for (int i = 0; i < bande_haute->nb_expressions; i++) {
        if (bande_haute->expressions[i]->entree_selectionnee != SELECTION_NULL){
            if (!bande_haute->expressions[i]->visible){
                bande_haute->scroll_offset = bande_haute->expressions[i]->rect_initial.y - bande_haute->surface.y;
                action_apres_modif_offset(bande_haute);
            }
            handle_event_entrees_expressions_TEXTINPUT(event, bande_haute->expressions[i]);
        }
    }
}

bool handle_event_bande_haut_KEYDOWN (SDL_Event event, Bande_haute* bande_haute){
    bool backspace_gere = 0; // Pour faire l'union des réponses sur le backspace est-il géré par les 
    for (int i = 0; i < bande_haute->nb_expressions && backspace_gere == 0; i++) {
        if (bande_haute->expressions[i]->entree_selectionnee != SELECTION_NULL){
            if (!bande_haute->expressions[i]->visible){
                bande_haute->scroll_offset = bande_haute->expressions[i]->rect_initial.y - bande_haute->surface.y;
                action_apres_modif_offset(bande_haute);
            }
            handle_event_entrees_expressions_KEYDOWN(event, bande_haute->expressions[i]);
            backspace_gere += bande_haute->expressions[i]->entree_selectionnee != SELECTION_NULL;
        }
    }
    return backspace_gere;
}

void handle_event_bande_haut_KEYUP (SDL_Event event, Bande_haute* bande_haute){
    for (int i = 0; i < bande_haute->nb_expressions; i++) {
        if (bande_haute->expressions[i]->visible) {
            handle_event_entrees_expressions_KEYUP(event, bande_haute->expressions[i]);
        }
    }
}

bool handle_event_bande_haut_MOUSEMOTION (SDL_Event event, Bande_haute* bande_haute, int x_souris_px, int y_souris_px){
    bool is_MOUSEMOTION_used = false;
    for (int i = 0; i < bande_haute->nb_expressions; i++) {
        if (!is_MOUSEMOTION_used && bande_haute->expressions[i]->visible) {
            is_MOUSEMOTION_used = handle_event_color_picker_SDL_MOUSEMOTION(event, bande_haute->expressions[i]->color_picker, x_souris_px, y_souris_px);
            handle_event_entrees_expressions_MOUSEMOTION(event, bande_haute, bande_haute->expressions[i], x_souris_px, y_souris_px, is_MOUSEMOTION_used);
        }
    }
    if (!is_MOUSEMOTION_used && is_souris_sur_rectangle(bande_haute->button_new_expression.bt.rect, x_souris_px, y_souris_px)) {
        bande_haute->button_new_expression.bt.hovered = 1;
    } else bande_haute->button_new_expression.bt.hovered = 0;
    return is_MOUSEMOTION_used;
}

bool handle_event_bande_haut_MOUSEBUTTONUP (SDL_Renderer* ren, SDL_Event event, Bande_haute* bande_haute, int x_souris_px, int y_souris_px){
    bool clic_utile = false;
    for (int i = 0; i < bande_haute->nb_expressions; i++) {
        if (!clic_utile && bande_haute->expressions[i]->visible) {
            clic_utile = handle_event_entrees_expressions_MOUSEBUTTONUP(event, bande_haute, bande_haute->expressions[i], x_souris_px, y_souris_px);
        }
    }
    if (!clic_utile && is_souris_sur_rectangle(bande_haute->button_new_expression.bt.rect, x_souris_px, y_souris_px)) {
        ajout_bande_expression(ren, bande_haute);
        clic_utile = true;
    }
    if (!clic_utile && is_souris_sur_rectangle(bande_haute->surface, x_souris_px, y_souris_px)) {
        bande_haute->expanding = !bande_haute->expanding;
        clic_utile = true;
    }
    return clic_utile;
}

bool handle_event_bande_haut_MOUSEBUTTONDOWN (SDL_Event event, Bande_haute* bande_haute, int x_souris_px, int y_souris_px){
    bool is_MOUSEBUTTONDOWN_used = false;
    for (int i = 0; i < bande_haute->nb_expressions; i++) {
        if (!is_MOUSEBUTTONDOWN_used && bande_haute->expressions[i]->visible) {
            is_MOUSEBUTTONDOWN_used = handle_event_color_picker_MOUSEBUTTONDOWN(bande_haute->expressions[i]->color_picker, x_souris_px, y_souris_px);
        }
    }
    return is_MOUSEBUTTONDOWN_used;
}

void handle_event_bande_haut_MOUSEWHEEL (SDL_Event event, Bande_haute* bande_haute, int x_souris_px, int y_souris_px){
    if (is_souris_sur_rectangle(bande_haute->surface, x_souris_px, y_souris_px)){
        if (bande_haute->expanding) {
            bande_haute->scroll_offset -= event.wheel.y * BANDE_EXPRESSIONS_ON_SCROLL_STEP;
        }
        if (!bande_haute->expanding) {
            bande_haute->scroll_offset -= event.wheel.y * BANDE_EXPRESSIONS_OFF_SCROLL_STEP;
        }
        action_apres_modif_offset(bande_haute);
    }
}


void handle_event_entrees_expressions_MOUSEMOTION (SDL_Event event, Bande_haute* bande_haute, Expression_fonction* expression, int x_souris_px, int y_souris_px, int is_MOUSEMOTION_used) {
    for (int i = 0; i < NB_ENTREES; i++) {
        expression->champs_entrees[i]->champs_texte->hovered = 0;
    }
    expression->color_picker->hovered = 0;
    expression->button_deplacement.bt.hovered = 0;
    expression->button_visibilite.bt.hovered = 0;
    expression->button_delete.bt.hovered = 0;
    expression->button_delete.bt.pourcentage_place = 70;
    if (y_souris_px < bande_haute->surface.y + bande_haute->surface.h - TAILLE_BARRE_BASSE_DE_BANDE_HAUT &&
            !is_MOUSEMOTION_used && is_souris_sur_rectangle(bande_haute->surface, x_souris_px, y_souris_px)){
        // Souris sur un bouton ?
        for (int i = 0; i < NB_ENTREES; i++) {
            if (is_souris_sur_rectangle(expression->champs_entrees[i]->champs_texte->rect, x_souris_px, y_souris_px)) {
                expression->champs_entrees[i]->champs_texte->hovered = 1;
            }
        }
        if (abs(expression->color_picker->boutton_x - x_souris_px) <= expression->color_picker->boutton_taille/2 && abs(expression->color_picker->boutton_y_affiche - y_souris_px) <= expression->color_picker->boutton_taille/2) {
            expression->color_picker->hovered = 1;
        }
        if (is_souris_sur_rectangle(expression->button_deplacement.bt.rect, x_souris_px, y_souris_px)) {
            expression->button_deplacement.bt.hovered = 1;
        }
        if (is_souris_sur_rectangle(expression->button_visibilite.bt.rect, x_souris_px, y_souris_px)) {
            expression->button_visibilite.bt.hovered = 1;
        }
        if (is_souris_sur_rectangle(expression->button_delete.bt.rect, x_souris_px, y_souris_px)) {
            expression->button_delete.bt.hovered = 1;
            expression->button_delete.bt.pourcentage_place = 50;
        }
    }
}

bool handle_event_entrees_expressions_MOUSEBUTTONUP (SDL_Event event, Bande_haute* bande_haute, Expression_fonction* expression, int x_souris_px, int y_souris_px) {
    // Vérifier si on clique sur un champ
    bool clic_utile = false;
    int au_moins_un_champs_selectionne = 0;
    clic_utile = handle_event_color_picker_MOUSEBUTTONUP(expression->color_picker, x_souris_px, y_souris_px);
    if (!clic_utile && y_souris_px < bande_haute->surface.y + bande_haute->surface.h - TAILLE_BARRE_BASSE_DE_BANDE_HAUT){
        for (int i = 0; i < NB_ENTREES && !clic_utile; i++) {
            if (is_souris_sur_rectangle(expression->champs_entrees[i]->champs_texte->rect, x_souris_px, y_souris_px)) {
                expression->entree_selectionnee = expression->champs_entrees[i]->type_entree;
                expression->champs_entrees[i]->cursorVisible = 1;
                expression->champs_entrees[i]->lastCursorToggle = SDL_GetTicks();
                au_moins_un_champs_selectionne = 1;
                clic_utile = true;
            }
        }
        if (abs(expression->color_picker->boutton_x - x_souris_px) <= expression->color_picker->boutton_taille/2 && abs(expression->color_picker->boutton_y_affiche - y_souris_px) <= expression->color_picker->boutton_taille/2) {
            expression->color_picker->show_picker = !expression->color_picker->show_picker;
            clic_utile = true;
            if (expression->color_picker->show_picker){
                expression->color_picker->rect_moving = expression->color_picker->rect_initial;
                expression->color_picker->boutton_quitter.rect = expression->color_picker->boutton_quitter_rect_base;
            }
        }
        if (is_souris_sur_rectangle(expression->button_deplacement.bt.rect, x_souris_px, y_souris_px)) {
            // TODO
            clic_utile = true;
        }
        if (is_souris_sur_rectangle(expression->button_visibilite.bt.rect, x_souris_px, y_souris_px)) {
            expression->fonction.visible = !expression->fonction.visible;
            clic_utile = true;
            if (expression->fonction.visible){
                expression->button_visibilite.bt.image = expression->image_button_visible;
            } else expression->button_visibilite.bt.image = expression->image_button_invisible;
        }
        if (is_souris_sur_rectangle(expression->button_delete.bt.rect, x_souris_px, y_souris_px)) {
            suppr_bande_expression(bande_haute, expression->numero);
            clic_utile = true;
        }
    }
    if (!au_moins_un_champs_selectionne) { // Si clic à côté, on regarde si on quitte un champs et si c'est le cas, on charge la valeur de ce champs. PS : l' entree_selectionnee sera également remise à SELECTION_NULL
        execute_champs_select_and_change_focus(expression, SELECTION_NULL);
    }
    return clic_utile;
}

void handle_event_entrees_expressions_TEXTINPUT (SDL_Event event, Expression_fonction* expression) {
    Entree_texte *target = expression->champs_entrees[expression->entree_selectionnee];
    if (expression->entree_selectionnee < 2) {
        if (strlen(target->text) < MAX_LEN_ENTREES_BORNES && (isdigit(event.text.text[0]) || event.text.text[0] == '-' || event.text.text[0] == '.'|| event.text.text[0] == 'E' || event.text.text[0] == 'e')) {
            insert_char(target->text, target->position_cursor++, event.text.text[0]);
        }
    } else
    if (strlen(target->text) < MAX_LEN_STR) {
        insert_char(target->text, target->position_cursor++, event.text.text[0]);
    }
}

void handle_event_entrees_expressions_KEYDOWN (SDL_Event event, Expression_fonction* expression) {
    if (event.key.keysym.sym == SDLK_BACKSPACE) {
        Entree_texte *target = expression->champs_entrees[expression->entree_selectionnee];
        if (strlen(target->text) > 0 && target->position_cursor > 0) {
            remove_char(target->text, --target->position_cursor);
        }
    }

    if (event.key.keysym.sym == SDLK_DOWN || event.key.keysym.sym == SDLK_UP) {
        if (event.key.keysym.sym == SDLK_DOWN) {
            execute_champs_select_and_change_focus(expression, (expression->entree_selectionnee - 1 + NB_ENTREES) % NB_ENTREES);
        } else {
            execute_champs_select_and_change_focus(expression, (expression->entree_selectionnee + 1) % NB_ENTREES);
        }
        for (int i = 0; i < NB_ENTREES; i++) {
            expression->champs_entrees[i]->cursorVisible = 0;
        }
        expression->champs_entrees[expression->entree_selectionnee]->cursorVisible = 1;
        expression->champs_entrees[expression->entree_selectionnee]->lastCursorToggle = SDL_GetTicks();
    }

    if (event.key.keysym.sym == SDLK_RIGHT){
        Entree_texte *target = expression->champs_entrees[expression->entree_selectionnee];
        if (target->position_cursor < strlen(target->text)) {
            target->position_cursor++;
        }
    }
    if (event.key.keysym.sym == SDLK_LEFT){
        Entree_texte *target = expression->champs_entrees[expression->entree_selectionnee];
        if (target->position_cursor > 0) {
            target->position_cursor--;
        }
    }
}

void handle_event_entrees_expressions_KEYUP (SDL_Event event, Expression_fonction* expression) {
    if (event.key.keysym.sym == SDLK_RETURN || event.key.keysym.sym == SDLK_KP_ENTER) {
        execute_champs_select_and_change_focus(expression, SELECTION_NULL);
    }
}



void handle_event_bande_droite_MOUSEMOTION (SDL_Event event, Bande_droite* bande_droite, int x_souris_px, int y_souris_px){
    if (is_souris_sur_rectangle(bande_droite->bouton_retour.rect, x_souris_px, y_souris_px)) {
        bande_droite->bouton_retour.hovered = 1;
        bande_droite->bouton_retour.pourcentage_place = 50;
    } else {
        bande_droite->bouton_retour.hovered = 0;
        bande_droite->bouton_retour.pourcentage_place = 80;
    }

    if (is_souris_sur_rectangle(bande_droite->bouton_centrer.rect, x_souris_px, y_souris_px)) {
        bande_droite->bouton_centrer.hovered = 1;
    } else bande_droite->bouton_centrer.hovered = 0;
}

int handle_event_bande_droite_MOUSEBUTTONUP (SDL_Renderer* ren, SDL_Event event, Bande_droite* bande_droite, Bande_haute* bande_haute, Graph* graph, int x_souris_px, int y_souris_px){
    int clic_utile = 0;
    if (!clic_utile && is_souris_sur_rectangle(bande_droite->bouton_retour.rect, x_souris_px, y_souris_px)) {
        return -1; // Retourner à l'accueil
    }
    if (!clic_utile && is_souris_sur_rectangle(bande_droite->bouton_centrer.rect, x_souris_px, y_souris_px)) {
        resize_recentrer(graph, &bande_haute->expressions[0]->fonction);
        clic_utile = 1;
    }
    return clic_utile;
}


void resize_fen_2D (Bande_haute* bande_haute, Bande_droite* bande_droite, Graph* graph){
    bande_haute->surface.w = FEN_X - TAILLE_BANDE_DROITE;
    int x = graph->x;
    int y = graph->y;
    resize_contours_graph(graph);
    graph->axe_x->taille_grad *= graph->x / x;
    graph->axe_y->taille_grad *= graph->y / y;
    resize_navigation(graph);
    resize_bande_haut(bande_haute);
    resize_bande_droite(bande_droite);
}

int handle_all_events (SDL_Renderer* ren, Bande_haute* bande_haute, Bande_droite* bande_droite, Graph* graph, int* x_souris_px, int* y_souris_px, bool* is_event_backspace_used){
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) return 1; // On quitte le grapheur

        if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_RESIZED) {
            FEN_X = event.window.data1;
            FEN_Y = event.window.data2;
            resize_fen_2D(bande_haute, bande_droite, graph);
        }

        if (event.type == SDL_MOUSEMOTION) {
            *x_souris_px = event.motion.x;
            *y_souris_px = event.motion.y;
            bool is_MOUSEMOTION_used = false;
            is_MOUSEMOTION_used = handle_event_bande_haut_MOUSEMOTION (event, bande_haute, *x_souris_px, *y_souris_px);
            if (!is_MOUSEMOTION_used) {
                handle_event_bande_droite_MOUSEMOTION(event, bande_droite, *x_souris_px, *y_souris_px);
                handle_event_graph_MOUSEMOTION (event, graph, *x_souris_px, *y_souris_px);
            }
        }

        if (event.type == SDL_MOUSEWHEEL) {
            handle_event_bande_haut_MOUSEWHEEL (event, bande_haute, *x_souris_px, *y_souris_px);
            handle_event_graph_MOUSEWHEEL (event, graph, *x_souris_px, *y_souris_px);
        }

        if (event.type == SDL_MOUSEBUTTONUP) {
            int is_MOUSEBUTTONUP_used = 0;
            is_MOUSEBUTTONUP_used = handle_event_bande_haut_MOUSEBUTTONUP (ren, event, bande_haute, *x_souris_px, *y_souris_px);
            if (!is_MOUSEBUTTONUP_used){
                is_MOUSEBUTTONUP_used = handle_event_bande_droite_MOUSEBUTTONUP(ren, event, bande_droite, bande_haute, graph, *x_souris_px, *y_souris_px);
                if (is_MOUSEBUTTONUP_used == -1) return 2; // Retourner à l'accueil
                else if (!is_MOUSEBUTTONUP_used){
                    handle_event_graph_MOUSEBUTTONUP (ren, event, graph, *x_souris_px, *y_souris_px, bande_haute);
                }
            }
        }

        if (event.type == SDL_MOUSEBUTTONDOWN) {
            bool is_MOUSEBUTTONDOWN_used = false;
            is_MOUSEBUTTONDOWN_used = handle_event_bande_haut_MOUSEBUTTONDOWN (event, bande_haute, *x_souris_px, *y_souris_px);
            if (!is_MOUSEBUTTONDOWN_used && event.button.button == SDL_BUTTON_RIGHT){
                handle_event_graph_MOUSEBUTTONDOWN (event, graph, *x_souris_px, *y_souris_px);
            }
        }

        if (event.type == SDL_TEXTINPUT) {
            handle_event_bande_haut_TEXTINPUT (event, bande_haute);
        }

        if (event.type == SDL_KEYDOWN) {
            *is_event_backspace_used = handle_event_bande_haut_KEYDOWN (event, bande_haute);
        }

        if (event.type == SDL_KEYUP) {
            handle_event_bande_haut_KEYUP (event, bande_haute);

            if (!*is_event_backspace_used && event.key.keysym.sym == SDLK_BACKSPACE){
                return 2; // On revient à l'écran d'accueil
            }

            if (event.key.keysym.sym == SDLK_c){
                resize_recentrer(graph, &bande_haute->expressions[0]->fonction);
            }
        }
    }
    return 0; // On ne quitte pas le grapheur
}
