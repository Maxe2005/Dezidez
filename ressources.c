#include "ressources.h"

TTF_Font* fonts[NB_FONTS] = {NULL};
Colors* colors = NULL;
int FEN_X = 1100;
int FEN_Y = 800;


void init_font (TTF_Font* font[NB_FONTS]) {
    // ! A faire correspondre avec <NB_FONTS>
    font[0] = createFont("Ressources/Fonts/DejaVuSans-Bold.ttf", 40); //Font de titres
    font[1] = createFont("Ressources/Fonts/DejaVuSans-Bold.ttf", 25); //Font de bouton normal
    font[2] = createFont("Ressources/Fonts/DejaVuSans-Bold.ttf", 30); //Font de bouton avec souris dessus
    font[3] = createFont("Ressources/Fonts/DejaVuSans-Bold.ttf", 35);
    font[4] = createFont("Ressources/Fonts/DejaVuSans-Bold.ttf", 15); //Font de texte descriptif de bande haute
    font[5] = createFont("Ressources/Fonts/DejaVuSans-Bold.ttf", 12); //Font de texte de graduation
}

int nb_alea(int min, int max){
    return min + rand() % (max-min +1);
}
