#include "grapheur_3D.h"

Camera camera = {
    .position = {0, 0, -10},
    .rotation = {0, 0, 0},
    .renderCenter = {0, 0, 0}
};
// Function to project 3D point to 2D screen coordinates
void projectPoint(Point3D point, int* screenX, int* screenY) {
    // Apply camera position
    point.x -= camera.position.x;
    point.y -= camera.position.y;
    point.z -= camera.position.z;
    
    // Apply rotation around X axis
    float tempY = point.y;
    point.y = point.y * cos(camera.rotation.x) - point.z * sin(camera.rotation.x);
    point.z = tempY * sin(camera.rotation.x) + point.z * cos(camera.rotation.x);
    
    // Apply rotation around Y axis
    float tempX = point.x;
    point.x = point.x * cos(camera.rotation.y) - point.z * sin(camera.rotation.y);
    point.z = tempX * sin(camera.rotation.y) + point.z * cos(camera.rotation.y);
    
    // Apply rotation around Z axis
    tempX = point.x;
    point.x = point.x * cos(camera.rotation.z) - point.y * sin(camera.rotation.z);
    point.y = tempX * sin(camera.rotation.z) + point.y * cos(camera.rotation.z);
    
    // Perspective projection
    float scale = 200.0f / (point.z + 5.0f);
    *screenX = FEN_X - TAILLE_BANDE_DROITE / 2 + (int)(point.x * scale);
    *screenY = FEN_Y - TAILLE_BANDE_HAUT / 2 + (int)(point.y * scale);
}

bool shouldRenderPoint(Point3D point) {
    // Check if point is within our render distance square
    return (point.x >= camera.renderCenter.x - RENDER_DISTANCE &&
            point.x <= camera.renderCenter.x + RENDER_DISTANCE &&
            point.y >= camera.renderCenter.y - RENDER_DISTANCE &&
            point.y <= camera.renderCenter.y + RENDER_DISTANCE);
}

void drawInfiniteAxis(SDL_Renderer* renderer) {
    // Draw infinite grid lines in X and Y directions
    SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
    
    // Calculate the bounds of our render area
    float minX = camera.renderCenter.x - RENDER_DISTANCE;
    float maxX = camera.renderCenter.x + RENDER_DISTANCE;
    float minY = camera.renderCenter.y - RENDER_DISTANCE;
    float maxY = camera.renderCenter.y + RENDER_DISTANCE;
    
    // Draw grid lines in X direction
    for (float y = floor(minY); y <= ceil(maxY); y += 1.0f) {
        Point3D start = {minX, y, 0};
        Point3D end = {maxX, y, 0};
        
        int sx, sy, ex, ey;
        projectPoint(start, &sx, &sy);
        projectPoint(end, &ex, &ey);
        SDL_RenderDrawLine(renderer, sx, sy, ex, ey);
    }
    
    // Draw grid lines in Y direction
    for (float x = floor(minX); x <= ceil(maxX); x += 1.0f) {
        Point3D start = {x, minY, 0};
        Point3D end = {x, maxY, 0};
        
        int sx, sy, ex, ey;
        projectPoint(start, &sx, &sy);
        projectPoint(end, &ex, &ey);
        SDL_RenderDrawLine(renderer, sx, sy, ex, ey);
    }
    
    // Draw main axes (X and Y)
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); // Red X axis
    Point3D xStart = {minX, 0, 0};
    Point3D xEnd = {maxX, 0, 0};
    int xs, ys, xe, ye;
    projectPoint(xStart, &xs, &ys);
    projectPoint(xEnd, &xe, &ye);
    SDL_RenderDrawLine(renderer, xs, ys, xe, ye);
    
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255); // Green Y axis
    Point3D yStart = {0, minY, 0};
    Point3D yEnd = {0, maxY, 0};
    projectPoint(yStart, &xs, &ys);
    projectPoint(yEnd, &xe, &ye);
    SDL_RenderDrawLine(renderer, xs, ys, xe, ye);
}

void renderGraph(SDL_Renderer* renderer) {
    // Draw a 3D function (z = sin(x)*cos(y) in this case)
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    
    float minX = camera.renderCenter.x - RENDER_DISTANCE;
    float maxX = camera.renderCenter.x + RENDER_DISTANCE;
    float minY = camera.renderCenter.y - RENDER_DISTANCE;
    float maxY = camera.renderCenter.y + RENDER_DISTANCE;
    
    float step = 0.2f;
    for (float x = minX; x <= maxX; x += step) {
        for (float y = minY; y <= maxY; y += step) {
            Point3D point = {x, y, sin(x) * cos(y)};
            if (shouldRenderPoint(point)) {
                int screenX, screenY;
                projectPoint(point, &screenX, &screenY);
                SDL_RenderDrawPoint(renderer, screenX, screenY);
            }
        }
    }
}

void handleInput(SDL_Event event, bool* running) {
    const Uint8* keyboardState = SDL_GetKeyboardState(NULL);
    float moveSpeed = 0.5f;
    
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            *running = false;
        } else if (event.type == SDL_MOUSEMOTION) {
            if (event.motion.state & SDL_BUTTON_LMASK) {
                // Left mouse button drag rotates the view
                camera.rotation.y += event.motion.xrel * 0.01f;
                camera.rotation.x += event.motion.yrel * 0.01f;
            }
        } else if (event.type == SDL_MOUSEWHEEL) {
            // Mouse wheel zooms in/out (moves camera along Z axis)
            camera.position.z += event.wheel.y * 0.1f;
        }
    }
    
    // Handle arrow key movement of the render center
    if (keyboardState[SDL_SCANCODE_LEFT]) {
        camera.renderCenter.x -= moveSpeed;
    }
    if (keyboardState[SDL_SCANCODE_RIGHT]) {
        camera.renderCenter.x += moveSpeed;
    }
    if (keyboardState[SDL_SCANCODE_UP]) {
        camera.renderCenter.y -= moveSpeed;
    }
    if (keyboardState[SDL_SCANCODE_DOWN]) {
        camera.renderCenter.y += moveSpeed;
    }
}

void render(SDL_Renderer* renderer) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    
    drawInfiniteAxis(renderer);
    renderGraph(renderer);
    
    // Draw render area boundary (for visualization)
    SDL_SetRenderDrawColor(renderer, 255, 255, 0, 100);
    Point3D corners[4] = {
        {camera.renderCenter.x - RENDER_DISTANCE, camera.renderCenter.y - RENDER_DISTANCE, 0},
        {camera.renderCenter.x + RENDER_DISTANCE, camera.renderCenter.y - RENDER_DISTANCE, 0},
        {camera.renderCenter.x + RENDER_DISTANCE, camera.renderCenter.y + RENDER_DISTANCE, 0},
        {camera.renderCenter.x - RENDER_DISTANCE, camera.renderCenter.y + RENDER_DISTANCE, 0}
    };
    
    int screenX[4], screenY[4];
    for (int i = 0; i < 4; i++) {
        projectPoint(corners[i], &screenX[i], &screenY[i]);
    }
    
    for (int i = 0; i < 4; i++) {
        SDL_RenderDrawLine(renderer, screenX[i], screenY[i],
                            screenX[(i+1)%4], screenY[(i+1)%4]);
    }
    
    SDL_RenderPresent(renderer);
}



void affiche_interface_graph_3D (SDL_Renderer* ren, Bande_haute* bande_haute, Bande_droite* bande_droite){
    SDL_SetRenderDrawColor(ren, colors->bg.r, colors->bg.g, colors->bg.b, colors->bg.a);
    SDL_RenderClear(ren);

    affiche_bande_haut(ren, bande_haute);
    // Rectangle pour cacher les bande d'expression qui ne sont qu'a moitié sur la bande haute. 
    boxRGBA(ren, bande_haute->surface.x, bande_haute->surface.y + bande_haute->surface.h - RAYON_BAS_BANDE_HAUT, bande_haute->surface.x + bande_haute->surface.w, bande_haute->surface.y + bande_haute->surface.h + bande_haute->params.height_bande_expression, colors->bg.r, colors->bg.g, colors->bg.b, colors->bg.a);

    
    // Dessiner le bas arrondi de la bande haute
    affiche_bande_arrondis_en_bas(ren, bande_haute->surface.x, bande_haute->surface.y + bande_haute->surface.h - TAILLE_BARRE_BASSE_DE_BANDE_HAUT, bande_haute->surface.x + bande_haute->surface.w, bande_haute->surface.y + bande_haute->surface.h, RAYON_BAS_BANDE_HAUT, colors->bande_bas_de_bande_haut);
    renderImageButton(ren, &bande_haute->button_new_expression.bt);
    // Affichage de la bande droite
    affiche_bande_droite(ren, bande_droite);

    for (int j = 0; j < bande_haute->nb_expressions; j++) {
        affiche_interface_color_picker(ren, bande_haute->expressions[j]->color_picker);
    }
}


void init_totale_interface_grapheur_3D (SDL_Renderer* ren, Grapheur_3D_elements *gr_ele){
    init_bande_droite(ren, gr_ele->bande_droite);
    init_bande_haute(ren, gr_ele->bande_haute);
}

int Grapheur_3D (SDL_Renderer* ren, Grapheur_3D_elements *gr_ele){
    Bande_haute* bande_haute = gr_ele->bande_haute;
    Bande_droite* bande_droite = gr_ele->bande_droite;

    SDL_StartTextInput();
    bool is_event_backspace_used = false;
    int x_souris_px, y_souris_px;
    int mode_quitter = 0; // Les différentes façons de quitter le grapheur : 0: pas quitter, 1: quitter la fenêtre, 2:quitter et revenir au menu principal 
    bool running = true;

    while (running) {
        affiche_interface_graph_3D(ren, bande_haute, bande_droite);

        mode_quitter = handle_all_events_3D(ren, bande_haute, bande_droite, &x_souris_px, &y_souris_px, &is_event_backspace_used);
        if (mode_quitter) break;

        updateDisplay(ren);
    }
    SDL_StopTextInput();
    return mode_quitter - 1;
}
