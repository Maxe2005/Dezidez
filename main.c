#include "ressources.h"
#include "menus.h"
#include "grapheur.h"

int main (int argc, char *argv[]){
    srand(time(NULL));
    // Initialisation globale fenêtre
    initSDL();
    IMG_Init(IMG_INIT_PNG);
    IMG_Init(IMG_INIT_JPG);

    initTTF();

    SDL_Window* window = createWindow("DEZIDEZ", FEN_X, FEN_Y);
    SDL_Renderer* ren = createRenderer(window);
    
    init_font(fonts);

    ecran_acceuil(ren);

    QuitSDL(window,ren);
    return 0;
}