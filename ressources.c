#include "ressources.h"

TTF_Font* fonts[NB_FONTS] = {NULL};

void init_font (TTF_Font* font[NB_FONTS]) {
    font[0] = createFont("Ressources/DejaVuSans-Bold.ttf", 40); //Font de titres
    font[1] = createFont("Ressources/DejaVuSans-Bold.ttf", 25); //Font de bouton normal
    font[2] = createFont("Ressources/DejaVuSans-Bold.ttf", 30); //Font de bouton avec souris dessus
    font[3] = createFont("Ressources/DejaVuSans-Bold.ttf", 35);
    font[4] = createFont("Ressources/DejaVuSans-Bold.ttf", 15); //Font de texte descriptif de bande haute
}

void renderButton(SDL_Renderer *renderer, Button *button) {
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    if (button->is_survolable) {
        TTF_Font * font;
        SDL_Color color = button->hovered ? button->color_hover : button->color_base;
        SDL_Rect tmp_rect;
        if (button->hovered) {
            int taille_bonus_x = button->taille_bonus_hover_x;
            int taille_bonus_y = button->taille_bonus_hover_y;
            tmp_rect.w = button->rect.w + taille_bonus_x;
            tmp_rect.h = button->rect.h + taille_bonus_y;
            tmp_rect.x = button->rect.x - taille_bonus_x/2;
            tmp_rect.y = button->rect.y - taille_bonus_y/2;
            font = button->font_text_hover;
        } else {
            tmp_rect.w = button->rect.w;
            tmp_rect.h = button->rect.h;
            tmp_rect.x = button->rect.x;
            tmp_rect.y = button->rect.y;
            font = button->font_text;
        }
            SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
            SDL_RenderFillRect(renderer, &tmp_rect);
            renderText(renderer, button->label, tmp_rect.x + tmp_rect.w/2, tmp_rect.y + tmp_rect.h/2, button->color_text, font);
    } else {
        SDL_SetRenderDrawColor(renderer, button->color_base.r, button->color_base.g, button->color_base.b, button->color_base.a);
        SDL_RenderFillRect(renderer, &button->rect);
        renderText(renderer, button->label, button->rect.x + button->rect.w/2, button->rect.y + button->rect.h/2, button->color_text, button->font_text);
    }
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