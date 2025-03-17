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
        } else {
            graph->souris_pressee = false;
        }
    }
}

void handle_event_graph_MOUSEBUTTONUP (SDL_Event event, Graph* graph, int x_souris_px, int y_souris_px) {
    graph->souris_pressee = false;
}

void handle_event_graph_MOUSEBUTTONDOWN (SDL_Event event, Graph* graph, int x_souris_px, int y_souris_px) {
    if (x_souris_px > graph->origine_x && x_souris_px < graph->origine_x + graph->x &&
            y_souris_px > graph->origine_y_apres_bande_haut && y_souris_px < graph->origine_y + graph->y){
        graph->souris_pressee = true;
    }
}




void handle_event_bande_haut_TEXTINPUT (SDL_Event event, Bande_entrees* bande_entrees){
    for (int i = 0; i < bande_entrees->nb_expressions; i++) {
        if (bande_entrees->expressions[i]->entree_selectionnee != SELECTION_NULL){
            if (!bande_entrees->expressions[i]->visible){
                bande_entrees->scroll_offset = bande_entrees->expressions[i]->rect_initial.y - bande_entrees->surface.y;
                action_apres_modif_offset(bande_entrees);
            }
            handle_event_entrees_expressions_TEXTINPUT(event, bande_entrees->expressions[i]);
        }
    }
}

bool handle_event_bande_haut_KEYDOWN (SDL_Event event, Bande_entrees* bande_entrees){
    bool backspace_gere = 0; // Pour faire l'union des réponses sur le backspace est-il géré par les 
    for (int i = 0; i < bande_entrees->nb_expressions && backspace_gere == 0; i++) {
        if (bande_entrees->expressions[i]->entree_selectionnee != SELECTION_NULL){
            if (!bande_entrees->expressions[i]->visible){
                bande_entrees->scroll_offset = bande_entrees->expressions[i]->rect_initial.y - bande_entrees->surface.y;
                action_apres_modif_offset(bande_entrees);
            }
            handle_event_entrees_expressions_KEYDOWN(event, bande_entrees->expressions[i]);
            backspace_gere += bande_entrees->expressions[i]->entree_selectionnee != SELECTION_NULL;
        }
    }
    return backspace_gere;
}

void handle_event_bande_haut_KEYUP (SDL_Event event, Bande_entrees* bande_entrees){
    for (int i = 0; i < bande_entrees->nb_expressions; i++) {
        if (bande_entrees->expressions[i]->visible) {
            handle_event_entrees_expressions_KEYUP(event, bande_entrees->expressions[i]);
        }
    }
}

void handle_event_bande_haut_MOUSEMOTION (SDL_Event event, Bande_entrees* bande_entrees, int x_souris_px, int y_souris_px){
    if (is_souris_sur_rectangle(bande_entrees->surface, x_souris_px, y_souris_px)){
        for (int i = 0; i < bande_entrees->nb_expressions; i++) {
            if (bande_entrees->expressions[i]->visible) {
                handle_event_entrees_expressions_MOUSEMOTION(event, bande_entrees->expressions[i], x_souris_px, y_souris_px);
            }
        }
    }
}

void handle_event_bande_haut_MOUSEBUTTONUP (SDL_Event event, Bande_entrees* bande_entrees, int x_souris_px, int y_souris_px){
    bool clic_utile = false;
    for (int i = 0; i < bande_entrees->nb_expressions; i++) {
        if (bande_entrees->expressions[i]->visible) {
            clic_utile += handle_event_entrees_expressions_MOUSEBUTTONUP(event, bande_entrees->expressions[i], x_souris_px, y_souris_px);
        }
    }
    if (!clic_utile && is_souris_sur_rectangle(bande_entrees->surface, x_souris_px, y_souris_px)) {
        bande_entrees->expanding = !bande_entrees->expanding;
    }
}

void handle_event_bande_haut_MOUSEWHEEL (SDL_Event event, Bande_entrees* bande_entrees, int x_souris_px, int y_souris_px){
    if (is_souris_sur_rectangle(bande_entrees->surface, x_souris_px, y_souris_px)){
        if (bande_entrees->expanding) {
            bande_entrees->scroll_offset -= event.wheel.y * BANDE_EXPRESSIONS_ON_SCROLL_STEP;
        }
        if (!bande_entrees->expanding) {
            bande_entrees->scroll_offset -= event.wheel.y * BANDE_EXPRESSIONS_OFF_SCROLL_STEP;
        }
        action_apres_modif_offset(bande_entrees);
    }
}
void action_apres_modif_offset (Bande_entrees* bande_entrees){
    if (bande_entrees->scroll_offset < 0) bande_entrees->scroll_offset = 0;
    int max_offset = (bande_entrees->nb_expressions - 1) * bande_entrees->params.height_bande_expression;
    if (bande_entrees->scroll_offset > max_offset) bande_entrees->scroll_offset = max_offset;
    for (int i = 0; i < bande_entrees->nb_expressions; i++) {
        placement_pour_affichage_avec_offset(bande_entrees->expressions[i], bande_entrees->scroll_offset);
        bande_entrees->expressions[i]->visible = (bande_entrees->expressions[i]->rect_affiche.y + bande_entrees->expressions[i]->rect_affiche.h > bande_entrees->surface.y &&
            bande_entrees->expressions[i]->rect_affiche.y < bande_entrees->surface.y + bande_entrees->surface.h);
    }
}


void handle_event_entrees_expressions_MOUSEMOTION (SDL_Event event, Expression_fonction* expression, int x_souris_px, int y_souris_px) {
    // Souris sur un bouton ?
    for (int i = 0; i < NB_ENTREES; i++) {
        if (is_souris_sur_rectangle(expression->champs_entrees[i]->champs_texte->rect, x_souris_px, y_souris_px)) {
            expression->champs_entrees[i]->champs_texte->hovered = 1;
        } else {
            expression->champs_entrees[i]->champs_texte->hovered = 0;
        }
    }
}

bool handle_event_entrees_expressions_MOUSEBUTTONUP (SDL_Event event, Expression_fonction* expression, int x_souris_px, int y_souris_px) {
    // Vérifier si on clique sur un champ
    bool clic_utile = false;
    int au_moins_un_champs_selectionne = 0;
    for (int i = 0; i < NB_ENTREES; i++) {
        if (is_souris_sur_rectangle(expression->champs_entrees[i]->champs_texte->rect, x_souris_px, y_souris_px)) {
            expression->entree_selectionnee = expression->champs_entrees[i]->type_entree;
            expression->champs_entrees[i]->cursorVisible = 1;
            expression->champs_entrees[i]->lastCursorToggle = SDL_GetTicks();
            au_moins_un_champs_selectionne = 1;
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

