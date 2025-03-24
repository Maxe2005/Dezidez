#include <SDL2/SDL.h>
#include <math.h>

// Structure pour représenter un quaternion
typedef struct {
    float w, x, y, z;
} Quaternion;

// Structure pour représenter un vecteur 3D
typedef struct {
    float x, y, z;
} Vector3D;

// Fonction pour multiplier deux quaternions
Quaternion quaternionMultiply(Quaternion q1, Quaternion q2) {
    Quaternion result;
    result.w = q1.w * q2.w - q1.x * q2.x - q1.y * q2.y - q1.z * q2.z;
    result.x = q1.w * q2.x + q1.x * q2.w + q1.y * q2.z - q1.z * q2.y;
    result.y = q1.w * q2.y - q1.x * q2.z + q1.y * q2.w + q1.z * q2.x;
    result.z = q1.w * q2.z + q1.x * q2.y - q1.y * q2.x + q1.z * q2.w;
    return result;
}

// Fonction pour appliquer une rotation à un vecteur 3D avec un quaternion
Vector3D rotateVector(Quaternion q, Vector3D v) {
    Quaternion p = {0, v.x, v.y, v.z};
    Quaternion qConj = {q.w, -q.x, -q.y, -q.z};
    Quaternion result = quaternionMultiply(quaternionMultiply(q, p), qConj);
    return (Vector3D){result.x, result.y, result.z};
}

// Fonction pour projeter un point 3D en 2D
Vector3D projectTo2D(Vector3D point, float fov, float zOffset) {
    Vector3D projected;
    projected.x = (point.x / (point.z + zOffset)) * fov;
    projected.y = (point.y / (point.z + zOffset)) * fov;
    projected.z = point.z;
    return projected;
}

int main(int argc, char* argv[]) {
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window* window = SDL_CreateWindow("3D Projection with Quaternions",
                                          SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                          800, 600, SDL_WINDOW_SHOWN);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    // Définir les sommets d'un cube
    Vector3D cubeVertices[8] = {
        {-1, -1, -1}, {1, -1, -1}, {1, 1, -1}, {-1, 1, -1},
        {-1, -1, 1}, {1, -1, 1}, {1, 1, 1}, {-1, 1, 1}
    };

    // Définir un quaternion pour la rotation
    Quaternion rotation = {1, 0, 0, 0};

    int running = 1;
    SDL_Event event;
    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = 0;
            }
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        // Appliquer la rotation et projeter les points
        for (int i = 0; i < 8; i++) {
            Vector3D rotated = rotateVector(rotation, cubeVertices[i]);
            Vector3D projected = projectTo2D(rotated, 200, 5);

            // Dessiner le point projeté
            SDL_RenderDrawPoint(renderer, 400 + projected.x, 300 - projected.y);
        }

        SDL_RenderPresent(renderer);

        // Mettre à jour le quaternion de rotation
        rotation.w = cos(SDL_GetTicks() / 1000.0 / 2);
        rotation.x = 0;
        rotation.y = sin(SDL_GetTicks() / 1000.0 / 2);
        rotation.z = 0;
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
