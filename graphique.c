#include "graphique.h"

TTF_Font* fonts[NB_FONTS] = {NULL};

void init_font (TTF_Font* font[NB_FONTS]) {
    font[0] = createFont("Ressources/DejaVuSans-Bold.ttf", 20); //Font de titres
    font[1] = createFont("Ressources/DejaVuSans-Bold.ttf", 25); //Font de bouton normal
    font[2] = createFont("Ressources/DejaVuSans-Bold.ttf", 30); //Font de bouton avec souris dessus
}

