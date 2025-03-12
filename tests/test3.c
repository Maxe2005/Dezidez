#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL2_gfxPrimitives.h>

typedef struct {
    SDL_Rect rect;
    int hovered;
    SDL_Texture *image;
    SDL_Color color_base;
    SDL_Color color_hover;
    int taille_bonus_hover_x;
    int taille_bonus_hover_y;
    int is_survolable; // Booleen pour savoir si le bouton utilise ses param hover (= 1) ou non (= 0)
    int radius; // Rayon des coins arrondis
    int pourcentage_place; // Le pourcentage de place utilisé par l'image dans le bouton
} ImageButton;

void draw_image (SDL_Renderer *renderer, ImageButton *button, SDL_Rect rect){
    SDL_Rect tmp_rect;
    tmp_rect.w = rect.w * button->pourcentage_place / 100;
    tmp_rect.h = rect.h * button->pourcentage_place / 100;
    tmp_rect.x = rect.x + (rect.w - tmp_rect.w)/2;
    tmp_rect.y = rect.y + (rect.h - tmp_rect.h)/2;
    SDL_RenderCopy(renderer, button->image, NULL, &tmp_rect);
}

void renderImageButton(SDL_Renderer *renderer, ImageButton *button) {
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    if (button->is_survolable && button->hovered) {
        SDL_Color color = button->color_hover;
        SDL_Rect tmp_rect;
        tmp_rect.w = button->rect.w + button->taille_bonus_hover_x;
        tmp_rect.h = button->rect.h + button->taille_bonus_hover_y;
        tmp_rect.x = button->rect.x - button->taille_bonus_hover_x/2;
        tmp_rect.y = button->rect.y - button->taille_bonus_hover_y/2;

        // Dessiner le rectangle avec des coins arrondis
        roundedBoxRGBA(renderer, tmp_rect.x, tmp_rect.y, tmp_rect.x + tmp_rect.w, tmp_rect.y + tmp_rect.h, button->radius, color.r, color.g, color.b, color.a);

        draw_image(renderer, button, tmp_rect);
        // Dessiner l'image avec des coins arrondis
        //SDL_RenderCopy(renderer, button->image, NULL, &tmp_rect);
    } else {
        // Dessiner le rectangle avec des coins arrondis
        roundedBoxRGBA(renderer, button->rect.x, button->rect.y, button->rect.x + button->rect.w, button->rect.y + button->rect.h, button->radius, button->color_base.r, button->color_base.g, button->color_base.b, button->color_base.a);

        draw_image(renderer, button, button->rect);
        // Dessiner l'image avec des coins arrondis
        //SDL_RenderCopy(renderer, button->image, NULL, &button->rect);
    }
}

void load_image (SDL_Renderer *renderer, const char * filename, SDL_Texture *image_dest){
    char path[100] = "Ressources/";
    strcat(path, filename);
    // Chargement de l'image
    SDL_Surface *surface = IMG_Load(filename);
    if (!surface) {
        printf("Erreur de chargement de l'image: %s\n", IMG_GetError());
        return 1;
    }

    image_dest = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
}

int main(int argc, char *argv[]) {
    // Initialisation de SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("Erreur d'initialisation de SDL: %s\n", SDL_GetError());
        return 1;
    }

    // Création de la fenêtre
    SDL_Window *window = SDL_CreateWindow("Exemple de bouton avec image", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_SHOWN);
    if (!window) {
        printf("Erreur de création de la fenêtre: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    // Création du renderer
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        printf("Erreur de création du renderer: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    

    // Création du bouton
    ImageButton button;
    button.rect = (SDL_Rect){200, 150, 200, 200};
    button.hovered = 0;
    load_image(renderer, "maison.png", button.image);
    button.color_base = (SDL_Color){255, 255, 255, 255};
    button.color_hover = (SDL_Color){200, 200, 200, 255};
    button.taille_bonus_hover_x = 20;
    button.taille_bonus_hover_y = 20;
    button.is_survolable = 1;
    button.radius = 0;
    button.pourcentage_place = 70;

    // Boucle principale
    int quit = 0;
    SDL_Event event;
    while (!quit) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                quit = 1;
            }
        }

        // Rendu du bouton
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        renderImageButton(renderer, &button);
        SDL_RenderPresent(renderer);
    }

    // Nettoyage
    SDL_DestroyTexture(button.image);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
