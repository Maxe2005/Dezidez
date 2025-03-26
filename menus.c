#include "menus.h"

void ecran_acceuil (SDL_Renderer* ren, Grapheur_elements *gr_ele){
    // Initialisation des boutons
    Button button_mode_emploi, button_remerciements, button_grapheur;
    Button* buttons[NB_BOUTONS_ACCUEIL];
    init_buttons_accueil(buttons, &button_mode_emploi, &button_remerciements, &button_grapheur);

    // Chargement du fond d'ecran
    Background* bg = malloc(sizeof(Background));
    init_background(ren, "bg2.jpg", bg);
    resize_background(bg);

    int running = 1;
    while (running) {
        SDL_SetRenderDrawColor(ren, 0, 0, 0, 255);
        SDL_RenderClear(ren);
        affiche_background(ren, bg);
        affiche_titre(ren);
        affiche_boutons_accueil(ren, buttons);
        updateDisplay(ren);

        handle_events_accueil(buttons, ren, bg, &running, gr_ele);
    }
    free_background(bg);
}

void affiche_titre (SDL_Renderer* ren){
    SDL_SetRenderDrawBlendMode(ren, SDL_BLENDMODE_BLEND);
    int marge_x_fond = 20;
    int marge_y_fond = 10;
    int pos_y = FEN_Y/3;

    SDL_Surface *surface = TTF_RenderUTF8_Solid(fonts[0], "Grapheur d'expressions fonctionnelles", (SDL_Color){255, 255, 255, 255});
    SDL_Texture *texture = SDL_CreateTextureFromSurface(ren, surface);
    SDL_Rect textRect = {FEN_X / 2 - surface->w / 2, pos_y / 2 - surface->h / 2, surface->w, surface->h};
    
    // Fond de l'en-tête
    SDL_SetRenderDrawColor(ren, 50, 50, 50, 200);
    SDL_Rect header = {textRect.x - marge_x_fond, textRect.y - marge_y_fond, textRect.w + 2*marge_x_fond, textRect.h + 2*marge_y_fond};
    SDL_RenderFillRect(ren, &header);

    SDL_RenderCopy(ren, texture, NULL, &textRect);

    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
}

void init_buttons_accueil(Button* buttons[], Button* button_mode_emploi, Button* button_remerciements, Button* button_grapheur) {
    int button_height = 60;
    int button_width = 300;
    int button_margin_x = 20;
    SDL_Color color_texte = {255, 255, 255, 255};
    SDL_Color color_base = {20, 30, 60, 200};
    SDL_Color color_touch = {0, 120, 255, 200};
    
    Button* but[] = {button_mode_emploi, button_remerciements, button_grapheur};
    char* noms[] = {"Mode d'emploi", "Remerciements", "Grapheur"};
    int origine_x = FEN_X/2 - (NB_BOUTONS_ACCUEIL * button_width + (NB_BOUTONS_ACCUEIL - 1) * button_margin_x)/2;
    for (int j = 0; j < NB_BOUTONS_ACCUEIL; j++){
        but[j]->rect.x = origine_x + j * (button_width + button_margin_x);
        but[j]->rect.y = (FEN_Y - button_height)/2;
        but[j]->rect.w = button_width;
        but[j]->rect.h = button_height;
        but[j]->is_survolable = 1;
        but[j]->hovered = 0;
        but[j]->label = noms[j];
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
    int origine_x = FEN_X/2 - (NB_BOUTONS_ACCUEIL * button_width + (NB_BOUTONS_ACCUEIL - 1) * button_margin_x)/2;
    for (int i = 0; i < NB_BOUTONS_ACCUEIL; i++) {
        boutons[i]->rect.x = origine_x + i * (button_width + button_margin_x);
        boutons[i]->rect.y = (FEN_Y - button_height)/2;
    }
}

void affiche_boutons_accueil(SDL_Renderer* ren, Button* buttons[]) {
    for (int i = 0; i < NB_BOUTONS_ACCUEIL; i++) {
        renderButton(ren, buttons[i]);
    }
}

void handle_events_accueil(Button* buttons[], SDL_Renderer* ren, Background* bg, int *running, Grapheur_elements *gr_ele) {
    SDL_Event e;
    while (SDL_PollEvent(&e)) {
        if (e.type == SDL_QUIT) *running = 0;

        if (e.type == SDL_WINDOWEVENT && e.window.event == SDL_WINDOWEVENT_RESIZED) {
            FEN_X = e.window.data1;
            FEN_Y = e.window.data2;
            resize_background(bg);
            resize_boutons_acceuil(buttons);
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
                            ecran_mode_emploi(ren);
                            break;
                        case 1:
                            ecran_remerciements(ren);
                            break;
                        case 2:
                            mode_quitter = Grapheur(ren, gr_ele);
                            break;
                    }
                    if (!mode_quitter) *running = 0;
                    resize_background(bg);
                    resize_boutons_acceuil(buttons);
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
            resize_background(bg);
            resize_boutons_acceuil(buttons);
        }
    }
}



int ecran_text (SDL_Renderer* ren, const char* markdown_file, char* titre){
    Background* bg = malloc(sizeof(Background));
    init_background(ren, "bg2.jpg", bg);
    bg->is_filtre = true;
    bg->color_filtre = (SDL_Color){50, 50, 50, 200};
    resize_background(bg);

    ImageButton bouton_retour;
    int pourcentage_du_header = 70;
    bouton_retour.image = load_image(ren, "Icons/maison.png");
    bouton_retour.rect.y = HEADER_HEIGHT * (100 - pourcentage_du_header) / 200;
    bouton_retour.rect.x = 2*bouton_retour.rect.y;
    bouton_retour.rect.w = HEADER_HEIGHT * pourcentage_du_header/100;
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
    TTF_Font *fonts_md [NB_FONTS_MD] = {font_text_md, font_title_1_md, font_title_2_md};

    MarkdownText md_txt = charge_markdown(fonts_md, markdown_file);

    int taille_header = 100;
    int scroll_offset_min = -40; // Décalage vertical minimum du scrolling
    int scroll_offset = scroll_offset_min/2; // Décalage vertical du scrolling
    int mode_quitter = 0; // Les différentes façons de quitter l'ecrant texte : 0: pas quitter, 1: quitter la fenêtre, 2:quitter et revenir au menu principal 
    int running = 1;
    SDL_Event event;

    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                mode_quitter = 0; // Quitter la fenêtre
                running = 0;
            }

            if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_RESIZED) {
                FEN_X = event.window.data1;
                FEN_Y = event.window.data2;
                resize_background(bg);
                free_MarkdownText(&md_txt);
                md_txt = charge_markdown(fonts_md, markdown_file);
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
                    mode_quitter = 1; // On revient à l'écran d'accueil
                    running = 0;
                }
            }
        
            if (event.type == SDL_MOUSEBUTTONUP){
                if (is_souris_sur_rectangle(bouton_retour.rect, event.motion.x, event.motion.y)){
                    mode_quitter = 1; // On revient à l'écran d'accueil
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
        render_markdown(ren, &md_txt, scroll_offset - taille_header);
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
    return mode_quitter;
}

int ecran_remerciements (SDL_Renderer* ren){
    int a = ecran_text(ren, "Ressources/Md_files/Remerciements.md", "Remerciements");
    return a;
}

int ecran_mode_emploi (SDL_Renderer* ren){
    int a = ecran_text(ren, "Ressources/Md_files/Mode_emploi.md", "Mode d'emploi");
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

