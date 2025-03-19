#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <string.h>
#include <stdlib.h>

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define LINE_HEIGHT 30  // Hauteur d'une ligne de texte
#define MAX_LINES 200   // Nombre max de lignes affichables

typedef struct {
    SDL_Texture *texture;
    int width, height;
} TextLine;

void render_text(SDL_Renderer *renderer, TTF_Font *font, TextLine *lines, const char **text_lines, int num_lines) {
    SDL_Color color = {255, 255, 255, 255};

    for (int i = 0; i < num_lines; i++) {
        if (lines[i].texture == NULL) {
            SDL_Surface *surface = TTF_RenderUTF8_Blended(font, text_lines[i], color);
            lines[i].texture = SDL_CreateTextureFromSurface(renderer, surface);
            lines[i].width = surface->w;
            lines[i].height = surface->h;
            SDL_FreeSurface(surface);
        }
    }
}

void render_visible_text(SDL_Renderer *renderer, TextLine *lines, int num_lines, int scroll_offset) {
    int start_index = scroll_offset / LINE_HEIGHT;
    int end_index = start_index + (WINDOW_HEIGHT / LINE_HEIGHT) + 1;

    if (end_index > num_lines) end_index = num_lines;

    for (int i = start_index; i < end_index; i++) {
        if (lines[i].texture) {
            SDL_Rect dst = {50, i * LINE_HEIGHT - scroll_offset, lines[i].width, lines[i].height};
            SDL_RenderCopy(renderer, lines[i].texture, NULL, &dst);
        }
    }
}

int main() {
    SDL_Init(SDL_INIT_VIDEO);
    TTF_Init();

    SDL_Window *window = SDL_CreateWindow("Texte Scrollable", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    TTF_Font *font = TTF_OpenFont("Ressources/Fonts/DejaVuSans-Bold.ttf", 24);
    if (!font) {
        printf("Erreur chargement police: %s\n", TTF_GetError());
        return -1;
    }

    // Générer un texte de 200 lignes pour test
    char *text_data[MAX_LINES];
    for (int i = 0; i < MAX_LINES; i++) {
        text_data[i] = malloc(150);
        sprintf(text_data[i], "Ligne numéro %d - UTF8 : éèàùç ñäöü ß  j kk  j k j jkj k j  kj  jk j kj k j kj k jk ", i + 1);
    }

    TextLine lines[MAX_LINES] = {0};
    render_text(renderer, font, lines, (const char **)text_data, MAX_LINES);

    int scroll_offset = 0;
    int running = 1;
    SDL_Event event;

    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) running = 0;

            if (event.type == SDL_MOUSEWHEEL) {
                scroll_offset -= event.wheel.y * 30;
                if (scroll_offset < 0) scroll_offset = 0;
                if (scroll_offset > (MAX_LINES * LINE_HEIGHT - WINDOW_HEIGHT))
                    scroll_offset = MAX_LINES * LINE_HEIGHT - WINDOW_HEIGHT;
            }
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        render_visible_text(renderer, lines, MAX_LINES, scroll_offset);

        SDL_RenderPresent(renderer);
    }

    for (int i = 0; i < MAX_LINES; i++) {
        if (lines[i].texture) SDL_DestroyTexture(lines[i].texture);
        free(text_data[i]);
    }

    TTF_CloseFont(font);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();
    return 0;
}
