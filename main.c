#include "ressources.h"
#include "menus.h"
#include "grapheur.h"
#include "Gestion_3D/grapheur_3D.h"

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
    colors = malloc(sizeof(Colors));
    change_color_mode(1);
    init_const_message(ren);
    message.is_visible = 0;
    probleme.is_visible = 0;

    Grapheur_elements grapheur_ele;
    grapheur_ele.bande_haute = malloc(sizeof(Bande_haute));
    grapheur_ele.bande_droite = malloc(sizeof(Bande_droite));
    grapheur_ele.graph = malloc(sizeof(Graph));
    init_totale_interface_grapheur(ren, &grapheur_ele);

    Grapheur_3D_elements grapheur_ele_3D;
    grapheur_ele_3D.bande_haute = malloc(sizeof(Bande_haute));
    grapheur_ele_3D.bande_droite = malloc(sizeof(Bande_droite));
    grapheur_ele_3D.graph_3D_1 = malloc(sizeof(Graph_3D_1));
    init_totale_interface_grapheur_3D(ren, &grapheur_ele_3D);

    ecran_acceuil(ren, &grapheur_ele, &grapheur_ele_3D);

    free_font(fonts);
    free_tous_les_json();
    free(grapheur_ele.bande_haute);
    free(grapheur_ele.bande_droite);
    free(grapheur_ele.graph);
    free(grapheur_ele_3D.bande_haute);
    free(grapheur_ele_3D.bande_droite);
    free(grapheur_ele_3D.graph_3D_1);
    free(colors);
    QuitSDL(window,ren);
    return 0;
}
