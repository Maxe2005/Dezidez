#include "grapheur.h"

float f (float x) {
    return exp(x);
}
/*
void affiche_quadrillage (SDL_Renderer* ren, Map* map){
    SDL_SetRenderDrawColor(ren, 255, 255, 255, 255);
    for (int i = 0; i<=map->x; i++) {
        SDL_RenderDrawLine(ren, ORIGINE_X + map->taille_case*i, ORIGINE_Y, ORIGINE_X + map->taille_case*i, ORIGINE_Y + map->taille_case*map->y);
    }
    for (int i = 0; i<=map->y; i++) {
        SDL_RenderDrawLine(ren, ORIGINE_X, ORIGINE_Y + map->taille_case*i, ORIGINE_X + map->taille_case*map->x, ORIGINE_Y + map->taille_case*i);
    }
}


void handle_events_modif_taille_map(SDL_Event event, Session_modif_map* session, Musique* musique) {
    if (event.type == SDL_MOUSEMOTION) {
        session->x_souris_px = event.motion.x;
        session->y_souris_px = event.motion.y;

        // Souris sur un bouton ?
        if (is_souris_sur_button(*session->sous_menu_modif_taille_map->annuler, session->x_souris_px, session->y_souris_px)) {
            session->sous_menu_modif_taille_map->annuler->hovered = 1;
        } else {
            session->sous_menu_modif_taille_map->annuler->hovered = 0;
        }
        if (is_souris_sur_button(*session->sous_menu_modif_taille_map->valider, session->x_souris_px, session->y_souris_px)) {
            session->sous_menu_modif_taille_map->valider->hovered = 1;
        } else {
            session->sous_menu_modif_taille_map->valider->hovered = 0;
        }
    }

    if (event.type == SDL_MOUSEBUTTONUP) {
        // Vérifier si on clique sur un champ
        if (is_souris_sur_button(*session->sous_menu_modif_taille_map->entree_width, session->x_souris_px, session->y_souris_px)) {
            session->sous_menu_modif_taille_map->is_typing_width = 1;
            session->sous_menu_modif_taille_map->cursorVisible = 1;
            session->sous_menu_modif_taille_map->lastCursorToggle = SDL_GetTicks();
        }
        if (is_souris_sur_button(*session->sous_menu_modif_taille_map->entree_height, session->x_souris_px, session->y_souris_px)) {
            session->sous_menu_modif_taille_map->is_typing_width = 0;
            session->sous_menu_modif_taille_map->cursorVisible = 1;
            session->sous_menu_modif_taille_map->lastCursorToggle = SDL_GetTicks();
        }

        // Vérifier si on clique sur "Valider"
        if (is_souris_sur_button(*session->sous_menu_modif_taille_map->valider, session->x_souris_px, session->y_souris_px)) {
            valider_modif_taille_map(session, musique);
        }

        // Vérifier si on clique sur "Annuler"
        if (is_souris_sur_button(*session->sous_menu_modif_taille_map->annuler, session->x_souris_px, session->y_souris_px)) {
            playSoundEffect(musique->select);
            session->is_modif_taille_map = 0;
            SDL_StopTextInput();
        }
    }

    if (event.type == SDL_TEXTINPUT) {
        char *target = session->sous_menu_modif_taille_map->is_typing_width ? session->sous_menu_modif_taille_map->width_text : session->sous_menu_modif_taille_map->height_text;
        if (strlen(target) < MAX_NB_LENGTH && isdigit(event.text.text[0])) {
            strcat(target, event.text.text);
        }
    }

    if (event.type == SDL_KEYDOWN) {
        if (event.key.keysym.sym == SDLK_BACKSPACE) {
            char *target = session->sous_menu_modif_taille_map->is_typing_width ? session->sous_menu_modif_taille_map->width_text : session->sous_menu_modif_taille_map->height_text;
            if (strlen(target) > 0) {
                target[strlen(target) - 1] = '\0';
            }
        }
    }

    if (event.type == SDL_KEYUP) {
        if (event.key.keysym.sym == SDLK_DOWN || event.key.keysym.sym == SDLK_UP) {
            session->sous_menu_modif_taille_map->is_typing_width = !session->sous_menu_modif_taille_map->is_typing_width;
            session->sous_menu_modif_taille_map->cursorVisible = 1;
            session->sous_menu_modif_taille_map->lastCursorToggle = SDL_GetTicks();
        }
        if (event.key.keysym.sym == SDLK_RETURN) {
            valider_modif_taille_map(session, musique);
        }
    }
}

void valider_modif_taille_map(Session_modif_map* session, Musique* musique) {
    int width = atoi(session->sous_menu_modif_taille_map->width_text);
    int height = atoi(session->sous_menu_modif_taille_map->height_text);
    if (width >= TAILLE_MIN_MAP && width <= TAILLE_MAX_MAP && height >= TAILLE_MIN_MAP && height <= TAILLE_MAX_MAP) {
        playSoundEffect(musique->select);
        *session->map_totale = modif_taille_map(session->map_totale, width, height);
        if (session->zoom > session->map_totale->x - ZOOM_MAX) {
            session->zoom = session->map_totale->x - ZOOM_MAX;
        }
        if (session->zoom > session->map_totale->y - ZOOM_MAX) {
            session->zoom = session->map_totale->y - ZOOM_MAX;
        }
        if (session->position_zoom_x + session->map->x > session->map_totale->x){
            session->position_zoom_x = session->map_totale->x - session->map->x;
            if (session->position_zoom_x < 0){
                session->position_zoom_x = 0;
            }
        }
        if (session->position_zoom_y + session->map->y > session->map_totale->y){
            session->position_zoom_y = session->map_totale->y - session->map->y;
            if (session->position_zoom_y < 0){
                session->position_zoom_y = 0;
            }
        }
        nouveau_zoom(session->map, session->map_totale, session->zoom, session->position_zoom_x, session->position_zoom_y);
        session->message->button_base.label = "La map à bien été redimentionnée";
        session->message->couleur_message = (SDL_Color){255, 255, 255, 255};
        session->message->couleur_fond = (SDL_Color){0, 255, 0, 150};
        session->is_modif_taille_map = 0;
        SDL_StopTextInput();
    } else {
        char *buffer = malloc(100); // Allouer un espace mémoire suffisant
        if (buffer) {
            snprintf(buffer, 100, "Les dimensions doivent être entre %d et %d !", TAILLE_MIN_MAP, TAILLE_MAX_MAP);
            session->message->button_base.label = buffer;
        }        session->message->couleur_message = (SDL_Color){0, 0, 0, 255};
        session->message->couleur_fond = (SDL_Color){255, 50, 0, 200};
    }
    session->message->temps_affichage = 3;
    session->message->is_visible = 1;
    session->message->start_time = time(NULL);
}

void affiche_interface (SDL_Renderer* ren){
    if (SDL_GetTicks() - session->sous_menu_modif_taille_map->lastCursorToggle >= CURSOR_BLINK_TIME) {
        session->sous_menu_modif_taille_map->cursorVisible = !session->sous_menu_modif_taille_map->cursorVisible;
        session->sous_menu_modif_taille_map->lastCursorToggle = SDL_GetTicks();
    }
    // Texte affiché (ajoute un curseur clignotant)
    snprintf(session->sous_menu_modif_taille_map->displayWidth, sizeof(session->sous_menu_modif_taille_map->displayWidth), "%s%s", session->sous_menu_modif_taille_map->width_text, (session->sous_menu_modif_taille_map->is_typing_width && session->sous_menu_modif_taille_map->cursorVisible) ? "|" : " ");
    snprintf(session->sous_menu_modif_taille_map->displayHeight, sizeof(session->sous_menu_modif_taille_map->displayHeight), "%s%s", session->sous_menu_modif_taille_map->height_text, (!session->sous_menu_modif_taille_map->is_typing_width && session->sous_menu_modif_taille_map->cursorVisible) ? "|" : " ");
    // Afficher les champs de texte
    session->sous_menu_modif_taille_map->entree_width->label = session->sous_menu_modif_taille_map->displayWidth;
    session->sous_menu_modif_taille_map->entree_height->label = session->sous_menu_modif_taille_map->displayHeight;
    renderButton(ren, session->sous_menu_modif_taille_map->entree_width, session->color->text, session->color->bg_text_descriptif, session->color->bg_text_descriptif);
    renderButton(ren, session->sous_menu_modif_taille_map->entree_height, session->color->text, session->color->bg_text_descriptif, session->color->bg_text_descriptif);
    
}
*/
void Grapheur (SDL_Renderer* ren){
    /*
    int running = 1;
    SDL_Event event;
    time_t current_time;

    while (running) {
        current_time = time(NULL);
        
        affiche_interface(ren, current_time);
        
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) running = 0;
            if (session->is_enregistrement) {
                handle_events_enregistrement(event, session, musique);
            } else if (session->is_modif_taille_map) {
                handle_events_modif_taille_map(event, session, musique);
            } else {
                handle_events_createur_map(event, session, &running, musique, ren, &z_pressed);
            }
        }
        updateDisplay(ren);
    }
    free_session(session);
    SDL_StopTextInput();
*/
    
}
