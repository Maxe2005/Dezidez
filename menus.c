#include "menus.h"

void ecran_acceuil (SDL_Renderer* ren){
    // Initialisation des boutons
    Button button_mode_emploi, button_remerciements, button_grapheur;
    Button* buttons[NB_BOUTONS_ACCUEIL];
    init_buttons_accueil(buttons, &button_mode_emploi, &button_remerciements, &button_grapheur);

    int running = 1;
    while (running) {
        SDL_SetRenderDrawColor(ren, 0, 0, 0, 255);
        SDL_RenderClear(ren);
        affiche_titre(ren);
        affiche_boutons_accueil(ren, buttons);
        updateDisplay(ren);

        handle_events_accueil(buttons, ren, &running);
    }
}

void affiche_titre (SDL_Renderer* ren){
    int pos_y = FEN_Y/3;
    // Fond de l'en-tête
    /*
    SDL_SetRenderDrawColor(ren, 50, 50, 50, 255);
    SDL_Rect header = {0, 0, FEN_X, HEADER_HEIGHT};
    SDL_RenderFillRect(ren, &header);*/

    SDL_Surface *surface = TTF_RenderUTF8_Solid(fonts[0], "Grapheur d'expressions fonctionnelles", (SDL_Color){255, 255, 255, 255});
    SDL_Texture *texture = SDL_CreateTextureFromSurface(ren, surface);
    SDL_Rect textRect = {FEN_X / 2 - surface->w / 2, pos_y / 2 - surface->h / 2, surface->w, surface->h};
    
    SDL_RenderCopy(ren, texture, NULL, &textRect);

    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
}

void init_buttons_accueil(Button* buttons[], Button* button_mode_emploi, Button* button_remerciements, Button* button_grapheur) {
    int button_height = 50;
    int button_width = 300;
    int button_margin_x = 20;
    SDL_Color color_texte = {255, 255, 255, 255};
    SDL_Color color_base = {0, 0, 255, 255};
    SDL_Color color_touch = {255, 0, 0, 255};
    
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
        but[j]->taille_bonus_hover_y = 20;
        buttons[j] = but[j];
    }
}

void affiche_boutons_accueil(SDL_Renderer* ren, Button* buttons[]) {
    for (int i = 0; i < NB_BOUTONS_ACCUEIL; i++) {
        renderButton(ren, buttons[i]);
    }
}

void handle_events_accueil(Button* buttons[], SDL_Renderer* ren, int *running) {
    SDL_Event e;
    while (SDL_PollEvent(&e)) {
        if (e.type == SDL_QUIT) {
            *running = 0;
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
                    switch (i) {
                        case 0:
                            ecran_mode_emploi(ren);
                            break;
                        case 1:
                            ecran_remerciements(ren);
                            break;
                        case 2:
                            Grapheur(ren);
                            break;
                    }
                    *running = 0;
                    return;
                }
            }
        }
        
        if (e.type == SDL_KEYUP) {
            char lancement = e.key.keysym.sym;
            switch (lancement) {
                case SDLK_SPACE:case SDLK_g:
                    Grapheur(ren); 
                    *running = 0;
                    break;
                case SDLK_r:
                    ecran_remerciements(ren);
                    *running = 0;
                    break;
                case SDLK_m:
                    ecran_mode_emploi(ren);
                    *running = 0;
                    break;
                default:
                    break;
            }
        }
    }
}


void ecran_text (SDL_Renderer* ren, const char* Text[], char* titre){
    // Texte des remerciements
    int space_entre_lignes = 20;
    int taille_ligne_y = 30;
    int margin_x = 10;

    int nb_lignes = 0;
    while (Text[nb_lignes] != NULL) {
        nb_lignes++;
    }
    Button lignes[nb_lignes];
    for (int i = 0; i < nb_lignes; i++) {
        lignes[i].rect.x = margin_x;
        lignes[i].rect.y = HEADER_HEIGHT + 40 + space_entre_lignes + i * (taille_ligne_y + space_entre_lignes);
        lignes[i].rect.w = FEN_X - 2*margin_x;
        lignes[i].rect.h = taille_ligne_y;
        lignes[i].label = Text[i];
        lignes[i].is_survolable = 0;
        lignes[i].color_text = (SDL_Color){255, 255, 255, 255};
        lignes[i].color_base = (SDL_Color){0, 0, 0, 255};
        lignes[i].font_text = fonts[1];
    }

    int scroll_offset = 0; // Décalage vertical du scrolling
    int running = 1;
    SDL_Event event;

    while (running) {
        SDL_SetRenderDrawColor(ren, 0, 0, 0, 255);
        SDL_RenderClear(ren);
        renderHeader(ren, titre);
        // Affichage du texte en tenant compte du scrolling
        for (int i = 0; i < nb_lignes; i++) {
            SDL_Rect original_rect = lignes[i].rect;
            lignes[i].rect.y -= scroll_offset; // Appliquer le scroll
            if (lignes[i].rect.y > HEADER_HEIGHT + BUTTON_MARGIN/2 && lignes[i].rect.y + lignes[i].rect.h/2 < FEN_Y) {
                renderButton(ren, &(lignes[i]));
            }
            lignes[i].rect = original_rect; // Rétablir la position originale
        }
        updateDisplay(ren);

        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) running = 0;

            if (event.type == SDL_MOUSEWHEEL) {
                scroll_offset -= event.wheel.y * SCROLL_SPEED;
            }

            if (event.type == SDL_KEYDOWN) {
                if (event.key.keysym.sym == SDLK_DOWN) scroll_offset += SCROLL_SPEED;
                if (event.key.keysym.sym == SDLK_UP) scroll_offset -= SCROLL_SPEED;
            }

            if (event.type == SDL_KEYUP) {
                if (event.key.keysym.sym == SDLK_BACKSPACE){
                    ecran_acceuil(ren);
                    running = 0;
                }
            }
        if (scroll_offset < 0) scroll_offset = 0;
        if (scroll_offset > (nb_lignes * (taille_ligne_y + space_entre_lignes)) - (FEN_Y/2 - HEADER_HEIGHT))
            scroll_offset = (nb_lignes * (taille_ligne_y + space_entre_lignes)) - (FEN_Y/2 - HEADER_HEIGHT);
        }
    }
}

void ecran_remerciements (SDL_Renderer* ren){
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
    
        ecran_text(ren, creditsText, "Remerciements");
}

void ecran_mode_emploi (SDL_Renderer* ren){
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
    
        ecran_text(ren, manuelText, "Mode d'emploi");
}
