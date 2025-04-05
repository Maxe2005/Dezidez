#include "menus.h"

void ecran_acceuil (SDL_Renderer* ren, Grapheur_elements *gr_ele, Grapheur_3D_elements *grapheur_ele_3D){
    Accueil accueil;
    // Initialisation des boutons
    Button button_mode_emploi, button_remerciements, button_grapheur, button_grapheur_3D;
    init_buttons_accueil(accueil.buttons, &button_mode_emploi, &button_remerciements, &button_grapheur, &button_grapheur_3D);

    // Chargement du fond d'ecran
    accueil.bg = malloc(sizeof(Background));
    init_background(ren, "bg2.jpg", accueil.bg);

    accueil.titre = malloc(sizeof(WrappedText));

    accueil.lang_bt = malloc(sizeof(Langues_bt));
    init_bt_langues(ren, accueil.lang_bt);

    resize_ecran_acceuil(&accueil);
    int running = 1;
    while (running) {
        SDL_SetRenderDrawColor(ren, 0, 0, 0, 255);
        SDL_RenderClear(ren);
        affiche_background(ren, accueil.bg);
        affiche_titre(ren, accueil.titre);
        affiche_boutons_accueil(ren, accueil.buttons);
        affiche_langues(ren, accueil.lang_bt);
        updateDisplay(ren);

        handle_events_accueil(ren, &accueil, &running, gr_ele, grapheur_ele_3D);
    }
    free_background(accueil.bg);
    free(accueil.titre);
}

void resize_ecran_acceuil (Accueil* accueil){
    resize_background(accueil->bg);
    resize_boutons_acceuil(accueil->buttons);
    resize_lang_bt(accueil->lang_bt);
    *accueil->titre = text_wrapper(fonts[0], get_texte("Acceuil", "titre"), FEN_X - 20);
}

void affiche_titre (SDL_Renderer* ren, WrappedText *titre){
    SDL_SetRenderDrawBlendMode(ren, SDL_BLENDMODE_BLEND);
    int marge_x_fond = 20;
    int marge_y_fond = 10;
    int pos_y = FEN_Y/6;

    int titre_width = 0;
    int w;
    Ligne_texte* current_line = &titre->fist_ligne;
    for (int i = 0; i < titre->nb_lines; i++) {
        TTF_SizeText(fonts[0], current_line->text, &w, NULL);
        if (w > titre_width) titre_width = w;
        current_line = current_line->ligne_suivante;
    }

    // Fond de l'en-tête
    SDL_SetRenderDrawColor(ren, 50, 50, 50, 200);
    SDL_Rect header = {(FEN_X - titre_width)/2 - marge_x_fond, pos_y - titre->total_height/2 - marge_y_fond, titre_width + 2*marge_x_fond, titre->total_height + 2*marge_y_fond};
    SDL_RenderFillRect(ren, &header);

    render_text_wrapped(ren, *titre, (SDL_Rect){0, 0, FEN_X, 2*pos_y}, (SDL_Color){255, 255, 255, 255}, ALIGN_CENTER_X, ALIGN_MIDDLE_Y);
}

void init_buttons_accueil(Button* buttons[], Button* button_mode_emploi, Button* button_remerciements, Button* button_grapheur, Button* button_grapheur_3D) {
    int button_height = 60;
    int button_width = 350;
    int button_margin_x = 20;
    int button_margin_y = 20;
    int marge_fen = 20;
    SDL_Color color_texte = {255, 255, 255, 255};
    SDL_Color color_base = {20, 30, 60, 200};
    SDL_Color color_touch = {0, 120, 255, 200};
    
    Button* but[] = {button_mode_emploi, button_remerciements, button_grapheur, button_grapheur_3D};
    char* id[] = {"Mode_emploi", "Remerciements", "Grapheur_2D", "Grapheur_3D"};
    for (int j = 0; j < NB_BOUTONS_ACCUEIL; j++){
        but[j]->rect.w = button_width;
        but[j]->rect.h = button_height;
        but[j]->is_survolable = 1;
        but[j]->hovered = 0;
        but[j]->label = get_texte("Acceuil", id[j]);
        but[j]->color_text = color_texte;
        but[j]->color_base = color_base;
        but[j]->color_hover = color_touch;
        but[j]->font_text = fonts[1];
        but[j]->font_text_hover = fonts[2];
        but[j]->taille_bonus_hover_x = 20;
        but[j]->taille_bonus_hover_y = 25;
        but[j]->radius = 20;
        buttons[j] = but[j];
    }
}

void resize_boutons_acceuil (Button* boutons[]){
    int button_height = 60;
    int button_width = 350;
    int button_margin_x = 20;
    int button_margin_y = 20;
    int marge_fen = 20;

    int origines_x[NB_BOUTONS_ACCUEIL];
    int nb_bts_par_ligne[NB_BOUTONS_ACCUEIL];
    nb_bts_par_ligne[0] = NB_BOUTONS_ACCUEIL;
    int k = 0;
    while (nb_bts_par_ligne[k] > 0){
        origines_x[k] = FEN_X/2 - (nb_bts_par_ligne[k] * button_width + (nb_bts_par_ligne[k] - 1) * button_margin_x)/2;
        while (origines_x[k] <= marge_fen && nb_bts_par_ligne[k] > 1){
            nb_bts_par_ligne[k]--;
            origines_x[k] = FEN_X/2 - (nb_bts_par_ligne[k] * button_width + (nb_bts_par_ligne[k] - 1) * button_margin_x)/2;
        }
        k++;
        if (k >= NB_BOUTONS_ACCUEIL) break;
        nb_bts_par_ligne[k] = NB_BOUTONS_ACCUEIL;
        for (int i = 0; i < k; i++) {
            nb_bts_par_ligne[k] -= nb_bts_par_ligne[i];
        }
    }
    for (int j = 0; j < NB_BOUTONS_ACCUEIL; j++){
        boutons[j]->rect.x = origines_x[(j / nb_bts_par_ligne[0])] + (j % nb_bts_par_ligne[0]) * (button_width + button_margin_x);
        boutons[j]->rect.y = (FEN_Y - button_height)/2 + (j / nb_bts_par_ligne[0]) * (button_height + button_margin_y);
    }
}

void affiche_boutons_accueil(SDL_Renderer* ren, Button* buttons[]) {
    for (int i = 0; i < NB_BOUTONS_ACCUEIL; i++) {
        renderButton(ren, buttons[i]);
    }
}

void handle_events_accueil(SDL_Renderer* ren, Accueil* accueil, int *running, Grapheur_elements *gr_ele, Grapheur_3D_elements *grapheur_ele_3D) {
    SDL_Event e;
    while (SDL_PollEvent(&e)) {
        if (e.type == SDL_QUIT) *running = 0;

        if (e.type == SDL_WINDOWEVENT && e.window.event == SDL_WINDOWEVENT_RESIZED) {
            FEN_X = e.window.data1;
            FEN_Y = e.window.data2;
            resize_ecran_acceuil(accueil);
        }

        if (e.type == SDL_MOUSEMOTION) {
            int x = e.motion.x, y = e.motion.y;
            for (int i = 0; i < NB_BOUTONS_ACCUEIL; i++) {
                if (x >= accueil->buttons[i]->rect.x && x <= accueil->buttons[i]->rect.x + accueil->buttons[i]->rect.w &&
                    y >= accueil->buttons[i]->rect.y && y <= accueil->buttons[i]->rect.y + accueil->buttons[i]->rect.h) {
                        accueil->buttons[i]->hovered = 1;
                } else {
                    accueil->buttons[i]->hovered = 0;
                }
            }
            if (is_souris_sur_rectangle(accueil->lang_bt->actual_lang.rect, x, y)){
                accueil->lang_bt->actual_lang.hovered = 1;
            } else accueil->lang_bt->actual_lang.hovered = 0;
        }
        
        if (e.type == SDL_MOUSEBUTTONUP) {
            int x = e.button.x, y = e.button.y;
            for (int i = 0; i < NB_BOUTONS_ACCUEIL; i++) {
                if (x >= accueil->buttons[i]->rect.x && x <= accueil->buttons[i]->rect.x + accueil->buttons[i]->rect.w &&
                    y >= accueil->buttons[i]->rect.y && y <= accueil->buttons[i]->rect.y + accueil->buttons[i]->rect.h) {
                    int mode_quitter = 1; // Si 0 on ferme la fenêtre, si 1 on reste sur le menu principal
                    switch (i) {
                        case 0:
                            mode_quitter = ecran_mode_emploi(ren);
                            break;
                        case 1:
                            mode_quitter = ecran_remerciements(ren);
                            break;
                        case 2:
                            mode_quitter = Grapheur(ren, gr_ele);
                            break;
                        case 3:
                            mode_quitter = Grapheur_3D(ren, grapheur_ele_3D);
                            break;
                    }
                    if (!mode_quitter) *running = 0;
                    accueil->buttons[i]->hovered = 0;
                    resize_ecran_acceuil(accueil);
                    return;
                }
            }
        
            if (is_souris_sur_rectangle(accueil->lang_bt->actual_lang.rect, x, y)){
                accueil->lang_bt->langues_dispo_visibles = !accueil->lang_bt->langues_dispo_visibles;
            } else accueil->lang_bt->langues_dispo_visibles = false;
            for (int j = 0; j < NB_LANGUES; j++) {
                if (is_souris_sur_rectangle(accueil->lang_bt->langues_dispo[j].rect, x, y)){
                    langue = j;
                    accueil->lang_bt->actual_lang.image = accueil->lang_bt->langues_dispo[j].image;
                    set_all_textes_by_lang(gr_ele, grapheur_ele_3D, accueil);
                }
            }
        }
        
        if (e.type == SDL_KEYUP) {
            char lancement = e.key.keysym.sym;
            int mode_quitter = 1; // Si 0 on ferme la fenêtre, si 1 on reste sur le menu principal
            switch (lancement) {
                case SDLK_SPACE:case SDLK_g:
                    mode_quitter = Grapheur(ren, gr_ele);
                    break;
                case SDLK_b:
                    mode_quitter = Grapheur_3D(ren, grapheur_ele_3D);
                    break;
                case SDLK_r:
                    mode_quitter = ecran_remerciements(ren);
                    break;
                case SDLK_m:
                    mode_quitter = ecran_mode_emploi(ren);
                    break;
                default:
                    break;
            }
            if (!mode_quitter) *running = 0;
            resize_ecran_acceuil(accueil);
        }
    }
}


void init_bt_langues (SDL_Renderer* ren, Langues_bt* lang_bt){
    lang_bt->taille = 50;
    lang_bt->espace_entre = 10;
    lang_bt->langues_dispo_visibles = false;

    char filename[30];
    sprintf(filename, "Icons/drapeaux/%s.png", get_lang_str());
    lang_bt->actual_lang.image = load_image(ren, filename);
    lang_bt->actual_lang.is_survolable = 1;
    lang_bt->actual_lang.hovered = 0;
    lang_bt->actual_lang.color_base = (SDL_Color){0,0,0,0};
    lang_bt->actual_lang.color_hover = (SDL_Color){0,0,0,0};
    lang_bt->actual_lang.pourcentage_place = 100;
    lang_bt->actual_lang.rect.w = lang_bt->taille;
    lang_bt->actual_lang.rect.h = lang_bt->actual_lang.rect.w;
    lang_bt->actual_lang.radius = lang_bt->actual_lang.rect.w / 2;
    lang_bt->actual_lang.taille_bonus_hover_x = 0.2 * lang_bt->actual_lang.rect.w;
    lang_bt->actual_lang.taille_bonus_hover_y = 0.2 * lang_bt->actual_lang.rect.h;

    int lang_actuelle = langue;
    for (int i = 0; i < NB_LANGUES; i++) {
        langue = i;
        char filename[30];
        sprintf(filename, "Icons/drapeaux/%s.png", get_lang_str());
        lang_bt->langues_dispo[i].image = load_image(ren, filename);
        lang_bt->langues_dispo[i].is_survolable = 0;
        lang_bt->langues_dispo[i].color_base = (SDL_Color){0,0,0,0};
        lang_bt->langues_dispo[i].pourcentage_place = 80;
        lang_bt->langues_dispo[i].rect.w = lang_bt->taille;
        lang_bt->langues_dispo[i].rect.h = lang_bt->langues_dispo[i].rect.w;
        lang_bt->langues_dispo[i].radius = lang_bt->langues_dispo[i].rect.w / 2;
    }
    langue = lang_actuelle;

    resize_lang_bt(lang_bt);

    lang_bt->rect_fond.x = lang_bt->langues_dispo[NB_LANGUES-1].rect.x - lang_bt->espace_entre;
    lang_bt->rect_fond.y = lang_bt->taille;
    lang_bt->rect_fond.w = (NB_LANGUES+1) * (lang_bt->taille + lang_bt->espace_entre);
    lang_bt->rect_fond.h = lang_bt->taille;
}

void resize_lang_bt (Langues_bt* lang_bt){
    lang_bt->actual_lang.rect.x = FEN_X - 2*lang_bt->actual_lang.rect.w;
    lang_bt->actual_lang.rect.y = lang_bt->actual_lang.rect.h;
    for (int i = 0; i < NB_LANGUES; i++) {
        lang_bt->langues_dispo[i].rect.x = FEN_X - 2*lang_bt->langues_dispo[i].rect.w - (i+1) * (lang_bt->taille + lang_bt->espace_entre);
        lang_bt->langues_dispo[i].rect.y = lang_bt->langues_dispo[i].rect.h;
    }
    lang_bt->rect_fond.x = lang_bt->langues_dispo[NB_LANGUES-1].rect.x - lang_bt->espace_entre;
}

void affiche_langues (SDL_Renderer* ren, Langues_bt* lang_bt){
    if (lang_bt->langues_dispo_visibles){
        roundedBoxRGBA(ren, lang_bt->rect_fond.x, lang_bt->rect_fond.y, lang_bt->rect_fond.x + lang_bt->rect_fond.w, lang_bt->rect_fond.y + lang_bt->rect_fond.h, 
            lang_bt->rect_fond.h/2, 150, 150, 150, 255);
    }
    
    renderImageButton(ren, &lang_bt->actual_lang);

    if (lang_bt->langues_dispo_visibles){
        for (int i = 0; i < NB_LANGUES; i++) {
            renderImageButton(ren, &lang_bt->langues_dispo[i]);
        }
    }
}

void set_all_textes_by_lang (Grapheur_elements *gr_ele, Grapheur_3D_elements *grapheur_ele_3D, Accueil* accueil) {
    // Page d'accueil
    char* id[] = {"Mode_emploi", "Remerciements", "Grapheur_2D", "Grapheur_3D"};
    for (int j = 0; j < NB_BOUTONS_ACCUEIL; j++){
        accueil->buttons[j]->label = get_texte("Acceuil", id[j]);
    }
    *accueil->titre = text_wrapper(fonts[0], get_texte("Acceuil", "titre"), FEN_X - 20);

    // Grapheur 2D
    // bande droite
    gr_ele->bande_droite->bouton_centrer.label = get_texte("Bande_droite", "centrer");
    // bande haute
    gr_ele->bande_haute->texte_descriptif_borne_inf->label = get_texte("Bande_haute", "borne_inf");
    gr_ele->bande_haute->texte_descriptif_borne_sup->label = get_texte("Bande_haute", "borne_sup");
    gr_ele->bande_haute->texte_descriptif_expression->label = get_texte("Bande_haute", "expression");

    // Grapheur 3D
    // bande haute
    grapheur_ele_3D->bande_haute->texte_descriptif_borne_inf->label = get_texte("Bande_haute", "borne_inf");
    grapheur_ele_3D->bande_haute->texte_descriptif_borne_sup->label = get_texte("Bande_haute", "borne_sup");
    grapheur_ele_3D->bande_haute->texte_descriptif_expression->label = get_texte("Bande_haute", "expression");

}



int ecran_text (SDL_Renderer* ren, const char* markdown_file, char* titre){
    int taille_header = 100;

    Background* bg = malloc(sizeof(Background));
    init_background(ren, "bg2.jpg", bg);
    bg->is_filtre = true;
    bg->color_filtre = (SDL_Color){50, 50, 50, 200};
    resize_background(bg);

    ImageButton bouton_retour;
    int pourcentage_du_header = 70;
    bouton_retour.image = load_image(ren, "Icons/maison.png");
    bouton_retour.rect.y = taille_header * (100 - pourcentage_du_header) / 200;
    bouton_retour.rect.x = 2*bouton_retour.rect.y;
    bouton_retour.rect.w = taille_header * pourcentage_du_header/100;
    bouton_retour.rect.h = bouton_retour.rect.w;
    bouton_retour.is_survolable = 1;
    bouton_retour.color_base = (SDL_Color){50, 50, 50, 255};
    bouton_retour.color_hover = (SDL_Color){0, 120, 255, 255};
    bouton_retour.hovered = 0;
    bouton_retour.radius =  bouton_retour.rect.w / 3;
    bouton_retour.pourcentage_place = 80;
    bouton_retour.taille_bonus_hover_x = 0;
    bouton_retour.taille_bonus_hover_y = 0;

    TTF_Font *font_text_md = createFont("Ressources/Fonts/DejaVuSans-Bold.ttf", FONT_TEXT_SIZE_MD);
    TTF_Font *font_title_1_md = createFont("Ressources/Fonts/DejaVuSans-Bold.ttf", FONT_TITRE_1_SIZE_MD);
    TTF_Font *font_title_2_md = createFont("Ressources/Fonts/DejaVuSans-Bold.ttf", FONT_TITRE_2_SIZE_MD);
    TTF_Font *font_title_3_md = createFont("Ressources/Fonts/DejaVuSans-Bold.ttf", FONT_TITRE_3_SIZE_MD);
    TTF_Font *fonts_md [NB_FONTS_MD] = {font_text_md, font_title_1_md, font_title_2_md,font_title_3_md};

    int marge = FEN_X / 7;
    MarkdownText md_txt = charge_markdown(fonts_md, markdown_file, marge);

    int taille_x_scrollbar = 20;
    int marge_x_scrollbar = taille_x_scrollbar;
    int marge_y_scrollbar = 10;
    Scrollbar scrollbar;
    init_scrollbar(&scrollbar, FEN_X - marge_x_scrollbar - taille_x_scrollbar, taille_header + marge_y_scrollbar, taille_x_scrollbar, FEN_Y - taille_header - 2*marge_y_scrollbar, md_txt.total_height);

    int mode_quitter = 0; // Les différentes façons de quitter l'ecrant texte : 0: pas quitter, 1: quitter la fenêtre, 2:quitter et revenir au menu principal 
    int running = 1;
    SDL_Event event;

    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                mode_quitter = 1; // Quitter la fenêtre
                running = 0;
            }

            if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_RESIZED) {
                FEN_X = event.window.data1;
                FEN_Y = event.window.data2;
                resize_background(bg);
                free_MarkdownText(&md_txt);
                marge = FEN_X / 7;
                md_txt = charge_markdown(fonts_md, markdown_file, marge);
                resize_scrollbar(&scrollbar, FEN_X - taille_x_scrollbar - marge_x_scrollbar, FEN_Y - taille_header - 2*marge_y_scrollbar, md_txt.total_height);
            }

            if (event.type == SDL_KEYUP) {
                if (event.key.keysym.sym == SDLK_BACKSPACE){
                    mode_quitter = 2; // On revient à l'écran d'accueil
                    running = 0;
                }
            }
        
            if (event.type == SDL_MOUSEBUTTONUP){
                if (is_souris_sur_rectangle(bouton_retour.rect, event.motion.x, event.motion.y)){
                    mode_quitter = 2; // On revient à l'écran d'accueil
                    running = 0;
                }
            }

            if (event.type == SDL_MOUSEMOTION){
                if (is_souris_sur_rectangle(bouton_retour.rect, event.motion.x, event.motion.y)){
                    bouton_retour.hovered = 1;
                    bouton_retour.pourcentage_place = 60;
                } else {
                    bouton_retour.hovered = 0;
                    bouton_retour.pourcentage_place = 80;
                }
            }
        
            handle_scrollbar_event(&scrollbar, &event);
        }        
        SDL_SetRenderDrawColor(ren, 0, 0, 0, 255);
        SDL_RenderClear(ren);
        affiche_background(ren, bg);
        render_markdown(ren, &md_txt, scrollbar.scroll_offset - taille_header - 0.2*taille_header, marge);
        render_scrollbar(ren, &scrollbar);
        renderHeader(ren, titre, fonts[3], FEN_X, taille_header);
        renderImageButton(ren, &bouton_retour);
        updateDisplay(ren);
    }
    for (int i = 0; i < NB_FONTS_MD; i++) {
        TTF_CloseFont(fonts_md[i]);
    }
    free_MarkdownText(&md_txt);
    SDL_DestroyTexture(bouton_retour.image);
    free(bg);
    return mode_quitter - 1;
}

void init_scrollbar(Scrollbar* sb, int x, int y, int width, int height, int total_height) {
    sb->x = x;
    sb->y = y;
    sb->width = width;
    sb->height = height;
    sb->total_content_height = total_height;
    sb->scroll_offset = 0;
    sb->scroll_speed = 30;
    sb->grabbed = 0;
    sb->click_offset_y = 0;
    resize_scrollbar(sb, x, height, total_height);
}

void render_scrollbar(SDL_Renderer* ren, Scrollbar* sb) {
    roundedBoxRGBA(ren, sb->x, sb->y, sb->x + sb->width, sb->y + sb->height, sb->width / 2 -2, 100, 100, 100, 255);

    float scroll_ratio = (float)sb->scroll_offset / sb->max_scroll;
    if (scroll_ratio > 1.0f) scroll_ratio = 1.0f;

    sb->thumb.y = sb->y + scroll_ratio * sb->thumb_travel;

    roundedBoxRGBA(ren, sb->thumb.x, sb->thumb.y, sb->thumb.x + sb->thumb.w, sb->thumb.y + sb->thumb.h, sb->thumb.w / 2 -2, 200, 200, 200, 255);
}

void handle_scrollbar_event(Scrollbar* sb, SDL_Event* event) {
    if (event->type == SDL_MOUSEBUTTONDOWN && event->button.button == SDL_BUTTON_LEFT) {
        if (SDL_PointInRect(&(SDL_Point){event->button.x, event->button.y}, &sb->thumb)) {
            sb->grabbed = 1;
            sb->click_offset_y = event->button.y - sb->thumb.y;
        }
    }

    if (event->type == SDL_MOUSEBUTTONUP && event->button.button == SDL_BUTTON_LEFT) {
        sb->grabbed = 0;
    }

    if (event->type == SDL_MOUSEMOTION && sb->grabbed) {
        int new_thumb_y = event->motion.y - sb->click_offset_y;
        if (new_thumb_y < sb->y) new_thumb_y = sb->y;
        if (new_thumb_y > sb->y + sb->height - sb->thumb.h)
            new_thumb_y = sb->y + sb->height - sb->thumb.h;

        float scroll_ratio = (float)(new_thumb_y - sb->y) / sb->thumb_travel;
        sb->scroll_offset = scroll_ratio * sb->max_scroll;
    }

    if (event->type == SDL_MOUSEWHEEL) {
        sb->scroll_offset -= event->wheel.y * sb->scroll_speed;
        if (sb->scroll_offset < 0) sb->scroll_offset = 0;
        if (sb->scroll_offset > sb->max_scroll) sb->scroll_offset = sb->max_scroll;
    }

    if (event->type == SDL_KEYDOWN) {
        if (event->key.keysym.sym == SDLK_DOWN) {
            sb->scroll_offset += sb->scroll_speed;
            if (sb->scroll_offset > sb->max_scroll) sb->scroll_offset = sb->max_scroll;
        }
        if (event->key.keysym.sym == SDLK_UP) {
            sb->scroll_offset -= sb->scroll_speed;
            if (sb->scroll_offset < 0) sb->scroll_offset = 0;
        }
    }
}

void resize_scrollbar(Scrollbar* sb, int x, int height, int total_height) {
    sb->x = x;
    sb->height = height;
    sb->total_content_height = total_height;

    float ratio = (float)sb->height / sb->total_content_height;
    if (ratio > 1.0f) ratio = 1.0f;

    int thumb_height = sb->height * ratio;
    sb->max_scroll = sb->total_content_height - sb->height/2;
    sb->thumb_travel = sb->height - thumb_height;
    if (sb->thumb_travel < 1) sb->thumb_travel = 1;

    float scroll_ratio = (float)sb->scroll_offset / sb->max_scroll;
    if (scroll_ratio > 1.0f) scroll_ratio = 1.0f;

    int thumb_y = sb->y + scroll_ratio * sb->thumb_travel;
    sb->thumb = (SDL_Rect){ sb->x, thumb_y, sb->width, thumb_height };
}


int ecran_remerciements (SDL_Renderer* ren){
    char filename[60];
    sprintf(filename, "Ressources/Md_files/Remerciements_%s.md", get_lang_str());
    int a = ecran_text(ren, filename, get_texte("Acceuil", "Titre_page_remerciement"));
    return a;
}

int ecran_mode_emploi (SDL_Renderer* ren){
    char filename[60];
    sprintf(filename, "Ressources/Md_files/Mode_emploi_%s.md", get_lang_str());
    int a = ecran_text(ren, filename, get_texte("Acceuil", "Titre_page_mode_emploi"));
    return a;
}


void init_background (SDL_Renderer* ren, const char* filename, Background* bg){
    char path[100] = "Background/";
    strcat(path, filename);
    // Chargement de l'image
    bg->backgroundTexture = load_image(ren, path);
    if (bg->backgroundTexture == NULL) {
        printf("Problème chargement image de fond");
        return;
    }

    // Obtenez les dimensions de la texture
    SDL_QueryTexture(bg->backgroundTexture, NULL, NULL, &bg->textureWidth, &bg->textureHeight);
    bg->is_filtre = false;
}

void resize_background (Background* bg){
    if ((float)FEN_X / FEN_Y < (float)bg->textureWidth / bg->textureHeight) {
        // La fenêtre est plus large par rapport à la texture
        bg->dstRect.h = FEN_Y;
        bg->dstRect.w = (FEN_Y * bg->textureWidth) / bg->textureHeight;
        bg->dstRect.x = (FEN_X - bg->dstRect.w) / 2;
        bg->dstRect.y = 0;
    } else {
        // La fenêtre est plus haute par rapport à la texture
        bg->dstRect.w = FEN_X;
        bg->dstRect.h = (FEN_X * bg->textureHeight) / bg->textureWidth;
        bg->dstRect.x = 0;
        bg->dstRect.y = (FEN_Y - bg->dstRect.h) / 2;
    }
    if (bg->is_filtre){
        bg->filtre.x = 0;
        bg->filtre.y = 0;
        bg->filtre.w = FEN_X;
        bg->filtre.h = FEN_Y;
    }
}

void affiche_background (SDL_Renderer* ren, Background* bg){
    SDL_RenderCopy(ren, bg->backgroundTexture, NULL, &bg->dstRect);
    if (bg->is_filtre){
        SDL_SetRenderDrawBlendMode(ren, SDL_BLENDMODE_BLEND);
        SDL_SetRenderDrawColor(ren, bg->color_filtre.r, bg->color_filtre.g, bg->color_filtre.b, bg->color_filtre.a);
        SDL_RenderFillRect(ren, &bg->filtre);
    }
}

void free_background (Background* bg){
    SDL_DestroyTexture(bg->backgroundTexture);
    free(bg);
}

