#ifndef GESTIONGRAPHIQUE_H_
#define GESTIONGRAPHIQUE_H_

#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

/**
* Initialise la SDL
*/
void initSDL();

/**
* Créé une fenêtre SDL
* @param winName Nom de la fenêtre
* @param winWidth Largeur de la fenêtre
* @param winHeigth Hauteur de la fenêtre
*/
SDL_Window * createWindow(char winName[], int winWidth, int winHeigth);

/**
* Créé un renderer
* @param win La fenêtre SDL
*/
SDL_Renderer * createRenderer(SDL_Window *win);

/**
* Charge une texture dans le rendu graphique
* @param file Le chemin vers la texture
* @param ren Un pointeur vers la structure contenant le rendu
*/
SDL_Texture* loadTexture(const char* file, SDL_Renderer *ren);

/**
* Met la texture dans le rendu graphique à une position (x,y) dans le repère écran avec une taille de size
* @param tex La texture à mettre dans le rendu
* @param ren Un pointeur vers la structure contenant le rendu
* @param x La coordonnée x dans le repère écran
* @param y La coordonnée y dans le repère écran
* @param size La taille de la texture sur l'écran
*/
void renderTexture(SDL_Texture *tex, SDL_Renderer *ren, int x, int y, int width, int height);

/**
* Initialise la bibliothèque TTF
*/
void initTTF();

/**
* Créé une police de caractère avec un taille de police donnée en paramètre
* @param fontPath Le chemin vers le fichier .ttf contenant la font
* @param fontSize La taille de la font
*/
TTF_Font* createFont(char fontPath[], int fontSize);

/**
* Met dans le rendu graphique un texte à la position (x,y) dans le repère écran avec une taille de (hsize x wsize)
* @param x La coordonnée x dans le repère écran
* @param y La coordonnée y dans le repère écran
* @param wsize La largeur du message sur l'écran
* @param hsize La hauteur du message sur l'écran
* @param font La police utilisée
* @param color La SDL_Color utilisée
* @param ren un pointeur vers une structure qui contient l'état du rendu graphique
*/
void printText(int x,int y,char *text, int wsize, int hsize,TTF_Font* font, SDL_Color color, SDL_Renderer *ren);


/**
* Met à jour l'affichage de l'écran
* @param ren Un pointeur sur une structure contenant l'état du rendu
*/
void updateDisplay(SDL_Renderer *ren);

/** Vide le rendu graphique
 * @param ren Un pointeur sur une structure contenant l'état du rendu
 */
void clearRenderer(SDL_Renderer *ren);

/** Gestion de la souris
 * @param running Un pointeur vers le booléen de la boucle principale de main (sert à stoper le programme)
 * @return le caractère ('h','b','d','g') correspondant à la direction choisie
 */
char processKeyboard(int *running);

/** Libère la mémoire de la fenêtre et du renderer
 * @param win Un pointeur sur une structure contenant la fenêtre
 * @param ren Un pointeur sur une structure contenant l'état du rendu
 */
void QuitSDL(SDL_Window *win, SDL_Renderer *ren);

#endif

