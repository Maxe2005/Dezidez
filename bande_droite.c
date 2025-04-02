#include "bande_droite.h"

void init_bande_droite (SDL_Renderer* ren, Bande_droite* bande_droite){
    init_boutton_home(ren, bande_droite);
    if (dimention == _2D){
        init_boutton_centrer(ren ,bande_droite);
    }
}

void init_boutton_home (SDL_Renderer* ren, Bande_droite* bande_droite){
    bande_droite->bouton_retour.image = load_image(ren, "Icons/maison.png");
    bande_droite->bouton_retour.rect.w = 70;
    bande_droite->bouton_retour.rect.h = bande_droite->bouton_retour.rect.w;
    bande_droite->bouton_retour.rect.x = FEN_X - TAILLE_BANDE_DROITE + (TAILLE_BANDE_DROITE - bande_droite->bouton_retour.rect.w)/2 ;
    bande_droite->bouton_retour.rect.y = bande_droite->bouton_retour.rect.h/2;
    bande_droite->bouton_retour.is_survolable = 1;
    bande_droite->bouton_retour.color_base = colors->bande_droite;
    bande_droite->bouton_retour.color_hover = (SDL_Color){0, 120, 255, 255};
    bande_droite->bouton_retour.hovered = 0;
    bande_droite->bouton_retour.radius = bande_droite->bouton_retour.rect.w / 3;
    bande_droite->bouton_retour.pourcentage_place = 80;
    bande_droite->bouton_retour.taille_bonus_hover_x = 0;
    bande_droite->bouton_retour.taille_bonus_hover_y = 0;
}

void init_boutton_centrer (SDL_Renderer* ren, Bande_droite* bande_droite){
    bande_droite->bouton_centrer.label = "Centrer";
    bande_droite->bouton_centrer.is_survolable = 1;
    bande_droite->bouton_centrer.color_text = (SDL_Color){255, 255, 255, 255};
    bande_droite->bouton_centrer.color_base = (SDL_Color){150, 150, 150, 255};
    bande_droite->bouton_centrer.color_hover = (SDL_Color){200, 200, 200, 255};
    bande_droite->bouton_centrer.hovered = 0;
    bande_droite->bouton_centrer.font_text = fonts[6];
    bande_droite->bouton_centrer.font_text_hover = fonts[7];
    int width, height;
    TTF_SizeText(bande_droite->bouton_centrer.font_text, bande_droite->bouton_centrer.label, &width, &height);
    bande_droite->bouton_centrer.rect.w = width + 30;
    bande_droite->bouton_centrer.rect.h = height + 20;
    bande_droite->bouton_centrer.rect.x = FEN_X - TAILLE_BANDE_DROITE + (TAILLE_BANDE_DROITE - bande_droite->bouton_centrer.rect.w) / 2;
    bande_droite->bouton_centrer.rect.y = 250;
    bande_droite->bouton_centrer.radius = bande_droite->bouton_centrer.rect.w / 3;
    bande_droite->bouton_centrer.taille_bonus_hover_x = 20;
    bande_droite->bouton_centrer.taille_bonus_hover_y = 20;
}

void resize_bande_droite (Bande_droite* bande_droite){
    bande_droite->bouton_retour.rect.x = FEN_X - TAILLE_BANDE_DROITE + (TAILLE_BANDE_DROITE - bande_droite->bouton_retour.rect.w)/2 ;
    if (dimention == _2D){
        bande_droite->bouton_centrer.rect.x = FEN_X - TAILLE_BANDE_DROITE + (TAILLE_BANDE_DROITE - bande_droite->bouton_centrer.rect.w) / 2;
    }
}

void affiche_bande_droite (SDL_Renderer* ren, Bande_droite* bande_droite){
    boxRGBA(ren, FEN_X - TAILLE_BANDE_DROITE, 0, FEN_X, FEN_Y, colors->bande_droite.r, colors->bande_droite.g, colors->bande_droite.b, colors->bande_droite.a);
    renderImageButton(ren, &bande_droite->bouton_retour);
    if (dimention == _2D){
        renderButton(ren, &bande_droite->bouton_centrer);
    }
}

