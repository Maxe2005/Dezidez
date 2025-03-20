#include "entrees_expressions.h"

Message message;

float f (float x) {
    return sin(x);
}
float g (float x) {
    return cos(x);
}
float h (float x) {
    return exp(x);
}
float k (float x) {
    return x;
}
float l (float x) {
    return 2 - x;
}

void init_placement_bande_descriptive (Bande_entrees* bande_entrees, Parametres_bandes_entrees params, Colors* colors){
    // Bande descriptive : bornes inférieure, supérieure et expression
    int num_element_du_premier_champs = 3;
    bande_entrees->texte_descriptif_borne_inf = malloc(sizeof(Button));
    bande_entrees->texte_descriptif_borne_sup = malloc(sizeof(Button));
    bande_entrees->texte_descriptif_expression = malloc(sizeof(Button));
    Button* but[] = {bande_entrees->texte_descriptif_borne_inf, bande_entrees->texte_descriptif_borne_sup, bande_entrees->texte_descriptif_expression};
    char* noms[] = {"Borne inférieure", "Borne supérieure", "Expression"};
    int x = calcul_pos(params.width_elements, params.espace_entre_elements, num_element_du_premier_champs);
    for (int j = 0; j < 3; j++){
        but[j]->rect.w = params.width_elements[j+num_element_du_premier_champs];
        but[j]->rect.h = params.height_texte_desctriptif;
        but[j]->rect.x = x;
        x += params.width_elements[j+num_element_du_premier_champs] + params.espace_entre_elements;
        but[j]->rect.y = (TAILLE_BANDE_DESCRIPTIONS - but[j]->rect.h)/2 ;
        but[j]->is_survolable = 0;
        but[j]->label = noms[j];
        but[j]->color_text = colors->texte_descriptifs_bande_haut;
        but[j]->color_base = colors->bande_haute_description;
        but[j]->font_text = fonts[4];
        but[j]->radius = 0;
    }
}

void init_button_deplacement (SDL_Renderer* ren, Expression_fonction* expression, Parametres_bandes_entrees params, Colors* colors){
    expression->button_deplacement.bt.image = load_image(ren, "Icons/angles_up_down.png");
    expression->button_deplacement.rect_base.h = params.taille_button_deplacement;
    expression->button_deplacement.rect_base.w = expression->button_deplacement.rect_base.h;
    expression->button_deplacement.rect_base.y = expression->rect_initial.y + (expression->rect_initial.h - expression->button_deplacement.rect_base.h) / 2;;
    expression->button_deplacement.rect_base.x = calcul_pos(params.width_elements, params.espace_entre_elements, 1);
    expression->button_deplacement.bt.rect = expression->button_deplacement.rect_base;
    expression->button_deplacement.bt.is_survolable = 0;
    expression->button_deplacement.bt.color_base = expression->bg_color;
    expression->button_deplacement.bt.radius = expression->button_deplacement.rect_base.w / 3;
    expression->button_deplacement.bt.pourcentage_place = 70;
}

void init_button_visibilite (SDL_Renderer* ren, Expression_fonction* expression, Parametres_bandes_entrees params, Colors* colors){
    expression->button_visibilite.bt.image = load_image(ren, "Icons/oeil.png");
    expression->button_visibilite.rect_base.h = params.taille_button_visibilite;
    expression->button_visibilite.rect_base.w = expression->button_visibilite.rect_base.h;
    expression->button_visibilite.rect_base.y = expression->rect_initial.y + (expression->rect_initial.h - expression->button_visibilite.rect_base.h) / 2;;
    expression->button_visibilite.rect_base.x = calcul_pos(params.width_elements, params.espace_entre_elements, 2);
    expression->button_visibilite.bt.rect = expression->button_visibilite.rect_base;
    expression->button_visibilite.bt.is_survolable = 1;
    expression->button_visibilite.bt.hovered = 0;
    expression->button_visibilite.bt.color_base = expression->bg_color;
    if (expression->numero % 2 == 0) {
        expression->button_deplacement.bt.color_hover = colors->bg_bandes_expression_2;
    } else {
        expression->button_deplacement.bt.color_hover = colors->bg_bandes_expression_1;
    }
    expression->button_visibilite.bt.radius = expression->button_visibilite.rect_base.w / 3;
    expression->button_visibilite.bt.pourcentage_place = 70;
    expression->button_visibilite.bt.taille_bonus_hover_x = 0;
    expression->button_visibilite.bt.taille_bonus_hover_y = 0;
}

void init_button_delete (SDL_Renderer* ren, Expression_fonction* expression, Parametres_bandes_entrees params, Colors* colors){
    expression->button_delete.bt.image = load_image(ren, "Icons/poubelle.png");
    expression->button_delete.rect_base.h = params.taille_button_delete;
    expression->button_delete.rect_base.w = expression->button_delete.rect_base.h;
    expression->button_delete.rect_base.y = expression->rect_initial.y + (expression->rect_initial.h - expression->button_delete.rect_base.h) / 2;;
    expression->button_delete.rect_base.x = calcul_pos(params.width_elements, params.espace_entre_elements, NB_ELEMENTS_PAR_EXPRESSION);
    expression->button_delete.bt.rect = expression->button_delete.rect_base;
    expression->button_delete.bt.is_survolable = 1;
    expression->button_delete.bt.hovered = 0;
    expression->button_delete.bt.color_base = expression->bg_color;
    expression->button_delete.bt.color_hover = (SDL_Color){255, 0, 0, 255};
    expression->button_delete.bt.radius = expression->button_delete.rect_base.w / 3;
    expression->button_delete.bt.pourcentage_place = 70;
    expression->button_delete.bt.taille_bonus_hover_x = -10;
    expression->button_delete.bt.taille_bonus_hover_y = -10;
}

void init_champs_entrees (SDL_Renderer* ren, Expression_fonction* expression, Parametres_bandes_entrees params, Colors* colors){
    int num_element_du_premier_champs = 3;
    expression->borne_inf = malloc(sizeof(Entree_texte));
    expression->borne_sup = malloc(sizeof(Entree_texte));
    expression->expression = malloc(sizeof(Entree_texte));
    // ! L'ordre des Entree_texte est important pour l'initialisation des champs d'entrées, il doit correspondre à celui de l'enum SelectionEntree
    Entree_texte* but[] = {expression->borne_inf, expression->borne_sup, expression->expression};
    SelectionEntree type[] = {BORNE_INF, BORNE_SUP, EXPRESSION};
    int x = calcul_pos(params.width_elements, params.espace_entre_elements, num_element_du_premier_champs);
    for (int j = 0; j < 3; j++){
        but[j]->champs_texte = malloc(sizeof(Button));
        but[j]->type_entree = type[j];
        but[j]->position_initiale.w = params.width_elements[j+num_element_du_premier_champs];
        but[j]->position_initiale.h = params.height_entrees_pourcentage * expression->rect_initial.h / 100;
        but[j]->position_initiale.x = x;
        x += params.width_elements[j+num_element_du_premier_champs] + params.espace_entre_elements;
        but[j]->position_initiale.y = expression->rect_initial.y + (expression->rect_initial.h - but[j]->position_initiale.h) / 2;
        but[j]->champs_texte->rect = but[j]->position_initiale;
        but[j]->champs_texte->is_survolable = 1;
        but[j]->champs_texte->hovered = 0;
        but[j]->champs_texte->color_text = colors->texte_champ_entree;
        but[j]->champs_texte->color_base = colors->bg_champ_entree;
        but[j]->champs_texte->color_hover = colors->bg_champ_entree_hover;
        but[j]->champs_texte->font_text = fonts[1];
        but[j]->champs_texte->font_text_hover = fonts[2];
        but[j]->champs_texte->taille_bonus_hover_x = 0;
        but[j]->champs_texte->taille_bonus_hover_y = 0;
        but[j]->champs_texte->radius = 0;
        but[j]->cursorVisible = 0;
        but[j]->position_cursor = 0;
        but[j]->lastCursorToggle = SDL_GetTicks();
        strcpy(but[j]->text, "");
        expression->champs_entrees[j] = malloc(sizeof(Entree_texte));
        expression->champs_entrees[j] = but[j];
    }
}

void init_placement_entrees (SDL_Renderer* ren, Expression_fonction* expression, Parametres_bandes_entrees params, SDL_Rect surface_bande_haut, Colors* colors){
    expression->entree_selectionnee = SELECTION_NULL;
    expression->rect_initial.x = surface_bande_haut.x;
    expression->rect_initial.h = params.height_bande_expression;
    expression->rect_initial.y = surface_bande_haut.y + expression->numero * (expression->rect_initial.h); // pour centrer : (TAILLE_BANDE_HAUT + TAILLE_BANDE_DESCRIPTIONS - but[j]->champs_texte->rect.h)/2 ;
    expression->rect_initial.w = surface_bande_haut.w;
    expression->rect_affiche = expression->rect_initial;
    if (expression->numero % 2 == 0){
        expression->bg_color = colors->bg_bandes_expression_1;
    } else {
        expression->bg_color = colors->bg_bandes_expression_2;
    }

    // Bouton de déplacement inter-expressions
    init_button_deplacement(ren, expression, params, colors);

    // Bouton pour afficher/cacher la courbe
    init_button_visibilite(ren, expression, params, colors);
    
    // Les champs d'entrées
    init_champs_entrees(ren, expression, params, colors);

    // La couleur :
    expression->color_picker = malloc(sizeof(Color_picker));
    *expression->color_picker = init_color_picker(ren, calcul_pos(params.width_elements, params.espace_entre_elements, NB_ELEMENTS_PAR_EXPRESSION-1) + params.taille_color_picker/2,
                                                        expression->rect_initial.y + expression->rect_initial.h/2,
                                                        params.taille_color_picker, &expression->fonction.color, classic_colors[expression->numero % NB_COULEURS_CLASSIQUES]);

    // Bouton de suppression de l'expression
    init_button_delete(ren, expression, params, colors);
}

int calcul_pos (int tab[NB_ELEMENTS_PAR_EXPRESSION + 1], int espaces, int num_element){
    if (num_element < 1 || num_element > NB_ELEMENTS_PAR_EXPRESSION+1) return -1;
    int x = tab[0] + espaces;
    for (int i = 1; i < num_element; i++) {
        x += tab[i] + espaces;
    }
    return x;
}

void init_bande_entrees (SDL_Renderer* ren, Bande_entrees* bande_entrees, Colors* colors){
    bande_entrees->expanding = false;
    bande_entrees->scroll_offset = 0;
    bande_entrees->surface.x = 0;
    bande_entrees->surface.y = TAILLE_BANDE_DESCRIPTIONS;
    bande_entrees->surface.w = FEN_X - TAILLE_BANDE_DROITE;
    bande_entrees->surface.h = TAILLE_BANDE_HAUT - TAILLE_BANDE_DESCRIPTIONS;

    bande_entrees->params.height_entrees_pourcentage = 70;
    bande_entrees->params.height_bande_expression = 70;

    bande_entrees->params.taille_color_picker = 0.4 * bande_entrees->params.height_bande_expression;
    bande_entrees->params.taille_button_deplacement = 0.5 * bande_entrees->params.height_bande_expression;
    bande_entrees->params.taille_button_visibilite = 0.5 * bande_entrees->params.height_bande_expression;
    bande_entrees->params.taille_button_delete = 0.5 * bande_entrees->params.height_bande_expression;
    int i = 0;
    bande_entrees->params.width_elements[i++] = bande_entrees->surface.x;
    bande_entrees->params.width_elements[i++] = bande_entrees->params.taille_button_deplacement; 
    bande_entrees->params.width_elements[i++] = bande_entrees->params.taille_button_visibilite; 
    bande_entrees->params.width_elements[i++] = WIDTH_CHAMPS_BORNES;
    bande_entrees->params.width_elements[i++] = WIDTH_CHAMPS_BORNES;
    bande_entrees->params.width_elements[i++] = WIDTH_CHAMP_EXPRESSION;
    bande_entrees->params.width_elements[i++] = bande_entrees->params.taille_color_picker;
    bande_entrees->params.width_elements[i++] = bande_entrees->params.taille_button_delete;
    
    bande_entrees->params.height_texte_desctriptif = TAILLE_BANDE_DESCRIPTIONS - 10;
    bande_entrees->params.espace_entre_elements = (FEN_X - TAILLE_BANDE_DROITE - calcul_pos(bande_entrees->params.width_elements, 0, NB_ELEMENTS_PAR_EXPRESSION+1)) / (NB_ELEMENTS_PAR_EXPRESSION + 1);

    init_placement_bande_descriptive(bande_entrees, bande_entrees->params, colors);

    float (*fx[])(float) = {f,g,h,k,l};
    const char* nom_f[] = {"sin(x)", "cos(x)", "exp(x)", "x", "2-x"};
    bande_entrees->nb_expressions = 0;
    for (int i = 0; i < 5; i++) {
        bande_entrees->expressions[i] = malloc(sizeof(Expression_fonction));
        bande_entrees->expressions[i]->numero = i;
        init_placement_entrees(ren, bande_entrees->expressions[i], bande_entrees->params, bande_entrees->surface, colors);
        bande_entrees->expressions[i]->fonction.borne_inf = -4;
        bande_entrees->expressions[i]->fonction.borne_sup = 4;
        bande_entrees->expressions[i]->fonction.f = fx[i];
        strcpy(bande_entrees->expressions[i]->expression->text, nom_f[i]);
        strcpy(bande_entrees->expressions[i]->borne_inf->text, "-4");
        strcpy(bande_entrees->expressions[i]->borne_sup->text, "4");
        bande_entrees->nb_expressions++;
    }
}

void resize_bande_haut (Bande_entrees* bande_entrees){
    bande_entrees->surface.w = FEN_X - TAILLE_BANDE_DROITE;
    bande_entrees->params.espace_entre_elements = (FEN_X - TAILLE_BANDE_DROITE - calcul_pos(bande_entrees->params.width_elements, 0, NB_ELEMENTS_PAR_EXPRESSION)) / (NB_ELEMENTS_PAR_EXPRESSION + 1);

    int num_element_du_premier_champs = 3;
    Button* but[] = {bande_entrees->texte_descriptif_borne_inf, bande_entrees->texte_descriptif_borne_sup, bande_entrees->texte_descriptif_expression};
    int x = calcul_pos(bande_entrees->params.width_elements, bande_entrees->params.espace_entre_elements, num_element_du_premier_champs);
    for (int j = 0; j < 3; j++){
        but[j]->rect.x = x;
        x += bande_entrees->params.width_elements[j+num_element_du_premier_champs] + bande_entrees->params.espace_entre_elements;
    }
    
    for (int i = 0; i < bande_entrees->nb_expressions; i++) {
        bande_entrees->expressions[i]->rect_initial.w = bande_entrees->surface.w;
        bande_entrees->expressions[i]->rect_affiche.w = bande_entrees->surface.w;

        Entree_texte* but[] = {bande_entrees->expressions[i]->borne_inf, bande_entrees->expressions[i]->borne_sup, bande_entrees->expressions[i]->expression};
        int x = calcul_pos(bande_entrees->params.width_elements, bande_entrees->params.espace_entre_elements, num_element_du_premier_champs);
        for (int j = 0; j < 3; j++){
            but[j]->position_initiale.x = x;
            but[j]->champs_texte->rect.x = x;
            x += bande_entrees->params.width_elements[j+num_element_du_premier_champs] + bande_entrees->params.espace_entre_elements;
        }
            
        init_placement_color_picker(bande_entrees->expressions[i]->color_picker);
        bande_entrees->expressions[i]->color_picker->boutton_x = calcul_pos(bande_entrees->params.width_elements, bande_entrees->params.espace_entre_elements, NB_ELEMENTS_PAR_EXPRESSION-1) + bande_entrees->params.taille_color_picker/2;
        
        bande_entrees->expressions[i]->button_deplacement.rect_base.x = calcul_pos(bande_entrees->params.width_elements, bande_entrees->params.espace_entre_elements, 1);
        bande_entrees->expressions[i]->button_deplacement.bt.rect.x = calcul_pos(bande_entrees->params.width_elements, bande_entrees->params.espace_entre_elements, 1);
        
        bande_entrees->expressions[i]->button_visibilite.rect_base.x = calcul_pos(bande_entrees->params.width_elements, bande_entrees->params.espace_entre_elements, 2);
        bande_entrees->expressions[i]->button_visibilite.bt.rect.x = calcul_pos(bande_entrees->params.width_elements, bande_entrees->params.espace_entre_elements, 2);
        
        bande_entrees->expressions[i]->button_delete.rect_base.x = calcul_pos(bande_entrees->params.width_elements, bande_entrees->params.espace_entre_elements, NB_ELEMENTS_PAR_EXPRESSION);
        bande_entrees->expressions[i]->button_delete.bt.rect.x = calcul_pos(bande_entrees->params.width_elements, bande_entrees->params.espace_entre_elements, NB_ELEMENTS_PAR_EXPRESSION);
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
            set_message("PROBLEME DE CONVERSION", expression->borne_inf->champs_texte->rect);
        
        } else if(test > expression->fonction.borne_sup){
            set_message("BORNE INF SUPERIEUR A BORNE SUP", expression->borne_inf->champs_texte->rect);
        }
        else {
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
            set_message("PROBLEME DE CONVERSION", expression->borne_sup->champs_texte->rect);
        } 
        else if(expression->fonction.borne_inf > test){
            set_message("BORNE SUP INFERIEUR A BORNE INF", expression->borne_sup->champs_texte->rect);
        }
        else {
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

void set_message (const char* text_erreur, SDL_Rect endroit_erreur){
    message.start_time = time(NULL);
    message.button_base.label = text_erreur;
    message.is_visible = 1;
    message.button_base.rect.x = endroit_erreur.x;
    message.button_base.rect.y = endroit_erreur.y + FEN_Y/16;
    message.button_base.rect.w = FEN_X/4;
    message.button_base.rect.h = FEN_Y/16;

}

void placement_pour_affichage_avec_offset (Expression_fonction* expression, int offset){
    expression->rect_affiche.y = expression->rect_initial.y - offset;
    for (int i = 0; i < 3; i++) {
        expression->champs_entrees[i]->champs_texte->rect.y = expression->champs_entrees[i]->position_initiale.y - offset;
    }
    expression->color_picker->boutton_y_affiche = expression->color_picker->boutton_y - offset;
    expression->button_deplacement.bt.rect.y = expression->button_deplacement.rect_base.y - offset;
    expression->button_visibilite.bt.rect.y = expression->button_visibilite.rect_base.y - offset;
    expression->button_delete.bt.rect.y = expression->button_delete.rect_base.y - offset;
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
            renderImageButton(ren, &bande_entrees->expressions[j]->button_deplacement.bt);
            renderImageButton(ren, &bande_entrees->expressions[j]->button_visibilite.bt);
            renderImageButton(ren, &bande_entrees->expressions[j]->button_delete.bt);
        }
    }

    // Fond de la bande haute des descriptions
    boxRGBA(ren, 0, 0, FEN_X - TAILLE_BANDE_DROITE, TAILLE_BANDE_DESCRIPTIONS, colors->bande_haute_description.r, colors->bande_haute_description.g, colors->bande_haute_description.b, colors->bande_haute_description.a);
    // Afficher les textes descriptifs
    renderButton(ren, bande_entrees->texte_descriptif_borne_inf);
    renderButton(ren, bande_entrees->texte_descriptif_borne_sup);
    renderButton(ren, bande_entrees->texte_descriptif_expression);
}


