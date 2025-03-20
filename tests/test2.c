#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <string.h>
#include <stdlib.h>

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define TAILLE_INTER_PARAHRAPHE 30
#define MARGE 20

typedef enum { ALIGN_LEFT, ALIGN_CENTER, ALIGN_RIGHT } TextAlignX;
typedef enum { ALIGN_TOP, ALIGN_MIDDLE, ALIGN_BOTTOM } TextAlignY;

// Fonction pour afficher du texte centré dans un rectangle avec gestion du retour à la ligne
void render_text_wrapped(SDL_Renderer *renderer, TTF_Font *font, const char *text, SDL_Rect rect, SDL_Color color) {
    int space_width, word_width, word_height;
    TTF_SizeText(font, " ", &space_width, NULL); // Largeur d'un espace

    char buffer[1024];  // Ligne de texte en cours
    buffer[0] = '\0';

    char *text_copy = strdup(text); // Copie du texte pour strtok
    char *word = strtok(text_copy, " "); // Découper en mots
    
    int x = rect.x;
    int line_count = 0, total_height = 0;
    int line_heights[100]; // Stocker les hauteurs de chaque ligne
    char *lines[100]; // Stocker chaque ligne rendue

    while (word) {
        if (strcmp(word, "\n") == 0) { // Retour à la ligne forcé par le '\n'
            if (buffer[0] == '\0') {
                line_heights[line_count-1] += TAILLE_INTER_PARAHRAPHE;
                total_height += TAILLE_INTER_PARAHRAPHE;
            } else {
                buffer[strlen(buffer)-1] = '\0'; // Supprimer l'espace inutile car pas de mot suivant
                lines[line_count] = strdup(buffer);
                line_heights[line_count] = word_height;
                total_height += word_height;
                line_count++;

                buffer[0] = '\0';  // Réinitialiser la ligne
            }
            x = rect.x;
        } else {
            TTF_SizeText(font, word, &word_width, &word_height);

            if (x + word_width > rect.x + rect.w - 2*MARGE) { // Retour à la ligne si ligne trop grande
                if (buffer[0] == '\0'){
                    printf("Attention ! La longeur du rectangle dans lequel doit être affiché le texte est trop petite par rapport à la taille d'un (ou plusieurs) mot(s) du texte (faire attention à la taille de la font !)\nLe texte n'a donc pas été affiché !\n");
                    return;
                }
                buffer[strlen(buffer)-1] = '\0'; // Supprimer l'espace inutile car pas de mot suivant
                lines[line_count] = strdup(buffer);
                line_heights[line_count] = word_height;
                total_height += word_height;
                line_count++;

                buffer[0] = '\0';  // Réinitialiser la ligne
                x = rect.x;
            }

            strcat(buffer, word);
            strcat(buffer, " ");
            x += word_width + space_width;
        }
        word = strtok(NULL, " ");
    }

    if (buffer[0] != '\0') {  // Ajouter la dernière ligne
        lines[line_count] = strdup(buffer);
        line_heights[line_count] = word_height;
        total_height += word_height;
        line_count++;
    }

    // Calculer le y de départ pour centrer verticalement
    int start_y = rect.y + (rect.h - total_height) / 2;
    if (start_y < rect.y){
        printf("Attention ! La hauteur du rectangle dans lequel le texte doit être affiché est trop petite par rapport à la taille du paragraphe ! (paragraphe trop long !)\n");
    }

    // Afficher chaque ligne de texte centrée
    int y = start_y;
    for (int i = 0; i < line_count; i++) {
        SDL_Surface *surface = TTF_RenderUTF8_Blended(font, lines[i], color);
        SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_Rect dst = {rect.x + (rect.w - surface->w) / 2, y, surface->w, surface->h};
        SDL_RenderCopy(renderer, texture, NULL, &dst);
        SDL_FreeSurface(surface);
        SDL_DestroyTexture(texture);
        y += line_heights[i];

        free(lines[i]); // Nettoyage mémoire
    }

    free(text_copy);
}


void render_text_wrapped_2(SDL_Renderer *renderer, TTF_Font *font, const char *text, SDL_Rect rect, SDL_Color color, TextAlignX align_x, TextAlignY align_y) {
    int space_width, word_width, word_height;
    TTF_SizeText(font, " ", &space_width, NULL);
    char buffer[1024] = "";
    char *text_copy = strdup(text);
    char *word = strtok(text_copy, " ");
    int x = rect.x, y = rect.y;
    int total_height = 0, line_count = 0;
    char *lines[100];
    int line_heights[100];
    
    while (word) {
        if (strcmp(word, "\n") == 0) { // Retour à la ligne forcé par le '\n'
            if (buffer[0] == '\0') {
                line_heights[line_count-1] += TAILLE_INTER_PARAHRAPHE;
                total_height += TAILLE_INTER_PARAHRAPHE;
            } else {
                buffer[strlen(buffer)-1] = '\0'; // Supprimer l'espace inutile car pas de mot suivant
                lines[line_count] = strdup(buffer);
                line_heights[line_count] = word_height;
                total_height += word_height;
                line_count++;

                buffer[0] = '\0';  // Réinitialiser la ligne
            }
            x = rect.x;
        } else {
            TTF_SizeText(font, word, &word_width, &word_height);
            if (x + word_width > rect.x + rect.w - 2 * MARGE) {
                if (buffer[0] == '\0'){
                    printf("Attention ! La longeur du rectangle dans lequel doit être affiché le texte est trop petite par rapport à la taille d'un (ou plusieurs) mot(s) du texte (faire attention à la taille de la font !)\nLe texte n'a donc pas été affiché !\n");
                    return;
                }
                buffer[strlen(buffer) - 1] = '\0';
                lines[line_count] = strdup(buffer);
                line_heights[line_count] = word_height;
                total_height += word_height;
                line_count++;
                buffer[0] = '\0';
                x = rect.x;
            }
            strcat(buffer, word);
            strcat(buffer, " ");
            x += word_width + space_width;
        }
        word = strtok(NULL, " ");
    }
    if (buffer[0] != '\0') {
        lines[line_count] = strdup(buffer);
        line_heights[line_count] = word_height;
        total_height += word_height;
        line_count++;
    }
    if (align_y == ALIGN_MIDDLE) y += (rect.h - total_height) / 2;
    else if (align_y == ALIGN_BOTTOM) y += rect.h - total_height;
    
    for (int i = 0; i < line_count; i++) {
        SDL_Surface *surface = TTF_RenderUTF8_Blended(font, lines[i], color);
        SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_Rect dst = {rect.x, y, surface->w, surface->h};
        if (align_x == ALIGN_CENTER) dst.x += (rect.w - surface->w) / 2;
        else if (align_x == ALIGN_RIGHT) dst.x += rect.w - surface->w;
        SDL_RenderCopy(renderer, texture, NULL, &dst);
        SDL_FreeSurface(surface);
        SDL_DestroyTexture(texture);
        y += line_heights[i];
        free(lines[i]);
    }
    free(text_copy);
}


int main() {
    SDL_Init(SDL_INIT_VIDEO);
    TTF_Init();
    int FEN_X = 500;
    int FEN_Y = 500;

    SDL_Window *window = SDL_CreateWindow("Texte Centré", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, FEN_X, FEN_Y, SDL_WINDOW_SHOWN);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    TTF_Font *font = TTF_OpenFont("Ressources/Fonts/DejaVuSans-Bold.ttf", 24);
    SDL_Color color = {255, 255, 255, 255};

    int marge = 100;
    SDL_Rect rect = {marge, marge, FEN_X - 2*marge, FEN_Y - 2*marge}; // Zone de texte

    const char *text = "Ceci est un exemple de texte \n qui devrait être centré \n automatiquement dans un rectangle \n en gérant les retours à la ligne. \n \n Paragrphe plus loin ...";

    int running = 1;
    SDL_Event event;
    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) running = 0;

            if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_RESIZED) {
                FEN_X = event.window.data1;
                FEN_Y = event.window.data2;
                rect.w = FEN_X - 2*marge;
                rect.h = FEN_Y - 2*marge;
            }

        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        // Dessiner un rectangle (facultatif)
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderDrawRect(renderer, &rect);

        // Afficher le texte centré dans le rectangle
        render_text_wrapped_2(renderer, font, text, rect, color, ALIGN_CENTER, ALIGN_BOTTOM);

        SDL_RenderPresent(renderer);
    }

    TTF_CloseFont(font);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();
    return 0;
}
