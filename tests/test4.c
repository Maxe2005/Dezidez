#include <SDL2/SDL.h>
#include <math.h>

// Dimensions de la fenêtre
#define WIDTH 800
#define HEIGHT 600

// Structure pour représenter un point 3D
typedef struct {
    float x, y, z;
} Point3D;

// Structure pour représenter un point 2D
typedef struct {
    int x, y;
} Point2D;

// Structure pour représenter un quaternion
typedef struct {
    float w, x, y, z;
} Quaternion;

// Paramètres de la caméra
Quaternion rotation = {1, 0, 0, 0};
float zoom = 1.0f;

// Fonction pour normaliser un quaternion
void normalizeQuaternion(Quaternion* q) {
    float length = sqrt(q->w * q->w + q->x * q->x + q->y * q->y + q->z * q->z);
    q->w /= length;
    q->x /= length;
    q->y /= length;
    q->z /= length;
}

// Fonction pour multiplier deux quaternions
Quaternion multiplyQuaternions(Quaternion q1, Quaternion q2) {
    Quaternion result;
    result.w = q1.w * q2.w - q1.x * q2.x - q1.y * q2.y - q1.z * q2.z;
    result.x = q1.w * q2.x + q1.x * q2.w + q1.y * q2.z - q1.z * q2.y;
    result.y = q1.w * q2.y - q1.x * q2.z + q1.y * q2.w + q1.z * q2.x;
    result.z = q1.w * q2.z + q1.x * q2.y - q1.y * q2.x + q1.z * q2.w;
    return result;
}

// Fonction pour créer un quaternion à partir d'un axe et d'un angle
Quaternion quaternionFromAxisAngle(float x, float y, float z, float angle) {
    Quaternion q;
    float halfAngle = angle * 0.5f;
    float sinHalfAngle = sin(halfAngle);
    q.w = cos(halfAngle);
    q.x = x * sinHalfAngle;
    q.y = y * sinHalfAngle;
    q.z = z * sinHalfAngle;
    return q;
}

// Fonction de projection 3D à 2D
Point2D project(Point3D p) {
    Point2D projected;
    // Appliquer la rotation
    Quaternion pQuat = {0, p.x, p.y, p.z};
    Quaternion rotated = multiplyQuaternions(rotation, multiplyQuaternions(pQuat, (Quaternion){rotation.w, -rotation.x, -rotation.y, -rotation.z}));

    // Projection avec zoom
    projected.x = (int)(rotated.x * zoom) + WIDTH / 2;
    projected.y = (int)(rotated.y * zoom) + HEIGHT / 2;
    return projected;
}

// Fonction mathématique à afficher
float function3D(float x, float y) {
    return sin(sqrt(x*x + y*y));
}

// Dessiner un axe avec des graduations
void drawAxis(SDL_Renderer* renderer, Point3D start, Point3D end, int steps) {
    Point2D start2D = project(start);
    Point2D end2D = project(end);
    SDL_RenderDrawLine(renderer, start2D.x, start2D.y, end2D.x, end2D.y);

    Point3D step;
    step.x = (end.x - start.x) / steps;
    step.y = (end.y - start.y) / steps;
    step.z = (end.z - start.z) / steps;

    for (int i = 1; i < steps; ++i) {
        Point3D point = {start.x + step.x * i, start.y + step.y * i, start.z + step.z * i};
        Point2D point2D = project(point);
        SDL_RenderDrawLine(renderer, point2D.x - 3, point2D.y, point2D.x + 3, point2D.y);
        SDL_RenderDrawLine(renderer, point2D.x, point2D.y - 3, point2D.x, point2D.y + 3);
    }
}

int main(int argc, char* argv[]) {
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window* window = SDL_CreateWindow("3D Function Plot", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, SDL_WINDOW_SHOWN);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    // Ajuster le zoom pour occuper 70% de l'espace
    zoom = 0.7f * (WIDTH < HEIGHT ? WIDTH : HEIGHT) / 10.0f;

    SDL_Event e;
    int quit = 0;
    int mouseX = 0, mouseY = 0;
    int dragging = 0;

    while (!quit) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                quit = 1;
            } else if (e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT) {
                dragging = 1;
                mouseX = e.button.x;
                mouseY = e.button.y;
            } else if (e.type == SDL_MOUSEBUTTONUP && e.button.button == SDL_BUTTON_LEFT) {
                dragging = 0;
            } else if (e.type == SDL_MOUSEMOTION && dragging) {
                float deltaX = (e.motion.x - mouseX) * 0.01f;
                float deltaY = (e.motion.y - mouseY) * 0.01f;
                Quaternion yaw = quaternionFromAxisAngle(0, 1, 0, deltaX);
                Quaternion pitch = quaternionFromAxisAngle(1, 0, 0, deltaY);
                rotation = multiplyQuaternions(yaw, multiplyQuaternions(pitch, rotation));
                normalizeQuaternion(&rotation);
                mouseX = e.motion.x;
                mouseY = e.motion.y;
            } else if (e.type == SDL_MOUSEWHEEL) {
                zoom += e.wheel.y * 0.1f;
                if (zoom < 0.1f) zoom = 0.1f;
            }
        }

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderClear(renderer);

        // Dessiner les axes avec graduations
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        drawAxis(renderer, (Point3D){-5, 0, 0}, (Point3D){5, 0, 0}, 10); // Axes X
        drawAxis(renderer, (Point3D){0, -5, 0}, (Point3D){0, 5, 0}, 10); // Axes Y
        drawAxis(renderer, (Point3D){0, 0, -5}, (Point3D){0, 0, 5}, 10); // Axes Z

        // Dessiner la fonction
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        for (float x = -5; x <= 5; x += 0.1) {
            for (float y = -5; y <= 5; y += 0.1) {
                Point3D p = {x, y, function3D(x, y)};
                Point2D projected = project(p);
                SDL_RenderDrawPoint(renderer, projected.x, projected.y);
            }
        }

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
