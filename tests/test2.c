#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include <SDL2/SDL_ttf.h>
#include <stdbool.h>

#define WIDTH 800
#define HEIGHT 600
#define HEADER_MIN 100
#define HEADER_MAX 300
#define BUTTON_SIZE 40
#define TEXT_LINES 15
#define SCROLL_STEP 20

SDL_Color header_color = {100, 150, 250, 255};
SDL_Color button_color = {50, 50, 50, 255};
SDL_Color text_color = {255, 255, 255, 255};
bool expanding = false;
int header_height = HEADER_MIN;
int scroll_offset = 0;

void drawHeader(SDL_Renderer *renderer, int height, TTF_Font *font, int scroll_offset) {
    // Dessiner le rectangle principal
    boxRGBA(renderer, 0, 0, WIDTH, height - 20, header_color.r, header_color.g, header_color.b, 255);
    
    // Dessiner le bas arrondi seulement
    filledCircleRGBA(renderer, 20, height - 20, 20, header_color.r, header_color.g, header_color.b, 255);
    filledCircleRGBA(renderer, WIDTH - 20, height - 20, 20, header_color.r, header_color.g, header_color.b, 255);
    boxRGBA(renderer, 20, height - 20, WIDTH - 20, height, header_color.r, header_color.g, header_color.b, 255);
    
    // Dessiner le bouton avec bords arrondis
    int button_x = WIDTH - BUTTON_SIZE - 20;
    int button_y = (height - BUTTON_SIZE) / 2;
    roundedBoxRGBA(renderer, button_x, button_y, button_x + BUTTON_SIZE, button_y + BUTTON_SIZE, 10, button_color.r, button_color.g, button_color.b, 255);
    
    // Dessiner le "+"
    thickLineRGBA(renderer, button_x + 10, button_y + BUTTON_SIZE / 2, button_x + BUTTON_SIZE - 10, button_y + BUTTON_SIZE / 2, 3, 255, 255, 255, 255);
    thickLineRGBA(renderer, button_x + BUTTON_SIZE / 2, button_y + 10, button_x + BUTTON_SIZE / 2, button_y + BUTTON_SIZE - 10, 3, 255, 255, 255, 255);
    
    // Dessiner le texte
    const char *lines[TEXT_LINES] = {"Titre principal", "Option 1", "Option 2", "Option 3", "Option 4", "Option 5", "Option 6", "Option 7", "Option 8", "Option 9", "Option 10", "Option 11", "Option 12", "Option 13", "Option 14"};
    for (int i = 0; i < TEXT_LINES; i++) {
        if (i == 0 || expanding) {
            SDL_Surface *textSurface = TTF_RenderText_Solid(font, lines[i], text_color);
            SDL_Texture *textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
            int text_w, text_h;
            SDL_QueryTexture(textTexture, NULL, NULL, &text_w, &text_h);
            SDL_Rect textRect = { (WIDTH - text_w) / 2, 20 + i * 40 - scroll_offset, text_w, text_h };
            if (textRect.y > 0 && textRect.y < HEADER_MAX) {
                SDL_RenderCopy(renderer, textTexture, NULL, &textRect);
            }
            SDL_FreeSurface(textSurface);
            SDL_DestroyTexture(textTexture);
        }
    }
}

int main() {
    SDL_Init(SDL_INIT_VIDEO);
    TTF_Init();
    SDL_Window *window = SDL_CreateWindow("Animated Header", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, SDL_WINDOW_SHOWN);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    
    TTF_Font *font = TTF_OpenFont("Ressources/Fonts/DejaVuSans-Bold.ttf", 24);
    if (!font) {
        SDL_Log("Failed to load font: %s", TTF_GetError());
        return 1;
    }
    
    SDL_Event event;
    bool running = true;
    
    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            }
            if (event.type == SDL_MOUSEBUTTONDOWN) {
                int x = event.button.x, y = event.button.y;
                if (y <= header_height) {
                    expanding = !expanding;
                }
            }
            if (event.type == SDL_MOUSEWHEEL) {
                if (expanding) {
                    scroll_offset -= event.wheel.y * SCROLL_STEP;
                    if (scroll_offset < 0) scroll_offset = 0;
                    if (scroll_offset > (TEXT_LINES - 1) * 40 - (HEADER_MAX - 40)) scroll_offset = (TEXT_LINES - 1) * 40 - (HEADER_MAX - 40);
                }
            }
        }
        
        // Animation de l'agrandissement
        if (expanding && header_height < HEADER_MAX) {
            header_height += 5;
        } else if (!expanding && header_height > HEADER_MIN) {
            header_height -= 5;
        }
        
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderClear(renderer);
        
        drawHeader(renderer, header_height, font, scroll_offset);
        
        SDL_RenderPresent(renderer);
        SDL_Delay(16);
    }
    
    TTF_CloseFont(font);
    TTF_Quit();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
