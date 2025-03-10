#include <SDL2/SDL.h>
#include <stdio.h>

int main() {
    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window *window = SDL_CreateWindow("Test SDL2 Events",
                                          SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                          800, 600, SDL_WINDOW_RESIZABLE);

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    int running = 1;
    while (running) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    running = 0;
                    break;

                case SDL_WINDOWEVENT:
                    switch (event.window.event) {
                        case SDL_WINDOWEVENT_RESIZED:
                            printf("Fenêtre redimensionnée : %dx%d\n", event.window.data1, event.window.data2);
                            break;
                        case SDL_WINDOWEVENT_SIZE_CHANGED:
                            printf("Taille de la fenêtre changée : %dx%d\n", event.window.data1, event.window.data2);
                            break;
                        case SDL_WINDOWEVENT_MAXIMIZED:
                            printf("Fenêtre maximisée\n");
                            break;
                        case SDL_WINDOWEVENT_MINIMIZED:
                            printf("Fenêtre minimisée\n");
                            break;
                        case SDL_WINDOWEVENT_RESTORED:
                            printf("Fenêtre restaurée\n");
                            break;
                        case SDL_WINDOWEVENT_ENTER:
                            printf("Fenêtre activée\n");
                            break;
                        case SDL_WINDOWEVENT_LEAVE:
                            printf("Fenêtre désactivée\n");
                            break;
                    }
                    break;

                case SDL_KEYDOWN:
                    if (event.key.keysym.sym == SDLK_f) {
                        Uint32 flags = SDL_GetWindowFlags(window);
                        if (flags & SDL_WINDOW_FULLSCREEN) {
                            SDL_SetWindowFullscreen(window, 0);  // Désactiver le plein écran
                            printf("Plein écran désactivé\n");
                        } else {
                            SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);  // Activer le plein écran
                            printf("Plein écran activé\n");
                        }
                    }
                    break;
            }
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
