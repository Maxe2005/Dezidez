#include "bande_haute.h"

Message message;

void init_placement_bande_descriptive (Bande_haute* bande_haute, Parametres_bandes_entrees params){
    // Bande descriptive : bornes inférieure, supérieure et expression
    int num_element_du_premier_champs = 3;
    bande_haute->texte_descriptif_borne_inf = malloc(sizeof(Button));
    bande_haute->texte_descriptif_borne_sup = malloc(sizeof(Button));
    bande_haute->texte_descriptif_expression = malloc(sizeof(Button));
    Button* but[] = {bande_haute->texte_descriptif_borne_inf, bande_haute->texte_descriptif_borne_sup, bande_haute->texte_descriptif_expression};
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

void init_button_deplacement (SDL_Renderer* ren, Expression_fonction* expression, Parametres_bandes_entrees params){
    expression->button_deplacement.bt.image = load_image(ren, "Icons/angles_up_down.png");
    expression->button_deplacement.rect_base.h = params.taille_button_deplacement;
    expression->button_deplacement.rect_base.w = expression->button_deplacement.rect_base.h;
    expression->button_deplacement.rect_base.y = expression->rect_initial.y + (expression->rect_initial.h - expression->button_deplacement.rect_base.h) / 2;
    expression->button_deplacement.rect_base.x = calcul_pos(params.width_elements, params.espace_entre_elements, 1);
    expression->button_deplacement.bt.rect = expression->button_deplacement.rect_base;
    expression->button_deplacement.bt.is_survolable = 0;
    expression->button_deplacement.bt.color_base = expression->bg_color;
    expression->button_deplacement.bt.radius = expression->button_deplacement.rect_base.w / 3;
    expression->button_deplacement.bt.pourcentage_place = 70;
}

void init_button_visibilite (SDL_Renderer* ren, Expression_fonction* expression, Parametres_bandes_entrees params){
    expression->image_button_visible = load_image(ren, "Icons/oeil.png");
    expression->image_button_invisible = load_image(ren, "Icons/oeil_barre.png");
    expression->button_visibilite.bt.image = expression->image_button_visible;
    expression->button_visibilite.rect_base.h = params.taille_button_visibilite;
    expression->button_visibilite.rect_base.w = expression->button_visibilite.rect_base.h;
    expression->button_visibilite.rect_base.y = expression->rect_initial.y + (expression->rect_initial.h - expression->button_visibilite.rect_base.h) / 2;
    expression->button_visibilite.rect_base.x = calcul_pos(params.width_elements, params.espace_entre_elements, 2);
    expression->button_visibilite.bt.rect = expression->button_visibilite.rect_base;
    expression->button_visibilite.bt.is_survolable = 1;
    expression->button_visibilite.bt.hovered = 0;
    expression->button_visibilite.bt.color_base = expression->bg_color;
    expression->button_visibilite.bt.color_hover = expression->bg_color_oppo;
    expression->button_visibilite.bt.radius = expression->button_visibilite.rect_base.w / 3;
    expression->button_visibilite.bt.pourcentage_place = 70;
    expression->button_visibilite.bt.taille_bonus_hover_x = 0;
    expression->button_visibilite.bt.taille_bonus_hover_y = 0;
}

void init_button_delete (SDL_Renderer* ren, Expression_fonction* expression, Parametres_bandes_entrees params){
    expression->button_delete.bt.image = load_image(ren, "Icons/poubelle.png");
    expression->button_delete.rect_base.h = params.taille_button_delete;
    expression->button_delete.rect_base.w = expression->button_delete.rect_base.h;
    expression->button_delete.rect_base.y = expression->rect_initial.y + (expression->rect_initial.h - expression->button_delete.rect_base.h) / 2;
    expression->button_delete.rect_base.x = calcul_pos(params.width_elements, params.espace_entre_elements, NB_ELEMENTS_PAR_EXPRESSION);
    expression->button_delete.bt.rect = expression->button_delete.rect_base;
    expression->button_delete.bt.is_survolable = 1;
    expression->button_delete.bt.hovered = 0;
    expression->button_delete.bt.color_base = expression->bg_color;
    expression->button_delete.bt.color_hover = (SDL_Color){255, 0, 0, 255};
    expression->button_delete.bt.radius = expression->button_delete.rect_base.w / 3;
    expression->button_delete.bt.pourcentage_place = 70;
    expression->button_delete.bt.taille_bonus_hover_x = 0;
    expression->button_delete.bt.taille_bonus_hover_y = 0;
}

void init_button_new_expression (SDL_Renderer* ren, Bande_haute* bande_haute){
    bande_haute->button_new_expression.bt.image = load_image(ren, "Icons/ajout.png");
    bande_haute->button_new_expression.bt.rect.h = bande_haute->params.taille_button_new_expression;
    bande_haute->button_new_expression.bt.rect.w = bande_haute->button_new_expression.bt.rect.h;
    bande_haute->button_new_expression.bt.rect.y = bande_haute->surface.y + bande_haute->surface.h - 1.3*bande_haute->button_new_expression.bt.rect.h;
    bande_haute->button_new_expression.bt.rect.x = bande_haute->surface.x + bande_haute->surface.w - 1.3*bande_haute->button_new_expression.bt.rect.w;
    bande_haute->button_new_expression.bt.is_survolable = 1;
    bande_haute->button_new_expression.bt.hovered = 0;
    bande_haute->button_new_expression.bt.color_base = colors->button_new_expression;
    bande_haute->button_new_expression.bt.color_hover = colors->button_new_expression_hover;
    bande_haute->button_new_expression.bt.radius = bande_haute->button_new_expression.bt.rect.w / 5;
    bande_haute->button_new_expression.bt.pourcentage_place = 110;
    bande_haute->button_new_expression.bt.taille_bonus_hover_x = 0.2 * bande_haute->button_new_expression.bt.rect.w;
    bande_haute->button_new_expression.bt.taille_bonus_hover_y = 0.2 * bande_haute->button_new_expression.bt.rect.h;
}

void init_champs_entrees (SDL_Renderer* ren, Expression_fonction* expression, Parametres_bandes_entrees params){
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
        but[j]->champs_texte->color_base = expression->bg_color_oppo;//colors->bg_champ_entree;
        but[j]->champs_texte->color_hover = but[j]->champs_texte->color_base;
        but[j]->champs_texte->color_hover.a = 150;
        but[j]->champs_texte->font_text = fonts[1];
        but[j]->champs_texte->font_text_hover = fonts[2];
        but[j]->champs_texte->taille_bonus_hover_x = 0;
        but[j]->champs_texte->taille_bonus_hover_y = 0;
        but[j]->champs_texte->radius = but[j]->position_initiale.h / 3;
        but[j]->cursorVisible = 0;
        strcpy(but[j]->text, "");
        but[j]->position_cursor = 0;//strlen(but[j]->text);
        but[j]->lastCursorToggle = SDL_GetTicks();
        expression->champs_entrees[j] = but[j];
    }
}

void init_placement_entrees (SDL_Renderer* ren, Expression_fonction* expression, Parametres_bandes_entrees params, SDL_Rect surface_bande_haut){
    expression->entree_selectionnee = SELECTION_NULL;
    expression->rect_initial.x = surface_bande_haut.x;
    expression->rect_initial.h = params.height_bande_expression;
    expression->rect_initial.y = surface_bande_haut.y + expression->numero * (expression->rect_initial.h); // pour centrer : (TAILLE_BANDE_HAUT + TAILLE_BANDE_DESCRIPTIONS - but[j]->champs_texte->rect.h)/2 ;
    expression->rect_initial.w = surface_bande_haut.w;
    expression->rect_affiche = expression->rect_initial;
    if (expression->numero % 2 == 0){
        expression->bg_color = colors->bg_bandes_expression_1;
        expression->bg_color_oppo = colors->bg_bandes_expression_2;
    } else {
        expression->bg_color = colors->bg_bandes_expression_2;
        expression->bg_color_oppo = colors->bg_bandes_expression_1;
    }

    // Bouton de déplacement inter-expressions
    init_button_deplacement(ren, expression, params);

    // Bouton pour afficher/cacher la courbe
    init_button_visibilite(ren, expression, params);
    
    // Les champs d'entrées
    init_champs_entrees(ren, expression, params);

    // La couleur :
    expression->color_picker = malloc(sizeof(Color_picker));
    *expression->color_picker = init_color_picker(ren, calcul_pos(params.width_elements, params.espace_entre_elements, NB_ELEMENTS_PAR_EXPRESSION-1) + params.taille_color_picker/2,
                                                        expression->rect_initial.y + expression->rect_initial.h/2,
                                                        params.taille_color_picker, &expression->fonction.color, classic_colors[expression->numero % NB_COULEURS_CLASSIQUES]);

    // Bouton de suppression de l'expression
    init_button_delete(ren, expression, params);
}

int calcul_pos (int tab[NB_ELEMENTS_PAR_EXPRESSION + 1], int espaces, int num_element){
    if (num_element < 1 || num_element > NB_ELEMENTS_PAR_EXPRESSION+1) return -1;
    int x = tab[0] + espaces;
    for (int i = 1; i < num_element; i++) {
        x += tab[i] + espaces;
    }
    return x;
}

void init_bande_haute (SDL_Renderer* ren, Bande_haute* bande_haute){
    bande_haute->expanding = false;
    bande_haute->scroll_offset = 0;
    bande_haute->surface.x = 0;
    bande_haute->surface.y = TAILLE_BANDE_DESCRIPTIONS;
    bande_haute->surface.w = FEN_X - TAILLE_BANDE_DROITE;
    bande_haute->surface.h = TAILLE_BANDE_HAUT - TAILLE_BANDE_DESCRIPTIONS;

    bande_haute->params.height_entrees_pourcentage = 70;
    bande_haute->params.height_bande_expression = 70;

    bande_haute->params.taille_color_picker = 0.4 * bande_haute->params.height_bande_expression;
    bande_haute->params.taille_button_deplacement = 0.5 * bande_haute->params.height_bande_expression;
    bande_haute->params.taille_button_visibilite = 0.5 * bande_haute->params.height_bande_expression;
    bande_haute->params.taille_button_delete = 0.5 * bande_haute->params.height_bande_expression;
    bande_haute->params.taille_button_new_expression = 0.4 * bande_haute->params.height_bande_expression;
    int i = 0;
    bande_haute->params.width_elements[i++] = bande_haute->surface.x;
    bande_haute->params.width_elements[i++] = bande_haute->params.taille_button_deplacement; 
    bande_haute->params.width_elements[i++] = bande_haute->params.taille_button_visibilite; 
    bande_haute->params.width_elements[i++] = WIDTH_CHAMPS_BORNES;
    bande_haute->params.width_elements[i++] = WIDTH_CHAMPS_BORNES;
    bande_haute->params.width_elements[i++] = WIDTH_CHAMP_EXPRESSION;
    bande_haute->params.width_elements[i++] = bande_haute->params.taille_color_picker;
    bande_haute->params.width_elements[i++] = bande_haute->params.taille_button_delete;
    
    bande_haute->params.height_texte_desctriptif = TAILLE_BANDE_DESCRIPTIONS - 10;
    bande_haute->params.espace_entre_elements = (FEN_X - TAILLE_BANDE_DROITE - calcul_pos(bande_haute->params.width_elements, 0, NB_ELEMENTS_PAR_EXPRESSION+1)) / (NB_ELEMENTS_PAR_EXPRESSION + 1);

    init_placement_bande_descriptive(bande_haute, bande_haute->params);

    bande_haute->nb_expressions = 0;
    for (int i = 0; i < 1; i++) {
        ajout_bande_expression(ren, bande_haute);
    }
    init_button_new_expression(ren, bande_haute);
}

void resize_bande_haut (Bande_haute* bande_haute){
    bande_haute->surface.w = FEN_X - TAILLE_BANDE_DROITE;
    bande_haute->params.espace_entre_elements = (FEN_X - TAILLE_BANDE_DROITE - calcul_pos(bande_haute->params.width_elements, 0, NB_ELEMENTS_PAR_EXPRESSION+1)) / (NB_ELEMENTS_PAR_EXPRESSION + 1);

    int num_element_du_premier_champs = 3;
    Button* but[] = {bande_haute->texte_descriptif_borne_inf, bande_haute->texte_descriptif_borne_sup, bande_haute->texte_descriptif_expression};
    int x = calcul_pos(bande_haute->params.width_elements, bande_haute->params.espace_entre_elements, num_element_du_premier_champs);
    for (int j = 0; j < 3; j++){
        but[j]->rect.x = x;
        x += bande_haute->params.width_elements[j+num_element_du_premier_champs] + bande_haute->params.espace_entre_elements;
    }
    
    for (int i = 0; i < bande_haute->nb_expressions; i++) {
        bande_haute->expressions[i]->rect_initial.w = bande_haute->surface.w;
        bande_haute->expressions[i]->rect_affiche.w = bande_haute->surface.w;

        Entree_texte* but[] = {bande_haute->expressions[i]->borne_inf, bande_haute->expressions[i]->borne_sup, bande_haute->expressions[i]->expression};
        int x = calcul_pos(bande_haute->params.width_elements, bande_haute->params.espace_entre_elements, num_element_du_premier_champs);
        for (int j = 0; j < 3; j++){
            but[j]->position_initiale.x = x;
            but[j]->champs_texte->rect.x = x;
            x += bande_haute->params.width_elements[j+num_element_du_premier_champs] + bande_haute->params.espace_entre_elements;
        }
            
        init_placement_color_picker(bande_haute->expressions[i]->color_picker);
        bande_haute->expressions[i]->color_picker->boutton_x = calcul_pos(bande_haute->params.width_elements, bande_haute->params.espace_entre_elements, NB_ELEMENTS_PAR_EXPRESSION-1) + bande_haute->params.taille_color_picker/2;
        
        bande_haute->expressions[i]->button_deplacement.rect_base.x = calcul_pos(bande_haute->params.width_elements, bande_haute->params.espace_entre_elements, 1);
        bande_haute->expressions[i]->button_deplacement.bt.rect.x = calcul_pos(bande_haute->params.width_elements, bande_haute->params.espace_entre_elements, 1);
        
        bande_haute->expressions[i]->button_visibilite.rect_base.x = calcul_pos(bande_haute->params.width_elements, bande_haute->params.espace_entre_elements, 2);
        bande_haute->expressions[i]->button_visibilite.bt.rect.x = calcul_pos(bande_haute->params.width_elements, bande_haute->params.espace_entre_elements, 2);
        
        bande_haute->expressions[i]->button_delete.rect_base.x = calcul_pos(bande_haute->params.width_elements, bande_haute->params.espace_entre_elements, NB_ELEMENTS_PAR_EXPRESSION);
        bande_haute->expressions[i]->button_delete.bt.rect.x = calcul_pos(bande_haute->params.width_elements, bande_haute->params.espace_entre_elements, NB_ELEMENTS_PAR_EXPRESSION);
    }
    bande_haute->button_new_expression.bt.rect.x = bande_haute->surface.x + bande_haute->surface.w - 1.15*bande_haute->button_new_expression.bt.rect.w;
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
        expression->borne_inf->position_cursor = strlen(expression->borne_inf->text);
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
        expression->borne_sup->position_cursor = strlen(expression->borne_sup->text);
    }
}

void execute_expression (Expression_fonction* expression){
    if (expression->expression->text[0] != '\0'){
        // TODO : A connecter avec les autres modules
        typejeton TabToken[TailleMax] = {};
        int erreur = 0;
        Analyse_Lexicale(TabToken, expression->expression->text, &erreur);
        expression->expression->position_cursor = strlen(expression->expression->text);
    }
}

void execute_champs_select_and_change_focus (Expression_fonction* expression, SelectionEntree nouvelle_entree){
    if (expression->entree_selectionnee == BORNE_INF) charge_valeur_borne_inf(expression);
    if (expression->entree_selectionnee == BORNE_SUP) charge_valeur_borne_sup(expression);
    if (expression->entree_selectionnee == EXPRESSION) execute_expression(expression);
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

void action_apres_modif_offset (Bande_haute* bande_haute){
    if (bande_haute->scroll_offset < 0) bande_haute->scroll_offset = 0;
    int max_offset = (bande_haute->nb_expressions - 1) * bande_haute->params.height_bande_expression;
    if (bande_haute->scroll_offset > max_offset) bande_haute->scroll_offset = max_offset;
    for (int i = 0; i < bande_haute->nb_expressions; i++) {
        placement_pour_affichage_avec_offset(bande_haute->expressions[i], bande_haute->scroll_offset);
        init_placement_color_picker(bande_haute->expressions[i]->color_picker);
        cacher_expression_si_nessessaire(bande_haute, bande_haute->expressions[i]);
    }

}

void cacher_expression_si_nessessaire (Bande_haute* bande_haute, Expression_fonction* expression){
    expression->visible = (expression->rect_affiche.y + expression->rect_affiche.h > bande_haute->surface.y &&
        expression->rect_affiche.y < bande_haute->surface.y + bande_haute->surface.h);
    if (!expression->visible && expression->color_picker->show_picker){
        expression->color_picker->show_picker = 0;
    }
}

void affiche_bande_haut (SDL_Renderer* ren, Bande_haute* bande_haute){
    // Fond de la bande haute des champs
    affiche_bande_arrondis_en_bas(ren, 0, TAILLE_BANDE_DESCRIPTIONS, FEN_X - TAILLE_BANDE_DROITE, bande_haute->surface.y + bande_haute->surface.h, RAYON_BAS_BANDE_HAUT, colors->bande_haute_expressions);

    // Texte affiché (ajoute un curseur clignotant)
    const char* texte_defaut[] = {"ex: -5", "ex: 5", "ex: sin(x)"};
    Entree_texte *target_champs = NULL;
    for (int j = 0; j < bande_haute->nb_expressions; j++) {
        if (bande_haute->expressions[j]->visible) {
            boxRGBA(ren, bande_haute->expressions[j]->rect_affiche.x, bande_haute->expressions[j]->rect_affiche.y, bande_haute->expressions[j]->rect_affiche.x + bande_haute->expressions[j]->rect_affiche.w, bande_haute->expressions[j]->rect_affiche.y + bande_haute->expressions[j]->rect_affiche.h, bande_haute->expressions[j]->bg_color.r, bande_haute->expressions[j]->bg_color.g, bande_haute->expressions[j]->bg_color.b, bande_haute->expressions[j]->bg_color.a);
            for (int i = 0; i < NB_ENTREES; i++) {
                target_champs = bande_haute->expressions[j]->champs_entrees[i];
                if (SDL_GetTicks() - target_champs->lastCursorToggle >= CURSOR_BLINK_TIME) {
                    target_champs->cursorVisible = !target_champs->cursorVisible;
                    target_champs->lastCursorToggle = SDL_GetTicks();
                }
                if (i != bande_haute->expressions[j]->entree_selectionnee && strcmp(target_champs->text, "") == 0) {
                    target_champs->champs_texte->label = texte_defaut[i];
                } else {
                    strcpy(target_champs->display, target_champs->text);
                    if (bande_haute->expressions[j]->entree_selectionnee == target_champs->type_entree){
                        if (target_champs->cursorVisible){
                            insert_char(target_champs->display, target_champs->position_cursor, '|');
                        } else insert_char(target_champs->display, target_champs->position_cursor, ' ');
                    }
                    target_champs->champs_texte->label = target_champs->display;
                }
                renderButton(ren, target_champs->champs_texte);
            }
            affiche_boutton_color_picker(ren, bande_haute->expressions[j]->color_picker);
            renderImageButton(ren, &bande_haute->expressions[j]->button_deplacement.bt);
            renderImageButton(ren, &bande_haute->expressions[j]->button_visibilite.bt);
            renderImageButton(ren, &bande_haute->expressions[j]->button_delete.bt);
        }
    }

    // Fond de la bande haute des descriptions
    boxRGBA(ren, 0, 0, FEN_X - TAILLE_BANDE_DROITE, TAILLE_BANDE_DESCRIPTIONS, colors->bande_haute_description.r, colors->bande_haute_description.g, colors->bande_haute_description.b, colors->bande_haute_description.a);
    // Afficher les textes descriptifs
    renderButton(ren, bande_haute->texte_descriptif_borne_inf);
    renderButton(ren, bande_haute->texte_descriptif_borne_sup);
    renderButton(ren, bande_haute->texte_descriptif_expression);
}

void free_bande_expression (Expression_fonction* expression){
    free(expression->borne_inf->champs_texte);
    free(expression->borne_inf);
    free(expression->borne_sup->champs_texte);
    free(expression->borne_sup);
    free(expression->expression->champs_texte);
    free(expression->expression);
    free(expression->color_picker);
    SDL_DestroyTexture(expression->button_deplacement.bt.image);
    SDL_DestroyTexture(expression->button_delete.bt.image);
    SDL_DestroyTexture(expression->button_visibilite.bt.image);
    SDL_DestroyTexture(expression->image_button_visible);
    SDL_DestroyTexture(expression->image_button_invisible);
    free(expression);
}

void suppr_bande_expression (Bande_haute* bande_haute, int num_expression){
    free_bande_expression(bande_haute->expressions[num_expression]);
    for (int i = num_expression; i < bande_haute->nb_expressions - 1; i++) {
        bande_haute->expressions[i] = bande_haute->expressions[i+1];
        bande_haute->expressions[i]->numero = i;
        if (bande_haute->expressions[i]->numero % 2 == 0){
            bande_haute->expressions[i]->bg_color = colors->bg_bandes_expression_1;
            bande_haute->expressions[i]->bg_color_oppo = colors->bg_bandes_expression_2;
        } else {
            bande_haute->expressions[i]->bg_color = colors->bg_bandes_expression_2;
            bande_haute->expressions[i]->bg_color_oppo = colors->bg_bandes_expression_1;
        }
        bande_haute->expressions[i]->rect_initial.y -= bande_haute->params.height_bande_expression;
        for (int j = 0; j < 3; j++) {
            bande_haute->expressions[i]->champs_entrees[j]->position_initiale.y -= bande_haute->params.height_bande_expression;
            
            bande_haute->expressions[i]->champs_entrees[j]->champs_texte->color_base = bande_haute->expressions[i]->bg_color_oppo;
            bande_haute->expressions[i]->champs_entrees[j]->champs_texte->color_hover = bande_haute->expressions[i]->champs_entrees[j]->champs_texte->color_base;
            bande_haute->expressions[i]->champs_entrees[j]->champs_texte->color_hover.a = 150;
        }
        bande_haute->expressions[i]->color_picker->boutton_y -= bande_haute->params.height_bande_expression;
        bande_haute->expressions[i]->button_deplacement.rect_base.y -= bande_haute->params.height_bande_expression;
        bande_haute->expressions[i]->button_visibilite.rect_base.y -= bande_haute->params.height_bande_expression;
        bande_haute->expressions[i]->button_delete.rect_base.y -= bande_haute->params.height_bande_expression;

        bande_haute->expressions[i]->button_deplacement.bt.color_base = bande_haute->expressions[i]->bg_color;
        bande_haute->expressions[i]->button_visibilite.bt.color_base = bande_haute->expressions[i]->bg_color;
        bande_haute->expressions[i]->button_visibilite.bt.color_hover = bande_haute->expressions[i]->bg_color_oppo;
        bande_haute->expressions[i]->button_delete.bt.color_base = bande_haute->expressions[i]->bg_color;
    }
    bande_haute->nb_expressions--;
    action_apres_modif_offset(bande_haute);
}

void ajout_bande_expression (SDL_Renderer* ren, Bande_haute* bande_haute){
    if (bande_haute->nb_expressions >= NB_EXPRESSIONS_MAX) return; // TODO : message d'erreur
    bande_haute->expressions[bande_haute->nb_expressions] = malloc(sizeof(Expression_fonction));
    bande_haute->expressions[bande_haute->nb_expressions]->numero = bande_haute->nb_expressions;
    bande_haute->expressions[bande_haute->nb_expressions]->fonction.visible = true;
    init_placement_entrees(ren, bande_haute->expressions[bande_haute->nb_expressions], bande_haute->params, bande_haute->surface);
    
    const char* nom_f[] = {"sin(x)", "cos(x)", "exp(x)", "x", "2-x"};
    typejeton x;
    x.lexem=VARIABLE;
    x.valeur.variable='x';
    //Soustraction
    typejeton Soustraction;
    Soustraction.lexem=OPERATEUR;
    Soustraction.valeur.operateur=MOINS;
    // Sinus
    typejeton Sinus;
    Sinus.lexem=FONCTION;
    Sinus.valeur.fonction=SIN;
    // Cosinus
    typejeton Cosinus;
    Cosinus.lexem=FONCTION;
    Cosinus.valeur.fonction=COS;
    // Exp
    typejeton Exp;
    Exp.lexem=FONCTION;
    Exp.valeur.fonction=EXP;
    //Deux
    typejeton Deux;
    Deux.lexem=REEL;
    Deux.valeur.reel=2;
    Node* DEUX_ARBRE = malloc(sizeof(Node));
    *DEUX_ARBRE = creation_arbre(Deux,NULL,NULL);
    Node* X_ARBRE = malloc(sizeof(Node));
    *X_ARBRE = creation_arbre(x,NULL,NULL);
    Node* COSINUS_ARBRE = malloc(sizeof(Node));
    *COSINUS_ARBRE = creation_arbre(Cosinus,X_ARBRE,NULL);
    Node* SOMME_ARBRE = malloc(sizeof(Node));
    *SOMME_ARBRE = creation_arbre(Soustraction,DEUX_ARBRE,X_ARBRE);
    Node* EXP_ARBRE = malloc(sizeof(Node));
    *EXP_ARBRE = creation_arbre(Exp,X_ARBRE,NULL);
    Node* SIN_ARBRE = malloc(sizeof(Node));
    *SIN_ARBRE = creation_arbre(Sinus,X_ARBRE,NULL);
    Node* arbres[] = {SIN_ARBRE, COSINUS_ARBRE, EXP_ARBRE, X_ARBRE, SOMME_ARBRE};
    const char* interval[][2] = {{"-4", "4"}, {"5", "10"}, {"-7", "-3"}, {"0.2", "3.5"}, {"-1e1", "2e-1"}};
    bande_haute->expressions[bande_haute->nb_expressions]->fonction.fonction_arbre = malloc(sizeof(Node));
    bande_haute->expressions[bande_haute->nb_expressions]->fonction.fonction_arbre = arbres[bande_haute->nb_expressions % 5];
    strcpy(bande_haute->expressions[bande_haute->nb_expressions]->expression->text, nom_f[bande_haute->expressions[bande_haute->nb_expressions]->numero % 5]);
    int a = nb_alea(0,4);
    strcpy(bande_haute->expressions[bande_haute->nb_expressions]->borne_inf->text, interval[a][0]);
    strcpy(bande_haute->expressions[bande_haute->nb_expressions]->borne_sup->text, interval[a][1]);
    charge_valeur_borne_inf(bande_haute->expressions[bande_haute->nb_expressions]);
    charge_valeur_borne_sup(bande_haute->expressions[bande_haute->nb_expressions]);
    bande_haute->expressions[bande_haute->nb_expressions]->borne_inf->position_cursor = strlen(bande_haute->expressions[bande_haute->nb_expressions]->borne_inf->text);
    bande_haute->expressions[bande_haute->nb_expressions]->borne_sup->position_cursor = strlen(bande_haute->expressions[bande_haute->nb_expressions]->borne_sup->text);
    bande_haute->expressions[bande_haute->nb_expressions]->expression->position_cursor = strlen(bande_haute->expressions[bande_haute->nb_expressions]->expression->text);

    bande_haute->nb_expressions++;
    action_apres_modif_offset(bande_haute);
}


