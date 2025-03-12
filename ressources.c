#include "ressources.h"

TTF_Font* fonts[NB_FONTS] = {NULL};
int FEN_X = 1000;
int FEN_Y = 800;


void init_font (TTF_Font* font[NB_FONTS]) {
    font[0] = createFont("Ressources/Fonts/DejaVuSans-Bold.ttf", 40); //Font de titres
    font[1] = createFont("Ressources/Fonts/DejaVuSans-Bold.ttf", 25); //Font de bouton normal
    font[2] = createFont("Ressources/Fonts/DejaVuSans-Bold.ttf", 30); //Font de bouton avec souris dessus
    font[3] = createFont("Ressources/Fonts/DejaVuSans-Bold.ttf", 35);
    font[4] = createFont("Ressources/Fonts/DejaVuSans-Bold.ttf", 15); //Font de texte descriptif de bande haute
    font[5] = createFont("Ressources/Fonts/DejaVuSans-Bold.ttf", 12); //Font de texte de graduation
}

void renderButton(SDL_Renderer *renderer, Button *button) {
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    if (button->rect.w < 2*button->radius) button->radius = button->rect.w / 2;
    if (button->rect.h < 2*button->radius) button->radius = button->rect.h / 2;
    if (button->is_survolable && button->hovered) {
        SDL_Rect tmp_rect;
        tmp_rect.w = button->rect.w + button->taille_bonus_hover_x;
        tmp_rect.h = button->rect.h + button->taille_bonus_hover_y;
        tmp_rect.x = button->rect.x - button->taille_bonus_hover_x/2;
        tmp_rect.y = button->rect.y - button->taille_bonus_hover_y/2;

        // Dessiner le rectangle avec des coins arrondis
        roundedBoxRGBA(renderer, tmp_rect.x, tmp_rect.y, tmp_rect.x + tmp_rect.w, tmp_rect.y + tmp_rect.h, button->radius, button->color_hover.r, button->color_hover.g, button->color_hover.b, button->color_hover.a);
        renderText(renderer, button->label, tmp_rect.x + tmp_rect.w/2, tmp_rect.y + tmp_rect.h/2, button->color_text, button->font_text_hover);
    } else {
        // Dessiner le rectangle avec des coins arrondis
        roundedBoxRGBA(renderer, button->rect.x, button->rect.y, button->rect.x + button->rect.w, button->rect.y + button->rect.h, button->radius, button->color_base.r, button->color_base.g, button->color_base.b, button->color_base.a);
        renderText(renderer, button->label, button->rect.x + button->rect.w/2, button->rect.y + button->rect.h/2, button->color_text, button->font_text);
    }
}

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
        SDL_Rect tmp_rect;
        tmp_rect.w = button->rect.w + button->taille_bonus_hover_x;
        tmp_rect.h = button->rect.h + button->taille_bonus_hover_y;
        tmp_rect.x = button->rect.x - button->taille_bonus_hover_x/2;
        tmp_rect.y = button->rect.y - button->taille_bonus_hover_y/2;

        // Dessiner le rectangle avec des coins arrondis
        roundedBoxRGBA(renderer, tmp_rect.x, tmp_rect.y, tmp_rect.x + tmp_rect.w, tmp_rect.y + tmp_rect.h, button->radius, button->color_hover.r, button->color_hover.g, button->color_hover.b, button->color_hover.a);
        draw_image(renderer, button, tmp_rect);
    } else {
        // Dessiner le rectangle avec des coins arrondis
        roundedBoxRGBA(renderer, button->rect.x, button->rect.y, button->rect.x + button->rect.w, button->rect.y + button->rect.h, button->radius, button->color_base.r, button->color_base.g, button->color_base.b, button->color_base.a);
        draw_image(renderer, button, button->rect);
    }
}

SDL_Texture* load_image(SDL_Renderer* renderer, const char* filename) {
    char path[100] = "Ressources/";
    strcat(path, filename);
    SDL_Texture* texture = NULL;
    SDL_Surface* loadedSurface = IMG_Load(path);
    if (loadedSurface == NULL) {
        printf("Unable to load image! SDL_image Error: %s\n", IMG_GetError());
    } else {
        texture = SDL_CreateTextureFromSurface(renderer, loadedSurface);
        SDL_FreeSurface(loadedSurface);
        if (texture == NULL) {
            printf("Unable to create texture from %s! SDL Error: %s\n", path, SDL_GetError());
        }
    }
    return texture;
}

void renderHeader(SDL_Renderer *renderer, char *titre) {
    // Fond de l'en-tête
    SDL_SetRenderDrawColor(renderer, 50, 50, 50, 255);
    SDL_Rect header = {0, 0, FEN_X, HEADER_HEIGHT};
    SDL_RenderFillRect(renderer, &header);

    // Texte "titre"
    SDL_Surface *surface = TTF_RenderUTF8_Solid(fonts[3], titre, (SDL_Color){255, 255, 255, 255});
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_Rect textRect = {FEN_X / 2 - surface->w / 2, HEADER_HEIGHT / 2 - surface->h / 2, surface->w, surface->h};
    
    SDL_RenderCopy(renderer, texture, NULL, &textRect);

    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
}

void renderText(SDL_Renderer *renderer, const char *text, int x, int y, SDL_Color color, TTF_Font *font){
    SDL_Surface *surface =  TTF_RenderUTF8_Solid(font, text, color);
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_Rect textRect = {x - surface->w/2, y - surface->h/2, surface->w, surface->h};

    SDL_RenderCopy(renderer, texture, NULL, &textRect);

    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
}

int is_souris_sur_rectangle (SDL_Rect rect, int x_souris_px, int y_souris_px){
    return x_souris_px >= rect.x && x_souris_px <= rect.x + rect.w &&
                    y_souris_px >= rect.y && y_souris_px <= rect.y + rect.h;
}

