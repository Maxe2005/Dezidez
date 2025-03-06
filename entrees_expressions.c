#include "entrees_expressions.h"

void init_placement_entrees (Bande_entrees* bande_entrees, Button* entrees[NB_ENTREES], Colors* colors){
    int width_entrees_bornes = 100;
    int width_entree_expression = 300;
    int height_entrees = 50;
    int height_texte_desctriptif = 40;
    int espace_entre_entrees = (FEN_X - 2*width_entrees_bornes - width_entree_expression - 100) / 4;
    int marge_entree_gauche = espace_entre_entrees;

    // Bande descriptive : bornes inférieure, supérieure et expression
    bande_entrees->texte_descriptif_borne_inf = malloc(sizeof(Button));
    bande_entrees->texte_descriptif_borne_sup = malloc(sizeof(Button));
    bande_entrees->texte_descriptif_expression = malloc(sizeof(Button));
    Button* but[] = {bande_entrees->texte_descriptif_borne_inf, bande_entrees->texte_descriptif_borne_sup, bande_entrees->texte_descriptif_expression};
    char* noms[] = {"Borne inférieure", "Borne supérieure", "Expression"};
    for (int j = 0; j < 3; j++){
        if (j == 2) {
            but[j]->rect.w = width_entree_expression;
        } else {
            but[j]->rect.w = width_entrees_bornes;
        }
        but[j]->rect.h = height_texte_desctriptif;
        but[j]->rect.x = marge_entree_gauche + j*(width_entrees_bornes + espace_entre_entrees);
        but[j]->rect.y = (TAILLE_BANDE_DESCRIPTIONS - but[j]->rect.h)/2 ;
        but[j]->is_survolable = 0;
        but[j]->label = noms[j];
        but[j]->color_text = colors->texte_descriptifs_bande_haut;
        but[j]->color_base = colors->bande_haute_description;
        but[j]->font_text = fonts[4];
    }

    // Les champs d'entrées
    bande_entrees->borne_inf.champs_texte = malloc(sizeof(Button));
    bande_entrees->borne_sup.champs_texte = malloc(sizeof(Button));
    bande_entrees->expression.champs_texte = malloc(sizeof(Button));
    Button* but_2[] = {bande_entrees->borne_inf.champs_texte, bande_entrees->borne_sup.champs_texte, bande_entrees->expression.champs_texte};
    for (int j = 0; j < 3; j++){
        if (j == 2) {
            but_2[j]->rect.w = width_entree_expression;
        } else {
            but_2[j]->rect.w = width_entrees_bornes;
        }
        but_2[j]->rect.h = height_entrees;
        but_2[j]->rect.x = marge_entree_gauche + j*(width_entrees_bornes + espace_entre_entrees);
        but_2[j]->rect.y = (TAILLE_BANDE_HAUT + TAILLE_BANDE_DESCRIPTIONS - but_2[j]->rect.h)/2 ;
        but_2[j]->is_survolable = 1;
        but_2[j]->hovered = 0;
        but_2[j]->color_text = colors->texte_champ_entree;
        but_2[j]->color_base = colors->bg_champ_entree;
        but_2[j]->color_hover = colors->bg_champ_entree_hover;
        but_2[j]->font_text = fonts[1];
        but_2[j]->font_text_hover = fonts[2];
        but_2[j]->taille_bonus_hover_x = 0;
        but_2[j]->taille_bonus_hover_y = 0;
        entrees[j] = but_2[j];
    }
    bande_entrees->borne_inf.cursorVisible = 0;
    strcpy(bande_entrees->borne_inf.text, " ");
    bande_entrees->borne_sup.cursorVisible = 0;
    strcpy(bande_entrees->borne_sup.text, " ");
    bande_entrees->expression.cursorVisible = 0;
    strcpy(bande_entrees->expression.text, " ");

}

void init_bande_entrees (Bande_entrees* bande_entrees, Colors* colors){
    bande_entrees->entree_selectionnee = SELECTION_NULL;
    init_placement_entrees(bande_entrees, bande_entrees->champs_entrees, colors);
}
/*
void handle_events_entrees_experssions(SDL_Event event, Session_modif_map* session, Musique* musique) {
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
*/
void affiche_bande_haut (SDL_Renderer* ren, Bande_entrees* bande_entrees, Colors* colors){
    // Fond de la bande haute
    SDL_SetRenderDrawColor(ren, colors->bande_haute_champs.r, colors->bande_haute_champs.g, colors->bande_haute_champs.b, colors->bande_haute_champs.a);
    SDL_Rect bande_haut_champs_entrees = {0, TAILLE_BANDE_DESCRIPTIONS, FEN_X, TAILLE_BANDE_HAUT - TAILLE_BANDE_DESCRIPTIONS};
    SDL_RenderFillRect(ren, &bande_haut_champs_entrees);
    SDL_SetRenderDrawColor(ren, colors->bande_haute_description.r, colors->bande_haute_description.g, colors->bande_haute_description.b, colors->bande_haute_description.a);
    SDL_Rect bande_haut_textes_descriptifs = {0, 0, FEN_X, TAILLE_BANDE_DESCRIPTIONS};
    SDL_RenderFillRect(ren, &bande_haut_textes_descriptifs);
    /*
    if (SDL_GetTicks() - session->sous_menu_modif_taille_map->lastCursorToggle >= CURSOR_BLINK_TIME) {
        session->sous_menu_modif_taille_map->cursorVisible = !session->sous_menu_modif_taille_map->cursorVisible;
        session->sous_menu_modif_taille_map->lastCursorToggle = SDL_GetTicks();
    }*/
    // Texte affiché (ajoute un curseur clignotant)
    snprintf(bande_entrees->borne_inf.display, sizeof(bande_entrees->borne_inf.display), "%s%s", bande_entrees->borne_inf.text, (bande_entrees->entree_selectionnee == BORNE_INF && bande_entrees->borne_inf.cursorVisible) ? "|" : " ");
    snprintf(bande_entrees->borne_sup.display, sizeof(bande_entrees->borne_sup.display), "%s%s", bande_entrees->borne_sup.text, (bande_entrees->entree_selectionnee == BORNE_SUP && bande_entrees->borne_sup.cursorVisible) ? "|" : " ");
    snprintf(bande_entrees->expression.display, sizeof(bande_entrees->expression.display), "%s%s", bande_entrees->expression.text, (bande_entrees->entree_selectionnee == EXPRESSION && bande_entrees->expression.cursorVisible) ? "|" : " ");
    // Afficher les champs d'entrées
    bande_entrees->borne_inf.champs_texte->label = bande_entrees->borne_inf.display;
    bande_entrees->borne_sup.champs_texte->label = bande_entrees->borne_sup.display;
    bande_entrees->expression.champs_texte->label = bande_entrees->expression.display;
    renderButton(ren, bande_entrees->borne_inf.champs_texte);
    renderButton(ren, bande_entrees->borne_sup.champs_texte);
    renderButton(ren, bande_entrees->expression.champs_texte);
    
    // Afficher les textes descriptifs
    renderButton(ren, bande_entrees->texte_descriptif_borne_inf);
    renderButton(ren, bande_entrees->texte_descriptif_borne_sup);
    renderButton(ren, bande_entrees->texte_descriptif_expression);
}


