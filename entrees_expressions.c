#include "entrees_expressions.h"

float f (float x) {
    return sin(x);
}
float g (float x) {
    return cos(x);
}
float h (float x) {
    return exp(x);
}
float i (float x) {
    return x;
}
float j (float x) {
    return 2 - x;
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

void init_placement_entrees (SDL_Renderer* ren, Expression_fonction* expression, Parametres_bandes_entrees params, SDL_Rect surface_bande_haut, Colors* colors){
    expression->entree_selectionnee = SELECTION_NULL;
    expression->rect_initial.x = surface_bande_haut.x;
    expression->rect_initial.h = params.height_bande_expression;
    expression->rect_initial.y = surface_bande_haut.y + expression->numero * (expression->rect_initial.h); // pour centrer : (TAILLE_BANDE_HAUT + TAILLE_BANDE_DESCRIPTIONS - but_2[j]->champs_texte->rect.h)/2 ;
    expression->rect_initial.w = surface_bande_haut.w;
    expression->rect_affiche = expression->rect_initial;
    if (expression->numero % 2 == 0){
        expression->bg_color = colors->bg_bandes_expression_1;
    } else {
        expression->bg_color = colors->bg_bandes_expression_2;
    }
    
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
            but_2[j]->position_initiale.w = params.width_entree_expression;
        } else {
            but_2[j]->position_initiale.w = params.width_entrees_bornes;
        }
        but_2[j]->position_initiale.h = params.height_entrees_pourcentage * expression->rect_initial.h / 100;
        but_2[j]->position_initiale.x = expression->rect_initial.x + params.marge_entree_gauche + j*(params.width_entrees_bornes + params.espace_entre_entrees);
        but_2[j]->position_initiale.y = expression->rect_initial.y + (expression->rect_initial.h - but_2[j]->position_initiale.h) / 2;
        but_2[j]->champs_texte->rect = but_2[j]->position_initiale;
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

    expression->color_picker = malloc(sizeof(Color_picker));
    *expression->color_picker = init_color_picker(ren, expression->rect_initial.x + params.marge_entree_gauche + NB_ENTREES*params.espace_entre_entrees + 2*params.width_entrees_bornes + params.width_entree_expression + params.taille_color_picker/2,
                                                        expression->rect_initial.y + expression->rect_initial.h/2,
                                                        params.taille_color_picker, &expression->fonction.color, classic_colors[expression->numero % NB_COULEURS_CLASSIQUES]);
}

void init_bande_entrees (SDL_Renderer* ren, Bande_entrees* bande_entrees, Colors* colors){
    bande_entrees->expanding = false;
    bande_entrees->scroll_offset = 0;
    bande_entrees->surface.x = 0;
    bande_entrees->surface.y = TAILLE_BANDE_DESCRIPTIONS;
    bande_entrees->surface.w = FEN_X - TAILLE_BANDE_DROITE;
    bande_entrees->surface.h = TAILLE_BANDE_HAUT - TAILLE_BANDE_DESCRIPTIONS;

    bande_entrees->params.width_entrees_bornes = 150;
    bande_entrees->params.width_entree_expression = 400;
    bande_entrees->params.height_entrees_pourcentage = 70;
    bande_entrees->params.height_bande_expression = 70;
    bande_entrees->params.taille_color_picker = 0.4 * bande_entrees->params.height_bande_expression;
    bande_entrees->params.height_texte_desctriptif = TAILLE_BANDE_DESCRIPTIONS - 10;
    bande_entrees->params.espace_entre_entrees = (FEN_X - TAILLE_BANDE_DROITE - bande_entrees->params.taille_color_picker - 2*bande_entrees->params.width_entrees_bornes - bande_entrees->params.width_entree_expression) / (NB_ENTREES + 2);
    bande_entrees->params.marge_entree_gauche = bande_entrees->params.espace_entre_entrees;

    init_placement_bande_descriptive(bande_entrees, bande_entrees->params, colors);

    float (*fx[])(float) = {f,g,h,i,j};
    const char* nom_f[] = {"sin(x)", "cos(x)", "exp(x)", "x", "2-x"};
    bande_entrees->nb_expressions = 0;
    for (int i = 0; i < 5; i++) {
        bande_entrees->expressions[i] = malloc(sizeof(Expression_fonction));
        bande_entrees->expressions[i]->numero = i;
        init_placement_entrees(ren, bande_entrees->expressions[i], bande_entrees->params, bande_entrees->surface, colors);
        bande_entrees->expressions[i]->fonction.borne_inf = -4;
        bande_entrees->expressions[i]->fonction.borne_sup = 4;
        //bande_entrees->expressions[i]->fonction.color = classic_colors[i];
        bande_entrees->expressions[i]->fonction.f = fx[i];
        strcpy(bande_entrees->expressions[i]->expression->text, nom_f[i]);
        strcpy(bande_entrees->expressions[i]->borne_inf->text, "-4");
        strcpy(bande_entrees->expressions[i]->borne_sup->text, "4");
        bande_entrees->nb_expressions++;
    }
}

void resize_bande_haut (Bande_entrees* bande_entrees){
    bande_entrees->surface.w = FEN_X - TAILLE_BANDE_DROITE;
    bande_entrees->params.espace_entre_entrees = (FEN_X - TAILLE_BANDE_DROITE - bande_entrees->params.taille_color_picker - 2*bande_entrees->params.width_entrees_bornes - bande_entrees->params.width_entree_expression) / (NB_ENTREES + 2);
    bande_entrees->params.marge_entree_gauche = bande_entrees->params.espace_entre_entrees;

    bande_entrees->texte_descriptif_borne_inf->rect.x = bande_entrees->params.marge_entree_gauche;
    bande_entrees->texte_descriptif_borne_sup->rect.x = bande_entrees->params.marge_entree_gauche + bande_entrees->params.width_entrees_bornes + bande_entrees->params.espace_entre_entrees;
    bande_entrees->texte_descriptif_expression->rect.x = bande_entrees->params.marge_entree_gauche + 2*(bande_entrees->params.width_entrees_bornes + bande_entrees->params.espace_entre_entrees);

    for (int i = 0; i < bande_entrees->nb_expressions; i++) {
        bande_entrees->expressions[i]->rect_initial.w = bande_entrees->surface.w;
        bande_entrees->expressions[i]->rect_affiche.w = bande_entrees->surface.w;

        bande_entrees->expressions[i]->borne_inf->position_initiale.x = bande_entrees->expressions[i]->rect_initial.x + bande_entrees->params.marge_entree_gauche;
        bande_entrees->expressions[i]->borne_sup->position_initiale.x = bande_entrees->expressions[i]->rect_initial.x + bande_entrees->params.marge_entree_gauche + bande_entrees->params.width_entrees_bornes + bande_entrees->params.espace_entre_entrees;
        bande_entrees->expressions[i]->expression->position_initiale.x = bande_entrees->expressions[i]->rect_initial.x + bande_entrees->params.marge_entree_gauche + 2*(bande_entrees->params.width_entrees_bornes + bande_entrees->params.espace_entre_entrees);
        
        bande_entrees->expressions[i]->borne_inf->champs_texte->rect.x = bande_entrees->expressions[i]->borne_inf->position_initiale.x;
        bande_entrees->expressions[i]->borne_sup->champs_texte->rect.x = bande_entrees->expressions[i]->borne_sup->position_initiale.x;
        bande_entrees->expressions[i]->expression->champs_texte->rect.x = bande_entrees->expressions[i]->expression->position_initiale.x;
    
        init_placement_color_picker(bande_entrees->expressions[i]->color_picker);
        bande_entrees->expressions[i]->color_picker->boutton_x = bande_entrees->expressions[i]->rect_initial.x + bande_entrees->params.marge_entree_gauche + NB_ENTREES*bande_entrees->params.espace_entre_entrees + 2*bande_entrees->params.width_entrees_bornes + bande_entrees->params.width_entree_expression + bande_entrees->params.taille_color_picker/2;
    }
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

void placement_pour_affichage_avec_offset (Expression_fonction* expression, int offset){
    expression->rect_affiche.y = expression->rect_initial.y - offset;
    for (int i = 0; i < 3; i++) {
        expression->champs_entrees[i]->champs_texte->rect.y = expression->champs_entrees[i]->position_initiale.y - offset;
        expression->color_picker->boutton_y_affiche = expression->color_picker->boutton_y - offset;
    }
}

void affiche_bande_haut (SDL_Renderer* ren, Bande_entrees* bande_entrees, Colors* colors){
    // Fond de la bande haute des champs
    affiche_bande_arrondis_en_bas(ren, 0, TAILLE_BANDE_DESCRIPTIONS, FEN_X - TAILLE_BANDE_DROITE, bande_entrees->surface.y + bande_entrees->surface.h, RAYON_BAS_BANDE_HAUT, colors->bande_haute_expressions);

    // Texte affiché (ajoute un curseur clignotant)
    const char* texte_defaut[] = {"ex: -5", "ex: 5", "ex: sin(x)"};
    Entree_texte *target_champs = NULL;
    for (int j = 0; j < bande_entrees->nb_expressions; j++) {
        if (bande_entrees->expressions[j]->visible) {
            boxRGBA(ren, bande_entrees->expressions[j]->rect_affiche.x, bande_entrees->expressions[j]->rect_affiche.y, bande_entrees->expressions[j]->rect_affiche.x + bande_entrees->expressions[j]->rect_affiche.w, bande_entrees->expressions[j]->rect_affiche.y + bande_entrees->expressions[j]->rect_affiche.h, bande_entrees->expressions[j]->bg_color.r, bande_entrees->expressions[j]->bg_color.g, bande_entrees->expressions[j]->bg_color.b, bande_entrees->expressions[j]->bg_color.a);
            for (int i = 0; i < NB_ENTREES; i++) {
                target_champs = bande_entrees->expressions[j]->champs_entrees[i];
                if (SDL_GetTicks() - target_champs->lastCursorToggle >= CURSOR_BLINK_TIME) {
                    target_champs->cursorVisible = !target_champs->cursorVisible;
                    target_champs->lastCursorToggle = SDL_GetTicks();
                }
                if (i != bande_entrees->expressions[j]->entree_selectionnee && strcmp(target_champs->text, "") == 0) {
                    target_champs->champs_texte->label = texte_defaut[i];
                } else {
                    strcpy(target_champs->display, target_champs->text);
                    if (bande_entrees->expressions[j]->entree_selectionnee == target_champs->type_entree && target_champs->cursorVisible){
                        insert_char(target_champs->display, target_champs->position_cursor, '|');
                    } else insert_char(target_champs->display, target_champs->position_cursor, ' ');
                    target_champs->champs_texte->label = target_champs->display;
                }
                renderButton(ren, target_champs->champs_texte);
            }
            affiche_boutton_color_picker(ren, bande_entrees->expressions[j]->color_picker);
        }
    }

    // Fond de la bande haute des descriptions
    boxRGBA(ren, 0, 0, FEN_X - TAILLE_BANDE_DROITE, TAILLE_BANDE_DESCRIPTIONS, colors->bande_haute_description.r, colors->bande_haute_description.g, colors->bande_haute_description.b, colors->bande_haute_description.a);
    // Afficher les textes descriptifs
    renderButton(ren, bande_entrees->texte_descriptif_borne_inf);
    renderButton(ren, bande_entrees->texte_descriptif_borne_sup);
    renderButton(ren, bande_entrees->texte_descriptif_expression);
}


