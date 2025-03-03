#include "graphique.h"

int main (int argc, char *argv[]){
    srand(time(NULL));
    // Initialisation globale fenêtre
    initSDL();
    initTTF();

    SDL_Window* window = createWindow("DEZIDEZ", 800, 700);
    SDL_Renderer* ren = createRenderer(window);

    init_font(fonts);

    //ecran_acceuil(ren, musique);
    SDL_Delay(2000);

    QuitSDL(window,ren);
    return 0;
}