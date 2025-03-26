#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define FEN_X 800
#define FEN_Y 600

#define NB_MAX_CHAR_PAR_LIGNE 1024
#define NB_MAX_MOTS_PAR_LIGNE 100
#define SCROLL_SPEED 20 // Vitesse de scroll

#define TAILLE_INTER_PARAHRAPHE 50
#define MARGE 50
#define FONT_TEXT_SIZE 20
#define FONT_TITRE_1_SIZE 34
#define FONT_TITRE_2_SIZE 28
#define NB_FONTS 3

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
 * @return Une structure contenant les lignes de texte découpées
 */
MarkdownText charge_markdown(TTF_Font *fonts[NB_FONTS], const char *filename);

WrappedText text_wrapper (TTF_Font *font, const char *text, int x_max) {
    WrappedText wrapped_text;
    wrapped_text.is_erreur = 0;
    wrapped_text.total_height = 0;
    wrapped_text.nb_lines = 0;
    wrapped_text.font = font;
    if (text[0] == '\0'){
        wrapped_text.total_height = TAILLE_INTER_PARAHRAPHE;
        wrapped_text.fist_ligne.hauteur = TAILLE_INTER_PARAHRAPHE;
        wrapped_text.fist_ligne.text = "";
        return wrapped_text;
    }
    Ligne_texte* current_line = &wrapped_text.fist_ligne;
    Ligne_texte* last_line = NULL;
    int space_width, word_width, word_height;
    TTF_SizeText(font, " ", &space_width, NULL);
    char buffer[NB_MAX_CHAR_PAR_LIGNE] = "";
    const char *text_2 = strcpy(malloc(strlen(text)+1), text);
    char *text_copy = strdup(text_2);
    char *word = strtok(text_copy, " ");
    int x = 0;
    
    while (word) {
        if (strcmp(word, "\n") == 0 || strcmp(word, "\\n") == 0) { // Retour à la ligne forcé par le '\n'
            if (buffer[0] == '\0') {
                if (wrapped_text.nb_lines > 0) last_line->hauteur += TAILLE_INTER_PARAHRAPHE;
                wrapped_text.total_height += TAILLE_INTER_PARAHRAPHE;
            } else {
                buffer[strlen(buffer)-1] = '\0'; // Supprimer l'espace inutile car pas de mot suivant
                current_line->text = strdup(buffer);
                current_line->hauteur = word_height;
                wrapped_text.total_height += word_height;
                wrapped_text.nb_lines++;
                last_line = current_line;
                current_line->ligne_suivante = malloc(sizeof(Ligne_texte));
                current_line = current_line->ligne_suivante;

                buffer[0] = '\0';  // Réinitialiser la ligne
            }
            x = 0;
        } else {
            TTF_SizeText(font, word, &word_width, &word_height);
            if (x + word_width > x_max) {
                if (buffer[0] == '\0'){
                    printf("Attention ! La longeur du rectangle dans lequel doit être affiché le texte est trop petite par rapport à la taille d'un (ou plusieurs) mot(s) du texte (faire attention à la taille de la font !)\nLe texte n'a donc pas été affiché !\n");
                    wrapped_text.is_erreur = true;
                    free(text_copy);
                    return wrapped_text;
                }
                buffer[strlen(buffer) - 1] = '\0';
                current_line->text = strdup(buffer);
                current_line->hauteur = word_height;
                wrapped_text.total_height += word_height;
                wrapped_text.nb_lines++;
                last_line = current_line;
                current_line->ligne_suivante = malloc(sizeof(Ligne_texte));
                current_line = current_line->ligne_suivante;
                buffer[0] = '\0';
                x = 0;
            }
            if (strlen(buffer) + strlen(word) > NB_MAX_CHAR_PAR_LIGNE) {
                printf("Attention ! Une ligne du texte dépace le nombre maximal de caractères par ligne !\nLe texte n'a donc pas été affiché !\n");
                wrapped_text.is_erreur = true;
                free(text_copy);
                return wrapped_text;
            }
            strcat(buffer, word);
            strcat(buffer, " ");
            x += word_width + space_width;
        }
        word = strtok(NULL, " ");
    }
    if (buffer[0] != '\0') {
        current_line->text = strdup(buffer);
        current_line->hauteur = word_height;
        wrapped_text.total_height += word_height;
        wrapped_text.nb_lines++;
        last_line = current_line;
        current_line->ligne_suivante = malloc(sizeof(Ligne_texte));
        current_line = current_line->ligne_suivante;
    }
    free(text_copy);
    return wrapped_text;
}

void render_text_wrapped(SDL_Renderer *renderer, WrappedText wrapped_text, SDL_Rect rect, SDL_Color color, TextAlignX align_x, TextAlignY align_y) {
    if (wrapped_text.is_erreur) return;
    int y = rect.y;
    if (align_y == ALIGN_MIDDLE_Y) y += (rect.h - wrapped_text.total_height) / 2;
    else if (align_y == ALIGN_BOTTOM) y += rect.h - wrapped_text.total_height;
    Ligne_texte* current_line = &wrapped_text.fist_ligne;

    for (int i = 0; i < wrapped_text.nb_lines; i++) {
        SDL_Surface *surface = TTF_RenderUTF8_Blended(wrapped_text.font, current_line->text, color);
        SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_Rect dst = {rect.x, y, surface->w, surface->h};
        if (align_x == ALIGN_CENTER_X) dst.x += (rect.w - surface->w) / 2;
        else if (align_x == ALIGN_RIGHT) dst.x += rect.w - surface->w;
        SDL_RenderCopy(renderer, texture, NULL, &dst);
        SDL_FreeSurface(surface);
        SDL_DestroyTexture(texture);
        y += current_line->hauteur;
        current_line = current_line->ligne_suivante;
    }
}

int compter_nb_lignes (const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        printf("Erreur : Impossible d'ouvrir le fichier %s\n", filename);
        return 0;
    }
    int nb_lines = 0;
    char line[1024];
    while (fgets(line, sizeof(line), file)) {
        nb_lines++;
    }
    fclose(file);
    return nb_lines;
}

MarkdownText charge_markdown(TTF_Font *fonts[NB_FONTS], const char *filename) {
    MarkdownText md_txt;
    FILE *file = fopen(filename, "r");
    if (!file) {
        printf("Erreur : Impossible d'ouvrir le fichier %s\n", filename);
        return md_txt;
    }
    int nb_lignes = compter_nb_lignes(filename);
    if (nb_lignes == 0) return md_txt;
    char line[NB_MAX_CHAR_PAR_LIGNE];
    Line* current_line = &md_txt.fist_line;
    Line* last_line = NULL;
    int y_offset = 0;
    while (fgets(line, sizeof(line), file)) {
        if (strlen(line) > NB_MAX_CHAR_PAR_LIGNE){
            printf("Attention ! Une ligne du fichier dépace le nombre maximal de caractères par ligne !\nLe fichier markdown n'a donc pas été chargé !\n");
            fclose(file);
            return md_txt;
        }
        current_line->y_position = y_offset;
        current_line->is_title = 0;
        current_line->is_list = 0;
        line[strlen(line)-1] = '\0'; // suppression du retour à la ligne du fichier
        if (line[0] == '#') {
            current_line->is_title = 1;
            for (int i = 1; i < NB_FONTS-1; i++) {
                if (line[i] == '#') current_line->is_title++;
                else break;
            }
            if (line[current_line->is_title] == '#') { // Erreur : titre trop grand
                current_line->wrapped_text = text_wrapper(fonts[0], line, FEN_X - 2*MARGE);
            } else { // C'est bien un titre connu
                current_line->wrapped_text = text_wrapper(fonts[current_line->is_title], line + current_line->is_title + 1, FEN_X - 2*MARGE);
            }
        } else {
            if (strncmp(line, "- ", 2) == 0){
                current_line->is_list = 1;
                char *ele_list = malloc(strlen(line) + 4);  // +4 pour "• " et le '\0'
                if (!ele_list) {
                    perror("malloc");
                    exit(1);
                }
                sprintf(ele_list, "• %s", line + 2);
                current_line->wrapped_text = text_wrapper(fonts[0], ele_list, FEN_X - 2*MARGE);
            } else {
                if (strncmp(line, "* ", 2) == 0) {
                    current_line->is_list = 2;
                    char *ele_list = malloc(strlen(line) + 4 + 10);
                    if (!ele_list) {
                        perror("malloc");
                        exit(1);
                    }
                
                    if (last_line == NULL || last_line->is_list != 2) {
                        snprintf(ele_list, strlen(line) + 4, "1. %s", line + 2);
                    } else {
                        int last_num = 1;  // Valeur par défaut
                        sscanf(last_line->wrapped_text.fist_ligne.text, "%d.", &last_num);  // Extraction du numéro de liste
                        snprintf(ele_list, strlen(line) + 4 + 10, "%d. %s", last_num + 1, line + 2);
                    }
                
                    current_line->wrapped_text = text_wrapper(fonts[0], ele_list, FEN_X - 2*MARGE);
                    free(ele_list);
                } else {
                    current_line->wrapped_text = text_wrapper(fonts[0], line, FEN_X - 2*MARGE);
                }
            }
        }
        y_offset += current_line->wrapped_text.total_height;
        current_line->ligne_suivante = malloc(sizeof(Line));
        last_line = current_line;
        current_line = current_line->ligne_suivante;
    }
    md_txt.nb_lines = nb_lignes;
    md_txt.total_height = y_offset;
    fclose(file);
    return md_txt;
}

void render_markdown(SDL_Renderer *renderer, MarkdownText* md_txt, int scroll_offset) {
    SDL_Color white = {255, 255, 255, 255};
    Line* current_line = &md_txt->fist_line;
    for (int i = 0; i < md_txt->nb_lines; i++) {
        if (current_line->y_position + current_line->wrapped_text.total_height - scroll_offset < 0){
            current_line = current_line->ligne_suivante;
            continue;
        }
        if (current_line->y_position - scroll_offset > FEN_Y) break;
        SDL_Rect rect = {MARGE, current_line->y_position - scroll_offset, FEN_X - 2*MARGE, FEN_Y};
        TextAlignX align_x = current_line->is_list ? ALIGN_LEFT : ALIGN_CENTER_X;
        render_text_wrapped(renderer, current_line->wrapped_text, rect, white, align_x, ALIGN_TOP);
        current_line = current_line->ligne_suivante;
    }
}

void free_WrappedText (WrappedText WrappedText){
    Ligne_texte* current_line = &WrappedText.fist_ligne;
    Ligne_texte* next_line;
    for (int i = 0; i < WrappedText.nb_lines; i++) {
        next_line = current_line->ligne_suivante;
        free(current_line->text);
        free(current_line);
        current_line = next_line;
    }
}

void free_MarkdownText (MarkdownText* md_txt){
    Line* current_line = &md_txt->fist_line;
    Line* next_line;
    for (int i = 0; i < md_txt->nb_lines; i++) {
        next_line = current_line->ligne_suivante;
        free_WrappedText(current_line->wrapped_text);
        free(current_line);
        current_line = next_line;
    }
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: %s <fichier_markdown>\n", argv[0]);
        return 1;
    }

    SDL_Init(SDL_INIT_VIDEO);
    TTF_Init();
    SDL_Window *window = SDL_CreateWindow("Markdown Viewer", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, FEN_X, FEN_Y, SDL_WINDOW_SHOWN);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    
    TTF_Font *font = TTF_OpenFont("Ressources/Fonts/DejaVuSans-Bold.ttf", 24);
    TTF_Font *font_title = TTF_OpenFont("Ressources/Fonts/DejaVuSans-Bold.ttf", 40);
    TTF_Font *font_title_2 = TTF_OpenFont("Ressources/Fonts/DejaVuSans-Bold.ttf", 32);
    TTF_Font *fonts [NB_FONTS] = {font, font_title, font_title_2};

    MarkdownText md_txt = charge_markdown(fonts, argv[1]);

    int scroll_offset = 0;
    int running = 1;
    SDL_Event event;

    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = 0;
            } else if (event.type == SDL_MOUSEWHEEL) {
                scroll_offset -= event.wheel.y * SCROLL_SPEED;
                if (scroll_offset < 0) scroll_offset = 0;
                if (scroll_offset > md_txt.total_height - md_txt.fist_line.wrapped_text.total_height) scroll_offset = md_txt.total_height - md_txt.fist_line.wrapped_text.total_height;
            }
        }
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        render_markdown(renderer, &md_txt, scroll_offset);
        SDL_RenderPresent(renderer);
    }
    
    TTF_CloseFont(font);
    TTF_CloseFont(font_title);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();
    return 0;
}

