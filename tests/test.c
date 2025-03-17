#include "../color_picker.h"


int main() {
    SDL_Init(SDL_INIT_VIDEO);
    initTTF();
    SDL_Window *window = SDL_CreateWindow("Color Picker", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, FEN_X, FEN_Y, SDL_WINDOW_SHOWN);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    init_font(fonts);
    Color_picker color_picker = init_color_picker(renderer, 100, 100, 60, (SDL_Color){255, 0, 0 , 255});

    int x,y;
    SDL_Event event;
    bool running = true;
    while (running) {
        
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) running = false;

            if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_RESIZED) {
                FEN_X = event.window.data1;
                FEN_Y = event.window.data2;
                init_placement_color_picker(&color_picker);
            }
        
            if (event.type == SDL_MOUSEBUTTONDOWN) {
                handle_event_color_picker_MOUSEBUTTONDOWN(&color_picker, x, y);
            }
            if (event.type == SDL_MOUSEBUTTONUP) {
                handle_event_color_picker_MOUSEBUTTONUP(&color_picker, x, y);
            }
            if (event.type == SDL_MOUSEMOTION) {
                x = event.button.x;
                y = event.button.y;
                handle_event_color_picker_SDL_MOUSEMOTION(event, &color_picker, x, y);
            }
        }

        SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);
        SDL_RenderClear(renderer);

        affiche_color_picker(renderer, &color_picker);

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
