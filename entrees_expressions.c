#include "entrees_expressions.h"

float f (float x) {
    return sin(x);
}

float g (float x) {
    return cos(x);
}

void init_placement_bande_descriptive (Bande_entrees* bande_entrees, Parametres_bandes_entrees params, Colors* colors){
    // Bande descriptive : bornes inférieure, supérieure et expression
    bande_entrees->texte_descriptif_borne_inf = malloc(sizeof(Button));
    bande_entrees->texte_descriptif_borne_sup = malloc(sizeof(Button));
    bande_entrees->texte_descriptif_expression = malloc(sizeof(Button));
    Button* but[] = {bande_entrees->texte_descriptif_borne_inf, bande_entrees->texte_descriptif_borne_sup, bande_entrees->texte_descriptif_expression};
    char* noms[] = {"Borne inférieure", "Borne supérieure", "Expression"};
    for (int j = 0; j < 3; j++){
        if (j == 2) {
            but[j]->rect.w = params.width_entree_expression;
        } else {
            but[j]->rect.w = params.width_entrees_bornes;
        }
        but[j]->rect.h = params.height_texte_desctriptif;
        but[j]->rect.x = params.marge_entree_gauche + j*(params.width_entrees_bornes + params.espace_entre_entrees);
        but[j]->rect.y = (TAILLE_BANDE_DESCRIPTIONS - but[j]->rect.h)/2 ;
        but[j]->is_survolable = 0;
        but[j]->label = noms[j];
        but[j]->color_text = colors->texte_descriptifs_bande_haut;
        but[j]->color_base = colors->bande_haute_description;
        but[j]->font_text = fonts[4];
        but[j]->radius = 0;
    }
}

void init_placement_entrees (Expression_fonction* expression, Parametres_bandes_entrees params, Colors* colors){
    expression->entree_selectionnee = SELECTION_NULL;
    // Les champs d'entrées
    expression->borne_inf = malloc(sizeof(Entree_texte));
    expression->borne_sup = malloc(sizeof(Entree_texte));
    expression->expression = malloc(sizeof(Entree_texte));
    // ! L'ordre des Entree_texte est important pour l'initialisation des champs d'entrées, il doit correspondre à celui de l'enum SelectionEntree
    Entree_texte* but_2[] = {expression->borne_inf, expression->borne_sup, expression->expression};
    SelectionEntree type[] = {BORNE_INF, BORNE_SUP, EXPRESSION};
    for (int j = 0; j < 3; j++){
        but_2[j]->champs_texte = malloc(sizeof(Button));
        but_2[j]->type_entree = type[j];
        if (j == 2) {
            but_2[j]->champs_texte->rect.w = params.width_entree_expression;
        } else {
            but_2[j]->champs_texte->rect.w = params.width_entrees_bornes;
        }
        but_2[j]->champs_texte->rect.h = params.height_entrees;
        but_2[j]->champs_texte->rect.x = params.marge_entree_gauche + j*(params.width_entrees_bornes + params.espace_entre_entrees);
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
        but_2[j]->champs_texte->radius = 0;
        but_2[j]->cursorVisible = 0;
        but_2[j]->position_cursor = 0;
        but_2[j]->lastCursorToggle = SDL_GetTicks();
        strcpy(but_2[j]->text, "");
        expression->champs_entrees[j] = malloc(sizeof(Entree_texte));
        expression->champs_entrees[j] = but_2[j];
    }
}

void init_bande_entrees (Bande_entrees* bande_entrees, Colors* colors){
    Parametres_bandes_entrees params;
    params.width_entrees_bornes = 150;
    params.width_entree_expression = 400;
    params.height_entrees = 50;
    params.height_texte_desctriptif = TAILLE_BANDE_DESCRIPTIONS - 10;
    params.espace_entre_entrees = (FEN_X - 2*params.width_entrees_bornes - params.width_entree_expression - 100) / 4;
    params.marge_entree_gauche = params.espace_entre_entrees;

    bande_entrees->expanding = false;
    bande_entrees->scroll_offset = 0;
    bande_entrees->surface.x = 0;
    bande_entrees->surface.y = TAILLE_BANDE_DESCRIPTIONS;
    bande_entrees->surface.w = FEN_X - TAILLE_BANDE_DROITE;
    bande_entrees->surface.h = TAILLE_BANDE_HAUT - TAILLE_BANDE_DESCRIPTIONS;

    init_placement_bande_descriptive(bande_entrees, params, colors);

    bande_entrees->nb_expressions = 1;
    bande_entrees->expressions[0] = malloc(sizeof(Expression_fonction));
    init_placement_entrees(bande_entrees->expressions[0], params, colors);
    bande_entrees->expressions[0]->fonction.borne_inf = -4;
    bande_entrees->expressions[0]->fonction.borne_sup = 4;
    bande_entrees->expressions[0]->fonction.color = (SDL_Color){255, 0, 0, 255};
    bande_entrees->expressions[0]->fonction.f = f;
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

void charge_valeur_borne_inf (Expression_fonction* expression){
    if (expression->borne_inf->text[0] != '\0'){
        char *end;
        float test = strtof(expression->borne_inf->text, &end);
        if (*end != '\0') {
            printf("Conversion incomplète, problème à : %s, nb gardé : %f\n", end, test); //TODO : ajouter les messages d'erreur
        } else {
            expression->fonction.borne_inf = test;
        }
    }
}

void charge_valeur_borne_sup (Expression_fonction* expression){
    if (expression->borne_sup->text[0] != '\0'){
        char *end;
        float test = strtof(expression->borne_sup->text, &end);
        if (*end != '\0') {
            printf("Conversion incomplète, problème à : %s, nb gardé : %f\n", end, test); //TODO : ajouter les messages d'erreur
        } else {
            expression->fonction.borne_sup = test;
        }
    }
}

void execute_champs_select_and_change_focus (Expression_fonction* expression, SelectionEntree nouvelle_entree){
    if (expression->entree_selectionnee == BORNE_INF) charge_valeur_borne_inf(expression);
    if (expression->entree_selectionnee == BORNE_SUP) charge_valeur_borne_sup(expression);
    if (expression->entree_selectionnee == EXPRESSION){
        // TODO : A connecter avec les autres modules
    }
    expression->entree_selectionnee = nouvelle_entree;
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

void affiche_bande_des_champs (SDL_Renderer* renderer, int x1, int y1, int x2, int y2, int radius, SDL_Color color){
    // Dessiner le rectangle principal
    boxRGBA(renderer, x1, y1, x2, y2 - radius, color.r, color.g, color.b, 255);
    
    // Dessiner le bas arrondi seulement
    filledCircleRGBA(renderer, x1 + radius, y2 - radius, radius, color.r, color.g, color.b, 255);
    filledCircleRGBA(renderer, x2 - radius, y2 - radius, radius, color.r, color.g, color.b, 255);
    boxRGBA(renderer, x1 + radius, y2 - radius, x2 - radius, y2, color.r, color.g, color.b, 255);
}

void affiche_bande_haut (SDL_Renderer* ren, Bande_entrees* bande_entrees, Expression_fonction* expression, Colors* colors){
    // Fond de la bande haute des champs
    affiche_bande_des_champs(ren, 0, TAILLE_BANDE_DESCRIPTIONS, FEN_X - TAILLE_BANDE_DROITE, bande_entrees->surface.y + bande_entrees->surface.h, 20, colors->bande_haute_champs);

    // Texte affiché (ajoute un curseur clignotant)
    const char* texte_defaut[] = {"ex: -5", "ex: 5", "ex: sin(x)"};
    Entree_texte *target_champs = NULL;
    for (int i = 0; i < NB_ENTREES; i++) {
        target_champs = expression->champs_entrees[i];
        if (SDL_GetTicks() - target_champs->lastCursorToggle >= CURSOR_BLINK_TIME) {
            target_champs->cursorVisible = !target_champs->cursorVisible;
            target_champs->lastCursorToggle = SDL_GetTicks();
        }
        if (i != expression->entree_selectionnee && strcmp(target_champs->text, "") == 0) {
            target_champs->champs_texte->label = texte_defaut[i];
        } else {
            strcpy(target_champs->display, target_champs->text);
            if (expression->entree_selectionnee == target_champs->type_entree && target_champs->cursorVisible){
                insert_char(target_champs->display, target_champs->position_cursor, '|');
            } else insert_char(target_champs->display, target_champs->position_cursor, ' ');
            target_champs->champs_texte->label = target_champs->display;
        }
        renderButton(ren, target_champs->champs_texte);
    }

    // Fond de la bande haute des descriptions
    boxRGBA(ren, 0, 0, FEN_X - TAILLE_BANDE_DROITE, TAILLE_BANDE_DESCRIPTIONS, colors->bande_haute_description.r, colors->bande_haute_description.g, colors->bande_haute_description.b, colors->bande_haute_description.a);
    // Afficher les textes descriptifs
    renderButton(ren, bande_entrees->texte_descriptif_borne_inf);
    renderButton(ren, bande_entrees->texte_descriptif_borne_sup);
    renderButton(ren, bande_entrees->texte_descriptif_expression);
}


