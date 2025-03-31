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
    init_tous_les_json();

    Grapheur_elements grapheur_ele;
    grapheur_ele.bande_haute = malloc(sizeof(Bande_haute));
    grapheur_ele.bande_droite = malloc(sizeof(Bande_droite));
    grapheur_ele.graph = malloc(sizeof(Graph));
    init_totale_interface_grapheur(ren, &grapheur_ele);

    ecran_acceuil(ren, &grapheur_ele);

    QuitSDL(window,ren);
    return 0;
}
