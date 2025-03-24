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
        }
    }
}



int ecran_text (SDL_Renderer* ren, const char* Text[], char* titre){
    // Texte des remerciements
    int space_entre_lignes = 20;
    int taille_ligne_y = 30;

    int nb_lignes = 0;
    while (Text[nb_lignes] != NULL) {
        nb_lignes++;
    }
    Button lignes[nb_lignes];
    for (int i = 0; i < nb_lignes; i++) {
        lignes[i].rect.x = 0;
        lignes[i].rect.y = HEADER_HEIGHT + 40 + space_entre_lignes + i * (taille_ligne_y + space_entre_lignes);
        lignes[i].rect.w = FEN_X;
        lignes[i].rect.h = taille_ligne_y;
        lignes[i].label = Text[i];
        lignes[i].is_survolable = 0;
        lignes[i].color_text = (SDL_Color){255, 255, 255, 255};
        lignes[i].color_base = (SDL_Color){50, 50, 50, 0};
        lignes[i].font_text = fonts[1];
        lignes[i].radius = 0;
    }

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

    int scroll_offset = 0; // Décalage vertical du scrolling
    int mode_quitter = 0; // Les différentes façons de quitter l'ecrant texte : 0: pas quitter, 1: quitter la fenêtre, 2:quitter et revenir au menu principal 
    int running = 1;
    SDL_Event event;

    while (running) {
        SDL_SetRenderDrawColor(ren, 0, 0, 0, 255);
        SDL_RenderClear(ren);
        affiche_background(ren, bg);
        renderHeader(ren, titre);
        renderImageButton(ren, &bouton_retour);
        // Affichage du texte en tenant compte du scrolling
        for (int i = 0; i < nb_lignes; i++) {
            SDL_Rect original_rect = lignes[i].rect;
            lignes[i].rect.y -= scroll_offset; // Appliquer le scroll
            if (lignes[i].rect.y > HEADER_HEIGHT + BUTTON_MARGIN/2 && lignes[i].rect.y + lignes[i].rect.h/2 < FEN_Y && strcmp(lignes[i].label, " ") != 0) {
                renderButton(ren, &(lignes[i]));
            }
            lignes[i].rect = original_rect; // Rétablir la position originale
        }
        updateDisplay(ren);

        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) return 1; // Quitter la fenêtre

            if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_RESIZED) {
                FEN_X = event.window.data1;
                FEN_Y = event.window.data2;
                resize_background(bg);
            }

            if (event.type == SDL_MOUSEWHEEL) {
                scroll_offset -= event.wheel.y * SCROLL_SPEED;
            }

            if (event.type == SDL_KEYDOWN) {
                if (event.key.keysym.sym == SDLK_DOWN) scroll_offset += SCROLL_SPEED;
                if (event.key.keysym.sym == SDLK_UP) scroll_offset -= SCROLL_SPEED;
            }

            if (event.type == SDL_KEYUP) {
                if (event.key.keysym.sym == SDLK_BACKSPACE){
                    running = 0;
                    return 1; // On revient à l'écran d'accueil
                }
            }
        
            if (event.type == SDL_MOUSEBUTTONUP){
                if (is_souris_sur_rectangle(bouton_retour.rect, event.motion.x, event.motion.y)){
                    running = 0;
                    return 1; // On revient à l'écran d'accueil
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
        if (scroll_offset < 0) scroll_offset = 0;
        if (scroll_offset > (nb_lignes * (taille_ligne_y + space_entre_lignes)) - (FEN_Y/2 - HEADER_HEIGHT))
            scroll_offset = (nb_lignes * (taille_ligne_y + space_entre_lignes)) - (FEN_Y/2 - HEADER_HEIGHT);
        
    }
}

int ecran_remerciements (SDL_Renderer* ren){
    const char* creditsText[] = {"Merci à tous ceux qui ont contribué ...",
        " ",
        " ",
        "Initiative du projet : M. José Mennesson",
        " ",
        " ",
        "Développeurs :",
        " ",
        "Maxence CHOISEL",
        "Arthur COPIN",
        "Mohammed DAIB",
        " ",
        " ",
        "Chef de projet : Maxence CHOISEL",
        " ",
        "Game Design : d'après le jeu original Pac-Man",
        " ",
        "Développeur moteur du jeu : Maxence CHOISEL",
        " ",
        "Développeur IA des fantômes : Maxence CHOISEL",
        " ",
        "Développeur interface utilisateur  : Arthur COPIN",
        " ",
        "Développeur gestion du son  : Arthur COPIN",
        " ",
        "Développeur système d’animation  : Mohammed DAIB",
        " ",
        "Développeur physique du jeu : Maxence CHOISEL",
        " ",
        "Développeur des cartes (level design) : ","Maxence CHOISEL",
        " ",
        " ",
        "Designer graphique  : Mohammed DAIB",
        " ",
        "Animateur 2D  : Mohammed DAIB",
        " ",
        "Créateur d’effets visuels (VFX)  : Mohammed DAIB",
        " ",
        " ",
        "Testeur Quality Assurance : Maxence CHOISEL",
        " ",
        "Développeur optimisation des performances :","Maxence CHOISEL",
        " ",
        NULL};
    
    int a = ecran_text(ren, creditsText, "Remerciements");
    return a;
}

int ecran_mode_emploi (SDL_Renderer* ren){
    const char* manuelText[] = {"Voici le manuel d'utilisation de ce",
        "grapheur d'expressions fonctionnelles",
        " ",
        " ",
        "Initiative du projet : M. José Mennesson",
        " ",
        " ",
        "Développeurs :",
        " ",
        "Maxence CHOISEL",
        "Arthur COPIN",
        "Mohammed DAIB",
        " ",
        " ",
        "Chef de projet : Maxence CHOISEL",
        " ",
        "Game Design : d'après le jeu original Pac-Man",
        " ",
        "Développeur moteur du jeu : Maxence CHOISEL",
        " ",
        "Développeur IA des fantômes : Maxence CHOISEL",
        " ",
        "Développeur interface utilisateur  : Arthur COPIN",
        " ",
        "Développeur gestion du son  : Arthur COPIN",
        " ",
        "Développeur système d’animation  : Mohammed DAIB",
        " ",
        "Développeur physique du jeu : Maxence CHOISEL",
        " ",
        "Développeur des cartes (level design) : ","Maxence CHOISEL",
        " ",
        " ",
        "Designer graphique  : Mohammed DAIB",
        " ",
        "Animateur 2D  : Mohammed DAIB",
        " ",
        "Créateur d’effets visuels (VFX)  : Mohammed DAIB",
        " ",
        " ",
        "Testeur Quality Assurance : Maxence CHOISEL",
        " ",
        "Développeur optimisation des performances :","Maxence CHOISEL",
        " ",
        NULL};
    
    int a = ecran_text(ren, manuelText, "Mode d'emploi");
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

