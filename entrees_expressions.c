#include "entrees_expressions.h"

void init_placement_entrees (Bande_entrees* bande_entrees, Entree_texte* entrees[NB_ENTREES], Colors* colors){
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
    bande_entrees->borne_inf = malloc(sizeof(Entree_texte));
    bande_entrees->borne_sup = malloc(sizeof(Entree_texte));
    bande_entrees->expression = malloc(sizeof(Entree_texte));
    // ! L'ordre des Entree_texte est important pour l'initialisation des champs d'entrées, il doit correspondre à celui de l'enum SelectionEntree
    Entree_texte* but_2[] = {bande_entrees->borne_inf, bande_entrees->borne_sup, bande_entrees->expression};
    SelectionEntree type[] = {BORNE_INF, BORNE_SUP, EXPRESSION};
    for (int j = 0; j < 3; j++){
        but_2[j]->champs_texte = malloc(sizeof(Button));
        but_2[j]->type_entree = type[j];
        if (j == 2) {
            but_2[j]->champs_texte->rect.w = width_entree_expression;
        } else {
            but_2[j]->champs_texte->rect.w = width_entrees_bornes;
        }
        but_2[j]->champs_texte->rect.h = height_entrees;
        but_2[j]->champs_texte->rect.x = marge_entree_gauche + j*(width_entrees_bornes + espace_entre_entrees);
        but_2[j]->champs_texte->rect.y = (TAILLE_BANDE_HAUT + TAILLE_BANDE_DESCRIPTIONS - but_2[j]->champs_texte->rect.h)/2 ;
        but_2[j]->champs_texte->is_survolable = 1;
        but_2[j]->champs_texte->hovered = 0;
        but_2[j]->champs_texte->color_text = colors->texte_champ_entree;
        but_2[j]->champs_texte->color_base = colors->bg_champ_entree;
        but_2[j]->champs_texte->color_hover = colors->bg_champ_entree_hover;
        but_2[j]->champs_texte->font_text = fonts[1];
        but_2[j]->champs_texte->font_text_hover = fonts[2];
        but_2[j]->champs_texte->taille_bonus_hover_x = 0;
        but_2[j]->champs_texte->taille_bonus_hover_y = 0;
        but_2[j]->cursorVisible = 0;
        but_2[j]->position_cursor = 0;
        but_2[j]->lastCursorToggle = SDL_GetTicks();
        strcpy(but_2[j]->text, "");
        entrees[j] = malloc(sizeof(Entree_texte));
        entrees[j] = but_2[j];
    }

}

void init_bande_entrees (Bande_entrees* bande_entrees, Colors* colors){
    bande_entrees->entree_selectionnee = SELECTION_NULL;
    init_placement_entrees(bande_entrees, bande_entrees->champs_entrees, colors);
}

int is_souris_sur_button (Button button, int x_souris_px, int y_souris_px){
    return x_souris_px >= button.rect.x && x_souris_px <= button.rect.x + button.rect.w &&
                    y_souris_px >= button.rect.y && y_souris_px <= button.rect.y + button.rect.h;
}

void insert_char(char text[], int i, char c) {
    int len = strlen(text);
    
    // Vérifier que l'index est valide et qu'on peut insérer un caractère
    if (i < 0 || i > len) {
        printf("Insertion impossible\n");
        return;
    }

    // Décaler les caractères vers la droite
    for (int j = len; j >= i; j--) {
        text[j + 1] = text[j];
    }

    // Insérer le caractère c
    text[i] = c;
}

void remove_char(char text[], int i) {
    int len = strlen(text);

    // Vérifier que l'index est valide
    if (i < 0 || i >= len) {
        printf("Suppression impossible\n");
        return;
    }

    // Décaler les caractères vers la gauche
    for (int j = i; j < len; j++) {
        text[j] = text[j + 1];
    }
}

int handle_events_entrees_experssions(SDL_Event event, Bande_entrees* bande_entrees) {
    if (event.type == SDL_MOUSEMOTION) {
        bande_entrees->x_souris_px = event.motion.x;
        bande_entrees->y_souris_px = event.motion.y;

        // Souris sur un bouton ?
        for (int i = 0; i < NB_ENTREES; i++) {
            if (is_souris_sur_button(*bande_entrees->champs_entrees[i]->champs_texte, bande_entrees->x_souris_px, bande_entrees->y_souris_px)) {
                bande_entrees->champs_entrees[i]->champs_texte->hovered = 1;
            } else {
                bande_entrees->champs_entrees[i]->champs_texte->hovered = 0;
            }
        }
    }

    if (event.type == SDL_MOUSEBUTTONUP) {
        // Vérifier si on clique sur un champ
        int au_moins_un_champs_selectionne = 0;
        for (int i = 0; i < NB_ENTREES; i++) {
            if (is_souris_sur_button(*bande_entrees->champs_entrees[i]->champs_texte, bande_entrees->x_souris_px, bande_entrees->y_souris_px)) {
                bande_entrees->entree_selectionnee = bande_entrees->champs_entrees[i]->type_entree;
                bande_entrees->champs_entrees[i]->cursorVisible = 1;
                bande_entrees->champs_entrees[i]->lastCursorToggle = SDL_GetTicks();
                au_moins_un_champs_selectionne = 1;
            }
        }
        if (!au_moins_un_champs_selectionne) {
            bande_entrees->entree_selectionnee = SELECTION_NULL;
        }
    }

    if (event.type == SDL_TEXTINPUT) {
        if (bande_entrees->entree_selectionnee != SELECTION_NULL){
            Entree_texte *target = bande_entrees->champs_entrees[bande_entrees->entree_selectionnee];
            if (bande_entrees->entree_selectionnee < 2) {
                if (strlen(target->text) < MAX_LEN_ENTREES_BORNES && isdigit(event.text.text[0])) {
                    insert_char(target->text, target->position_cursor++, event.text.text[0]);
                }
            } else
            if (strlen(target->text) < MAX_LEN_STR) {
                insert_char(target->text, target->position_cursor++, event.text.text[0]);
            }
        }
    }

    if (event.type == SDL_KEYDOWN) {
        if (bande_entrees->entree_selectionnee != SELECTION_NULL){
            if (event.key.keysym.sym == SDLK_BACKSPACE) {
                Entree_texte *target = bande_entrees->champs_entrees[bande_entrees->entree_selectionnee];
                if (strlen(target->text) > 0 && target->position_cursor > 0) {
                    remove_char(target->text, --target->position_cursor);
                }
            }

            if (event.key.keysym.sym == SDLK_DOWN || event.key.keysym.sym == SDLK_UP) {
                if (event.key.keysym.sym == SDLK_DOWN) {
                    bande_entrees->entree_selectionnee = (bande_entrees->entree_selectionnee - 1 + NB_ENTREES) % NB_ENTREES;
                } else {
                    bande_entrees->entree_selectionnee = (bande_entrees->entree_selectionnee + 1) % NB_ENTREES;
                }
                for (int i = 0; i < NB_ENTREES; i++) {
                    bande_entrees->champs_entrees[i]->cursorVisible = 0;
                }
                bande_entrees->champs_entrees[bande_entrees->entree_selectionnee]->cursorVisible = 1;
                bande_entrees->champs_entrees[bande_entrees->entree_selectionnee]->lastCursorToggle = SDL_GetTicks();
            }

            if (event.key.keysym.sym == SDLK_RIGHT){
                Entree_texte *target = bande_entrees->champs_entrees[bande_entrees->entree_selectionnee];
                if (target->position_cursor < strlen(target->text)) {
                    target->position_cursor++;
                }
            }
            if (event.key.keysym.sym == SDLK_LEFT){
                Entree_texte *target = bande_entrees->champs_entrees[bande_entrees->entree_selectionnee];
                if (target->position_cursor > 0) {
                    target->position_cursor--;
                }
            }
        }
    }

    if (event.type == SDL_KEYUP) {
        if (event.key.keysym.sym == SDLK_RETURN) {
            //valider_modif_taille_map(session, musique);
        }
    }
    
    return bande_entrees->entree_selectionnee == SELECTION_NULL;
}
/*
void valider_modif_taille_map(Session_modif_map* session) {
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

    // Texte affiché (ajoute un curseur clignotant)
    const char* texte_defaut[] = {"ex: -5", "ex: 5", "ex: sin(x)"};
    Entree_texte *target_champs = NULL;
    for (int i = 0; i < NB_ENTREES; i++) {
        target_champs = bande_entrees->champs_entrees[i];
        if (SDL_GetTicks() - target_champs->lastCursorToggle >= CURSOR_BLINK_TIME) {
            target_champs->cursorVisible = !target_champs->cursorVisible;
            target_champs->lastCursorToggle = SDL_GetTicks();
        }
        if (i != bande_entrees->entree_selectionnee && strcmp(target_champs->text, "") == 0) {
            target_champs->champs_texte->label = texte_defaut[i];
        } else {
            strcpy(target_champs->display, target_champs->text);
            if (bande_entrees->entree_selectionnee == target_champs->type_entree && target_champs->cursorVisible){
                insert_char(target_champs->display, target_champs->position_cursor, '|');
            } else insert_char(target_champs->display, target_champs->position_cursor, ' ');
            target_champs->champs_texte->label = target_champs->display;
        }
        renderButton(ren, target_champs->champs_texte);
    }

    // Afficher les textes descriptifs
    renderButton(ren, bande_entrees->texte_descriptif_borne_inf);
    renderButton(ren, bande_entrees->texte_descriptif_borne_sup);
    renderButton(ren, bande_entrees->texte_descriptif_expression);
}


