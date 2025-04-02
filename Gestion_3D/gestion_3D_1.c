#include "gestion_3D_1.h"

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
Point2D project(Point3D p, Graph_3D_1* graph) {
    Point2D projected;
    // Appliquer la rotation
    Quaternion pQuat = {0, p.x, p.y, p.z};
    Quaternion rotated = multiplyQuaternions(graph->rotation, multiplyQuaternions(pQuat, (Quaternion){graph->rotation.w, -graph->rotation.x, -graph->rotation.y, -graph->rotation.z}));

    // Projection avec zoom
    projected.x = (int)(rotated.x * graph->zoom) + (FEN_X - TAILLE_BANDE_DROITE) / 2;
    projected.y = (int)(rotated.y * graph->zoom) + (FEN_Y + TAILLE_BANDE_HAUT) / 2;
    return projected;
}

// Dessiner un axe avec des graduations
void drawAxis(SDL_Renderer* renderer, Point3D start, Point3D end, int steps, Graph_3D_1* graph) {
    Point2D start2D = project(start, graph);
    Point2D end2D = project(end, graph);
    if (start2D.y < graph->origine_y_apres_bande_haut) {
        //start2D.y = ((graph->origine_y_apres_bande_haut - start2D.y) * (end2D.x - start2D.x)) / (end2D.y - start2D.y) + start2D.x;
    }
    if (end2D.y < graph->origine_y_apres_bande_haut) {
        //end2D.y = ((graph->origine_y_apres_bande_haut - end2D.y) * (start2D.x - end2D.x)) / (start2D.y - end2D.y) + end2D.x;
    }
    SDL_RenderDrawLine(renderer, start2D.x, start2D.y, end2D.x, end2D.y);

    Point3D step;
    step.x = (end.x - start.x) / steps;
    step.y = (end.y - start.y) / steps;
    step.z = (end.z - start.z) / steps;

    for (int i = 1; i < steps; ++i) {
        Point3D point = {start.x + step.x * i, start.y + step.y * i, start.z + step.z * i};
        Point2D point2D = project(point, graph);
        if (point2D.y > graph->origine_y_apres_bande_haut){
            SDL_RenderDrawLine(renderer, point2D.x - 3, point2D.y, point2D.x + 3, point2D.y);
            SDL_RenderDrawLine(renderer, point2D.x, point2D.y - 3, point2D.x, point2D.y + 3);
        }
    }
}


void handle_event_3D_1 (SDL_Event e, Graph_3D_1* graph, int x_souris_px, int y_souris_px){
    if (is_souris_sur_rectangle((SDL_Rect){0, graph->origine_y_apres_bande_haut, FEN_X - TAILLE_BANDE_DROITE, FEN_Y - graph->origine_y_apres_bande_haut}, x_souris_px, y_souris_px)){
        if (e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT) {
            graph->dragging = 1;

        } else if (e.type == SDL_MOUSEBUTTONUP && e.button.button == SDL_BUTTON_LEFT) {
            graph->dragging = 0;

        } else if (e.type == SDL_MOUSEMOTION) {
            if (e.motion.state & SDL_BUTTON_LMASK) {
                float deltaX = e.motion.xrel * 0.01f;
                float deltaY = e.motion.yrel * 0.01f;
                Quaternion yaw = quaternionFromAxisAngle(0, 1, 0, deltaX);
                Quaternion pitch = quaternionFromAxisAngle(1, 0, 0, deltaY);
                graph->rotation = multiplyQuaternions(yaw, multiplyQuaternions(pitch, graph->rotation));
                normalizeQuaternion(&graph->rotation);
            }

        } else if (e.type == SDL_MOUSEWHEEL) {
            graph->zoom += e.wheel.y * 1.0f;
            if (graph->zoom < 15) graph->zoom = 15;
        }
    }
}

void renderGraph3D_1(SDL_Renderer* renderer, Graph_3D_1* graph, Bande_haute* bande_haute) {
        // Dessiner les axes avec graduations
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        drawAxis(renderer, (Point3D){-5, 0, 0}, (Point3D){5, 0, 0}, 10, graph); // Axes X
        drawAxis(renderer, (Point3D){0, -5, 0}, (Point3D){0, 5, 0}, 10, graph); // Axes Y
        drawAxis(renderer, (Point3D){0, 0, -5}, (Point3D){0, 0, 5}, 10, graph); // Axes Z

        for (int i = 0; i < bande_haute->nb_expressions; i++) {
            Fonction f = bande_haute->expressions[i]->fonction;
            if (f.visible){
                // Dessiner la fonction
                SDL_SetRenderDrawColor(renderer, f.color.r, f.color.g, f.color.b, f.color.a);
                for (float x = f.borne_inf; x <= f.borne_sup; x += graph->zoom/470) {
                    for (float y = f.borne_inf; y <= f.borne_sup; y += graph->zoom/470) {
                        int code_erreur = 0;
                        float z = evaluateur(f.fonction_arbre, x, y, &code_erreur);
                        if (code_erreur){
                            ErrorInfo info_erreur = get_error_message(code_erreur);
                            SDL_Rect rectangle = {3*FEN_X/8,3*FEN_Y/8,FEN_X/8,FEN_Y/16};
                            set_message(info_erreur.message,rectangle);
                            return;
                        }
                        Point3D p = {x, y, z};
                        Point2D projected = project(p, graph);
                        if (projected.y > graph->origine_y_apres_bande_haut){
                            SDL_RenderDrawPoint(renderer, projected.x, projected.y);
                        }
                    }
                }
            }
        }
        
    }
