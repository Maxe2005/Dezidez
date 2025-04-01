#ifndef GESTIONGRAPHIQUE_H_
#define GESTIONGRAPHIQUE_H_

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include "SDL2_gfx/SDL2_gfxPrimitives.h"

#define NB_MAX_CHAR_PAR_LIGNE 1024
#define TAILLE_INTER_PARAHRAPHE 50

// Paramètres pour l'affichage du Markdown (MD)
#define MARGE_MD 150
#define FONT_TEXT_SIZE_MD 20
#define FONT_TITRE_1_SIZE_MD 50
#define FONT_TITRE_2_SIZE_MD 34
#define FONT_TITRE_3_SIZE_MD 28
#define NB_FONTS_MD 4

extern int FEN_X;
extern int FEN_Y;

/**
 * Structure d'un bouton avec son rectangle, son texte et son état de survol
 */
typedef struct {
    SDL_Rect rect;
    int hovered;
    const char* label;
    SDL_Color color_text;
    SDL_Color color_base;
    SDL_Color color_hover;
    TTF_Font * font_text;
    TTF_Font * font_text_hover;
    int taille_bonus_hover_x;
    int taille_bonus_hover_y;
    int is_survolable; // Booleen pour savoir si le bouton utilise ses param hover (= 1) ou non (= 0)
    int radius; // Rayon des coins arrondis
} Button;

typedef struct {
    SDL_Rect rect;
    int hovered;
    SDL_Texture *image;
    SDL_Color color_base;
    SDL_Color color_hover;
    int taille_bonus_hover_x;
    int taille_bonus_hover_y;
    int is_survolable; // Booleen pour savoir si le bouton utilise ses param hover (= 1) ou non (= 0)
    int radius; // Rayon des coins arrondis
    int pourcentage_place; // Le pourcentage de place utilisé par l'image dans le bouton
} ImageButton;

typedef enum { ALIGN_LEFT, ALIGN_CENTER_X, ALIGN_RIGHT } TextAlignX;
typedef enum { ALIGN_TOP, ALIGN_MIDDLE_Y, ALIGN_BOTTOM } TextAlignY;

typedef struct Ligne_texte {
    char* text;
    int hauteur;
    struct Ligne_texte* ligne_suivante;
} Ligne_texte;

typedef struct {
    TTF_Font *font;
    Ligne_texte fist_ligne;
    bool is_erreur; // Pour savoir si le texte a été correctement affiché : 0 si oui, 1 sinon
    int total_height; // Hauteur totale du texte
    int nb_lines; // Nombre de lignes
} WrappedText;

typedef struct Line{
    WrappedText wrapped_text;
    int y_position;
    int is_title; // Pour savoir si le texte est un titre : 0 si non, 1 si titre 1, 2 si titre 2, ...
    int is_list; // Pour savoir si le texte est une liste : 0 si non, 1 si liste à puce, 2 si liste numérotée
    struct Line* ligne_suivante; 
} Line;

typedef struct {
    Line fist_line;
    int nb_lines;
    int total_height;
} MarkdownText;

/**
 * Fonction qui découpe un texte en lignes de manière à ce qu'il rentre dans un rectangle donné en prenant en compte les retour à la ligne forcés avec '\n'
 * @param font La police du texte
 * @param text Le texte à découper
 * @param x_max La largeur maximale du rectangle
 * @return Une structure contenant les lignes de texte découpées
 */
WrappedText text_wrapper (TTF_Font *font, const char *text, int x_max);

/**
 * Fonction qui affiche un texte découpé par la fonction text_wrapper avec un alignement horizontal et vertical
 * @param renderer Le renderer sur lequel afficher le texte
 * @param wrapped_text Le texte découpé
 * @param rect Le rectangle dans lequel afficher le texte
 * @param color La couleur du texte
 * @param align_x L'alignement horizontal du texte
 * @param align_y L'alignement vertical du texte
 */
void render_text_wrapped(SDL_Renderer *renderer, WrappedText wrapped_text, SDL_Rect rect, SDL_Color color, TextAlignX align_x, TextAlignY align_y);

/**
 * Fonction qui compte le nombre de lignes dans un fichier
 * @param filename Le nom du fichier
 * @return Le nombre de lignes
 */
int compter_nb_lignes (const char *filename);

/**
 * Charge un fichier markdown et le découpe en lignes de texte
 * @param fonts Les polices de caractères
 * @param filename Le nom du fichier
 * @param marge La marge à gauche et à droite du texte
 * @return Une structure contenant les lignes de texte découpées
 */
MarkdownText charge_markdown(TTF_Font *fonts[NB_FONTS_MD], const char *filename, int marge);

/**
 * Affiche un texte markdown sur un renderer
 * @param renderer Le renderer sur lequel afficher le texte
 * @param md_txt Le texte markdown à afficher
 * @param scroll_offset Le décalage de scroll
 * @param marge La marge à gauche et à droite du texte
 */
void render_markdown(SDL_Renderer *renderer, MarkdownText* md_txt, int scroll_offset, int marge);

/**
 * Libère la mémoire allouée pour une structure WrappedText
 * @param WrappedText La structure à libérer
 */
void free_WrappedText (WrappedText WrappedText);

/**
 * Libère la mémoire allouée pour une structure MarkdownText
 * @param md_txt La structure à libérer
 */
void free_MarkdownText (MarkdownText* md_txt);

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

/** Libère la mémoire de la fenêtre et du renderer
 * @param win Un pointeur sur une structure contenant la fenêtre
 * @param ren Un pointeur sur une structure contenant l'état du rendu
 */
void QuitSDL(SDL_Window *win, SDL_Renderer *ren);


/**
 * Affiche un en-tête en début de page
 * @param renderer Un pointeur sur une structure contenant l'état du rendu
 * @param titre Le texte affiché dans le header
 * @param font La police du texte
 * @param fen_x La largeur de la fenêtre
 * @param header_height La hauteur de l'en-tête
 */
void renderHeader(SDL_Renderer *renderer, char *titre, TTF_Font *font, int fen_x, int header_height);

/**
 * Affiche un bouton avec du texte
 * @param renderer Un pointeur sur une structure contenant l'état du rendu
 * @param button Une instance de bouton
 */
void renderButton(SDL_Renderer *renderer, Button *button);

/**
 * Affiche du texte sur le renderer autour de la position x, y
 * @param renderer Un pointeur sur une structure contenant l'état du rendu
 * @param text Le texte à afficher
 * @param x La position du centre en x du texte
 * @param y La position du centre en y du texte
 * @param color La couleur du texte
 * @param font La police du texte
 */
void renderText(SDL_Renderer *renderer, const char *text, int x, int y, SDL_Color color, TTF_Font *font);

/**
 * Affiche un bouton avec une image
 * @param renderer Un pointeur sur une structure contenant l'état du rendu
 * @param button Une instance de bouton
 */
void renderImageButton(SDL_Renderer *renderer, ImageButton *button);

/**
 * Affiche une image de bouton dans le rectangle rect en prenant en compte le paramètre du bouton pourcentage_place
 * @param renderer Un pointeur sur une structure contenant l'état du rendu
 * @param button Une instance de bouton
 * @param rect Le rectangle dans lequel placer l'image
 */
void draw_image (SDL_Renderer *renderer, ImageButton *button, SDL_Rect rect);

/**
 * Charge l'image (avec le nom <filename> se trouvant dans le répèrtoire Ressources/) et la renvoie
 * @param renderer Un pointeur sur une structure contenant l'état du rendu
 * @param filename Le nom de l'image avec son extention ou le path pour accèder à l'image depuis Ressources/
 * @return La texture de l'image chargée
 */
SDL_Texture* load_image (SDL_Renderer *renderer, const char * filename);

/**
 * Test si la souris est dans le rectangle
 * @param rect Le rectangle
 * @param x_souris_px La position de la souris sur l'axe x en pixels
 * @param y_souris_px La position de la souris sur l'axe y en pixels
 */
int is_souris_sur_rectangle (SDL_Rect rect, int x_souris_px, int y_souris_px);

/**
 * Affiche un rectangle avec les bord bas arrondis
 * @param ren Un pointeur sur une structure contenant l'état du rendu
 * @param x1 Le point en haut à gauche du rectangle
 * @param y1 Le point en haut à gauche du rectangle
 * @param x2 Le point en bas à droite du rectangle
 * @param y2 Le point en bas à droite du rectangle
 * @param radius Le rayon de la courbure des coins bas
 * @param color La couleur du rectangle
 */
void affiche_bande_arrondis_en_bas (SDL_Renderer* renderer, int x1, int y1, int x2, int y2, int radius, SDL_Color color);

/**
 * Affiche un rectangle avec les bord bas arrondis
 * @param ren Un pointeur sur une structure contenant l'état du rendu
 * @param x1 Le point en haut à gauche du rectangle
 * @param y1 Le point en haut à gauche du rectangle
 * @param x2 Le point en bas à droite du rectangle
 * @param y2 Le point en bas à droite du rectangle
 * @param radius Le rayon de la courbure des coins bas
 * @param color La couleur du rectangle
 */
void affiche_bande_arrondis_en_haut (SDL_Renderer* renderer, int x1, int y1, int x2, int y2, int radius, SDL_Color color);


#endif

