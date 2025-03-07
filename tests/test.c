#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include <stdbool.h>

#define WIDTH 700
#define HEIGHT 500
#define PICKER_SIZE 256
#define BUTTON_SIZE 50

SDL_Color classic_colors[] = {
    {255, 0, 0, 255},   // Rouge
    {0, 255, 0, 255},   // Vert
    {0, 0, 255, 255},   // Bleu
    {255, 255, 0, 255}, // Jaune
    {0, 255, 255, 255}, // Cyan
    {255, 0, 255, 255}, // Magenta
    {255, 255, 255, 255}, // Blanc
    {0, 0, 0, 255}      // Noir
};

SDL_Color selected_color = {255, 255, 255, 255};
bool show_picker = false;
bool dragging_picker = false;
int picker_x = 200, picker_y = 50;

void filledRoundedRectangle(SDL_Renderer *renderer, int x1, int y1, int x2, int y2, int radius, Uint8 r, Uint8 g, Uint8 b, Uint8 a) {
    // Dessiner les coins arrondis
    filledCircleRGBA(renderer, x1 + radius, y1 + radius, radius, r, g, b, a);
    filledCircleRGBA(renderer, x2 - radius, y1 + radius, radius, r, g, b, a);
    filledCircleRGBA(renderer, x1 + radius, y2 - radius, radius, r, g, b, a);
    filledCircleRGBA(renderer, x2 - radius, y2 - radius, radius, r, g, b, a);

    // Dessiner les parties rectangulaires
    boxRGBA(renderer, x1 + radius, y1, x2 - radius, y2, r, g, b, a);
    boxRGBA(renderer, x1, y1 + radius, x2, y2 - radius, r, g, b, a);
}

void drawColorPicker(SDL_Renderer *renderer) {
    // Contour du picker
    int marge = 10;
    roundedRectangleRGBA(renderer, picker_x-marge, picker_y-marge, picker_x + PICKER_SIZE+marge, picker_y + PICKER_SIZE + 40 + marge, 10, 0, 0, 0, 255);
    
    // Dessiner la palette RGB
    for (int x = 0; x < PICKER_SIZE; x++) {
        for (int y = 0; y < PICKER_SIZE; y++) {
            SDL_SetRenderDrawColor(renderer, x, y, 255 - x, 255);
            SDL_RenderDrawPoint(renderer, picker_x + x, picker_y + y);
        }
    }
    
    // Dessiner les couleurs classiques
    int startY = picker_y + PICKER_SIZE + 10;
    for (int i = 0; i < 8; i++) {
        SDL_Rect rect = {picker_x + i * 30, startY, 30, 30};
        SDL_SetRenderDrawColor(renderer, classic_colors[i].r, classic_colors[i].g, classic_colors[i].b, 255);
        SDL_RenderFillRect(renderer, &rect);
    }
}

bool isInsideRect(int x, int y, SDL_Rect rect) {
    return (x >= rect.x && x <= rect.x + rect.w && y >= rect.y && y <= rect.y + rect.h);
}

int main() {
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window *window = SDL_CreateWindow("Color Picker", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, SDL_WINDOW_SHOWN);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    SDL_Rect button = {20, 20, BUTTON_SIZE, BUTTON_SIZE};
    SDL_Event event;
    bool running = true;

    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            }
            if (event.type == SDL_MOUSEBUTTONDOWN) {
                int x = event.button.x, y = event.button.y;
                if (isInsideRect(x, y, button)) {
                    show_picker = !show_picker;
                }
                if (show_picker && x >= picker_x && x < picker_x + PICKER_SIZE && y >= picker_y && y < picker_y + PICKER_SIZE) {
                    selected_color.r = x - picker_x;
                    selected_color.g = y - picker_y;
                    selected_color.b = 255 - selected_color.r;
                }
                if (show_picker && y > picker_y + PICKER_SIZE && y < picker_y + PICKER_SIZE + 40) {
                    int index = (x - picker_x) / 30;
                    if (index >= 0 && index < 8) {
                        selected_color = classic_colors[index];
                    }
                }
                if (show_picker && x >= picker_x && x <= picker_x + PICKER_SIZE && y >= picker_y - 20 && y <= picker_y) {
                    dragging_picker = true;
                }
            }
            if (event.type == SDL_MOUSEBUTTONUP) {
                dragging_picker = false;
            }
            if (event.type == SDL_MOUSEMOTION && dragging_picker) {
                picker_x += event.motion.xrel;
                picker_y += event.motion.yrel;
            }
        }

        SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);
        SDL_RenderClear(renderer);

        filledRoundedRectangle(renderer, button.x, button.y, button.x + button.w, button.y + button.h, 10, selected_color.r, selected_color.g, selected_color.b, 255);
        thickLineRGBA(renderer, button.x + 10, button.y + 10, button.x + 40, button.y + 40, 5, 0, 0, 0, 255); // Icône pinceau
        thickLineRGBA(renderer, button.x + 10, button.y + 40, button.x + 40, button.y + 10, 5, 0, 0, 0, 255);

        if (show_picker) {
            drawColorPicker(renderer);
        }

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
