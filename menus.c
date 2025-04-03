#include "menus.h"

void ecran_acceuil (SDL_Renderer* ren, Grapheur_elements *gr_ele, Grapheur_3D_elements *grapheur_ele_3D){
    // Initialisation des boutons
    Button button_mode_emploi, button_remerciements, button_grapheur, button_grapheur_3D;
    Button* buttons[NB_BOUTONS_ACCUEIL];
    init_buttons_accueil(buttons, &button_mode_emploi, &button_remerciements, &button_grapheur, &button_grapheur_3D);

    // Chargement du fond d'ecran
    Background* bg = malloc(sizeof(Background));
    init_background(ren, "bg2.jpg", bg);

    WrappedText *titre = malloc(sizeof(WrappedText));

    resize_ecran_acceuil(buttons, bg, titre);
    int running = 1;
    while (running) {
        SDL_SetRenderDrawColor(ren, 0, 0, 0, 255);
        SDL_RenderClear(ren);
        affiche_background(ren, bg);
        affiche_titre(ren, titre);
        affiche_boutons_accueil(ren, buttons);
        updateDisplay(ren);

        handle_events_accueil(buttons, ren, bg, titre, &running, gr_ele, grapheur_ele_3D);
    }
    free_background(bg);
    free(titre);
}

void resize_ecran_acceuil (Button* buttons[], Background* bg, WrappedText *titre){
    resize_background(bg);
    resize_boutons_acceuil(buttons);
    *titre = text_wrapper(fonts[0], get_texte("Acceuil", "titre"), FEN_X - 20);
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
    int button_width = 300;
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
    int button_width = 300;
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

void handle_events_accueil(Button* buttons[], SDL_Renderer* ren, Background* bg, WrappedText *titre, int *running, Grapheur_elements *gr_ele, Grapheur_3D_elements *grapheur_ele_3D) {
    SDL_Event e;
    while (SDL_PollEvent(&e)) {
        if (e.type == SDL_QUIT) *running = 0;

        if (e.type == SDL_WINDOWEVENT && e.window.event == SDL_WINDOWEVENT_RESIZED) {
            FEN_X = e.window.data1;
            FEN_Y = e.window.data2;
            resize_ecran_acceuil(buttons, bg, titre);
        }

        if (e.type == SDL_MOUSEMOTION) {
            int x = e.motion.x, y = e.motion.y;
            for (int i = 0; i < NB_BOUTONS_ACCUEIL; i++) {
                if (x >= buttons[i]->rect.x && x <= buttons[i]->rect.x + buttons[i]->rect.w &&
                    y >= buttons[i]->rect.y && y <= buttons[i]->rect.y + buttons[i]->rect.h) {
                    buttons[i]->hovered = 1;
                } else {
                    buttons[i]->hovered = 0;
                }
            }
        }
        
        if (e.type == SDL_MOUSEBUTTONUP) {
            int x = e.button.x, y = e.button.y;
            for (int i = 0; i < NB_BOUTONS_ACCUEIL; i++) {
                if (x >= buttons[i]->rect.x && x <= buttons[i]->rect.x + buttons[i]->rect.w &&
                    y >= buttons[i]->rect.y && y <= buttons[i]->rect.y + buttons[i]->rect.h) {
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
                    buttons[i]->hovered = 0;
                    resize_ecran_acceuil(buttons, bg, titre);
                    return;
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
            resize_ecran_acceuil(buttons, bg, titre);
        }
    }
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

    
    int scroll_offset_min = -40; // Décalage vertical minimum du scrolling
    int scroll_offset = scroll_offset_min/2; // Décalage vertical du scrolling
    int SCROLL_SPEED = 30; // Vitesse de défilement
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
            }

            if (event.type == SDL_MOUSEWHEEL) {
                scroll_offset -= event.wheel.y * SCROLL_SPEED;
                if (scroll_offset < scroll_offset_min) scroll_offset = scroll_offset_min;
                if (scroll_offset > md_txt.total_height - md_txt.fist_line.wrapped_text.total_height) scroll_offset = md_txt.total_height - md_txt.fist_line.wrapped_text.total_height;
            }

            if (event.type == SDL_KEYDOWN) {
                if (event.key.keysym.sym == SDLK_DOWN) scroll_offset += SCROLL_SPEED;
                if (event.key.keysym.sym == SDLK_UP) scroll_offset -= SCROLL_SPEED;
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
        }
        
        SDL_SetRenderDrawColor(ren, 0, 0, 0, 255);
        SDL_RenderClear(ren);
        affiche_background(ren, bg);
        render_markdown(ren, &md_txt, scroll_offset - taille_header, marge);
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


void set_all_textes_by_lang (Grapheur_elements *gr_ele, Grapheur_3D_elements *grapheur_ele_3D, Button* buttons[], WrappedText *titre) {
    // Page d'accueil
    char* id[] = {"Mode_emploi", "Remerciements", "Grapheur_2D", "Grapheur_3D"};
    for (int j = 0; j < NB_BOUTONS_ACCUEIL; j++){
        buttons[j]->label = get_texte("Acceuil", id[j]);
    }
    *titre = text_wrapper(fonts[0], get_texte("Acceuil", "titre"), FEN_X - 20);

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
