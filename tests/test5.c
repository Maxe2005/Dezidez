#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600
#define LINE_HEIGHT 25 // Hauteur de ligne approximative
#define SCROLL_SPEED 20 // Vitesse de scroll

#define TAILLE_INTER_PARAHRAPHE 30
#define MARGE 20

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

typedef struct {
    char text[1024];
    int y_position;
    int is_title;
} Line;

void render_markdown(SDL_Renderer *renderer, TTF_Font *font, TTF_Font *font_title, const char *filename, int scroll_offset) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        printf("Erreur : Impossible d'ouvrir le fichier %s\n", filename);
        return;
    }
    
    SDL_Color white = {255, 255, 255, 255};
    char line[1024];
    Line lines[500]; // Stocker les lignes pré-chargées
    int line_count = 0;
    int y_offset = MARGE;
    
    while (fgets(line, sizeof(line), file)) {
        if (line_count >= 500) break; // Éviter le dépassement du tableau
        
        lines[line_count].y_position = y_offset;
        lines[line_count].is_title = (strncmp(line, "# ", 2) == 0);
        strncpy(lines[line_count].text, line + (lines[line_count].is_title ? 2 : 0), sizeof(lines[line_count].text));
        
        y_offset += lines[line_count].is_title ? 40 : LINE_HEIGHT;
        line_count++;
    }
    fclose(file);
    
    for (int i = 0; i < line_count; i++) {
        if (lines[i].y_position + scroll_offset + LINE_HEIGHT < 0) continue; // Hors écran haut
        if (lines[i].y_position + scroll_offset > SCREEN_HEIGHT) break; // Hors écran bas
        
        SDL_Rect rect = {MARGE, lines[i].y_position + scroll_offset, SCREEN_WIDTH - 2*MARGE, SCREEN_HEIGHT};
        TTF_Font *current_font = lines[i].is_title ? font_title : font;
        render_text_wrapped(renderer, current_font, lines[i].text, rect, white);
    }
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: %s <fichier_markdown>\n", argv[0]);
        return 1;
    }

    SDL_Init(SDL_INIT_VIDEO);
    TTF_Init();
    SDL_Window *window = SDL_CreateWindow("Markdown Viewer", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    
    TTF_Font *font = TTF_OpenFont("Ressources/Fonts/DejaVuSans-Bold.ttf", 24);
    TTF_Font *font_title = TTF_OpenFont("Ressources/Fonts/DejaVuSans-Bold.ttf", 32);
    
    int scroll_offset = 0;
    int running = 1;
    SDL_Event event;
    
    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = 0;
            } else if (event.type == SDL_MOUSEWHEEL) {
                scroll_offset += event.wheel.y * SCROLL_SPEED;
                if (scroll_offset > 0) scroll_offset = 0;
            }
        }
        
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        render_markdown(renderer, font, font_title, argv[1], scroll_offset);
        SDL_RenderPresent(renderer);
    }
    
    TTF_CloseFont(font);
    TTF_CloseFont(font_title);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();
    return 0;
}
