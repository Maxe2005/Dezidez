#include "gestionGraphique.h"


void initSDL(){
if(SDL_Init(SDL_INIT_VIDEO) < 0)  // initialisation de la SDL
    {
    	printf("Erreur d'initialisation de la SDL : %s", SDL_GetError());
    	SDL_Quit();
    }
}

SDL_Window * createWindow(char winName[], int winWidth, int winHeigth){
    if (SDL_Init(SDL_INIT_VIDEO) != 0){
			printf("SDL_Init Error\n");
    }
    SDL_Window *win = SDL_CreateWindow(winName, SDL_WINDOWPOS_CENTERED,
    	SDL_WINDOWPOS_CENTERED, winWidth, winHeigth, SDL_WINDOW_RESIZABLE);
    if (win == NULL){
		printf("SDL_CreateWindow Error\n");
		SDL_Quit();
    }
    return win;
}

SDL_Renderer * createRenderer(SDL_Window *win){
	SDL_Renderer *ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (ren == NULL){
		SDL_DestroyWindow(win);
		printf("SDL_CreateRenderer Error");
		SDL_Quit();
	}
	return ren;
}

SDL_Texture* loadTexture(const char* file, SDL_Renderer *ren){
	SDL_Texture *texture = NULL;
	SDL_Surface *loadedImage = SDL_LoadBMP(file);
	if (loadedImage != NULL){
		texture = SDL_CreateTextureFromSurface(ren, loadedImage);
		SDL_FreeSurface(loadedImage);
		if (texture == NULL){
			printf("Error : CreateTextureFromSurface\n");
		}
	}
	else {
		printf("Error : LoadBMP\n");
	}
	return texture;
}

void renderTexture(SDL_Texture *tex, SDL_Renderer *ren, int x, int y, int width, int height){
	SDL_Rect dst;
	dst.x = x;
	dst.y = y;
	dst.w = width;
	dst.h = height;
	SDL_RenderCopy(ren, tex, NULL, &dst);
}

void initTTF(){
	if(TTF_Init()==-1) {
	    printf("TTF_Init: %s\n", TTF_GetError());
	    SDL_Quit();
	}
}

TTF_Font* createFont(char fontPath[], int fontSize){
	TTF_Font* font = TTF_OpenFont(fontPath, fontSize); 
	if(font==NULL){
    	printf("TTF_OpenFont: %s\n", TTF_GetError());
		SDL_Quit();
	}
	return font; 
}

void printText(int x,int y,char *text, int wsize, int hsize,TTF_Font* font, SDL_Color color, SDL_Renderer *ren){
		SDL_Surface* surfaceMessage = TTF_RenderText_Solid(font, text, color); 
		SDL_Texture* Message = SDL_CreateTextureFromSurface(ren, surfaceMessage); 
		SDL_Rect Message_rect; 
		Message_rect.x = x;  
		Message_rect.y = y; 
		Message_rect.w = wsize;
		Message_rect.h = hsize;
		SDL_RenderCopy(ren, Message, NULL, &Message_rect); 
		SDL_FreeSurface(surfaceMessage);
		SDL_DestroyTexture(Message);
}

void updateDisplay(SDL_Renderer *ren){
	SDL_RenderPresent(ren);
}

void clearRenderer(SDL_Renderer *ren){
	SDL_RenderClear(ren);
}

void QuitSDL(SDL_Window *win, SDL_Renderer *ren){
	SDL_DestroyWindow(win);
	SDL_DestroyRenderer(ren);
	SDL_Quit(); 
}



void renderButton(SDL_Renderer *renderer, Button *button) {
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    if (button->rect.w < 2*button->radius) button->radius = button->rect.w / 2;
    if (button->rect.h < 2*button->radius) button->radius = button->rect.h / 2;
    if (button->is_survolable && button->hovered) {
        SDL_Rect tmp_rect;
        tmp_rect.w = button->rect.w + button->taille_bonus_hover_x;
        tmp_rect.h = button->rect.h + button->taille_bonus_hover_y;
        tmp_rect.x = button->rect.x - button->taille_bonus_hover_x/2;
        tmp_rect.y = button->rect.y - button->taille_bonus_hover_y/2;

        // Dessiner le rectangle avec des coins arrondis
        roundedBoxRGBA(renderer, tmp_rect.x, tmp_rect.y, tmp_rect.x + tmp_rect.w, tmp_rect.y + tmp_rect.h, button->radius, button->color_hover.r, button->color_hover.g, button->color_hover.b, button->color_hover.a);
        renderText(renderer, button->label, tmp_rect.x + tmp_rect.w/2, tmp_rect.y + tmp_rect.h/2, button->color_text, button->font_text_hover);
    } else {
        // Dessiner le rectangle avec des coins arrondis
        roundedBoxRGBA(renderer, button->rect.x, button->rect.y, button->rect.x + button->rect.w, button->rect.y + button->rect.h, button->radius, button->color_base.r, button->color_base.g, button->color_base.b, button->color_base.a);
        renderText(renderer, button->label, button->rect.x + button->rect.w/2, button->rect.y + button->rect.h/2, button->color_text, button->font_text);
    }
}

void draw_image (SDL_Renderer *renderer, ImageButton *button, SDL_Rect rect){
    SDL_Rect tmp_rect;
    tmp_rect.w = rect.w * button->pourcentage_place / 100;
    tmp_rect.h = rect.h * button->pourcentage_place / 100;
    tmp_rect.x = rect.x + (rect.w - tmp_rect.w)/2;
    tmp_rect.y = rect.y + (rect.h - tmp_rect.h)/2;
    SDL_RenderCopy(renderer, button->image, NULL, &tmp_rect);
}

void renderImageButton(SDL_Renderer *renderer, ImageButton *button) {
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    if (button->is_survolable && button->hovered) {
        SDL_Rect tmp_rect;
        tmp_rect.w = button->rect.w + button->taille_bonus_hover_x;
        tmp_rect.h = button->rect.h + button->taille_bonus_hover_y;
        tmp_rect.x = button->rect.x - button->taille_bonus_hover_x/2;
        tmp_rect.y = button->rect.y - button->taille_bonus_hover_y/2;

        // Dessiner le rectangle avec des coins arrondis
        roundedBoxRGBA(renderer, tmp_rect.x, tmp_rect.y, tmp_rect.x + tmp_rect.w, tmp_rect.y + tmp_rect.h, button->radius, button->color_hover.r, button->color_hover.g, button->color_hover.b, button->color_hover.a);
        draw_image(renderer, button, tmp_rect);
    } else {
        // Dessiner le rectangle avec des coins arrondis
        roundedBoxRGBA(renderer, button->rect.x, button->rect.y, button->rect.x + button->rect.w, button->rect.y + button->rect.h, button->radius, button->color_base.r, button->color_base.g, button->color_base.b, button->color_base.a);
        draw_image(renderer, button, button->rect);
    }
}

SDL_Texture* load_image(SDL_Renderer* renderer, const char* filename) {
    char path[100] = "Ressources/";
    strcat(path, filename);
    SDL_Texture* texture = NULL;
    SDL_Surface* loadedSurface = IMG_Load(path);
    if (loadedSurface == NULL) {
        printf("Unable to load image! SDL_image Error: %s\n", IMG_GetError());
    } else {
        texture = SDL_CreateTextureFromSurface(renderer, loadedSurface);
        SDL_FreeSurface(loadedSurface);
        if (texture == NULL) {
            printf("Unable to create texture from %s! SDL Error: %s\n", path, SDL_GetError());
        }
    }
    return texture;
}

void renderHeader(SDL_Renderer *renderer, char *titre, TTF_Font *font, int fen_x, int header_height) {
    // Fond de l'en-tête
    SDL_SetRenderDrawColor(renderer, 50, 50, 50, 255);
    SDL_Rect header = {0, 0, fen_x, header_height};
    SDL_RenderFillRect(renderer, &header);

    // Texte "titre"
    SDL_Surface *surface = TTF_RenderUTF8_Solid(font, titre, (SDL_Color){255, 255, 255, 255});
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_Rect textRect = {fen_x / 2 - surface->w / 2, header_height / 2 - surface->h / 2, surface->w, surface->h};
    
    SDL_RenderCopy(renderer, texture, NULL, &textRect);

    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
}

void renderText(SDL_Renderer *renderer, const char *text, int x, int y, SDL_Color color, TTF_Font *font){
    SDL_Surface *surface =  TTF_RenderUTF8_Solid(font, text, color);
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_Rect textRect = {x - surface->w/2, y - surface->h/2, surface->w, surface->h};

    SDL_RenderCopy(renderer, texture, NULL, &textRect);

    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
}

int is_souris_sur_rectangle (SDL_Rect rect, int x_souris_px, int y_souris_px){
    return x_souris_px >= rect.x && x_souris_px <= rect.x + rect.w &&
                    y_souris_px >= rect.y && y_souris_px <= rect.y + rect.h;
}

void affiche_bande_arrondis_en_bas (SDL_Renderer* renderer, int x1, int y1, int x2, int y2, int radius, SDL_Color color){
    // Dessiner le rectangle principal
    boxRGBA(renderer, x1, y1, x2, y2 - radius, color.r, color.g, color.b, color.a);
    
    // Dessiner le bas arrondi seulement
    filledPieRGBA(renderer, x1 + radius, y2 - radius, radius, 0, 180, color.r, color.g, color.b, color.a);
    filledPieRGBA(renderer, x2 - radius, y2 - radius, radius, 0, 180, color.r, color.g, color.b, color.a);
    boxRGBA(renderer, x1 + radius, y2 - radius, x2 - radius, y2, color.r, color.g, color.b, color.a);
}

void affiche_bande_arrondis_en_haut (SDL_Renderer* renderer, int x1, int y1, int x2, int y2, int radius, SDL_Color color){
    // Dessiner le rectangle principal
    boxRGBA(renderer, x1, y1 + radius, x2, y2, color.r, color.g, color.b, color.a);
    
    // Dessiner le haut arrondi seulement
    filledPieRGBA(renderer, x1 + radius, y1 + radius, radius, 180, 360, color.r, color.g, color.b, color.a);
    filledPieRGBA(renderer, x2 - radius, y1 + radius, radius, 180, 360, color.r, color.g, color.b, color.a);
    boxRGBA(renderer, x1 + radius, y1, x2 - radius, y1 + radius, color.r, color.g, color.b, color.a);
}



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
                    printf("Attention ! La longeur du rectangle dans lequel doit être affiché le texte est trop petite par rapport à la taille d'un (ou plusieurs) mot(s) du texte (faire attention à la taille de la font !)\nLe texte n'a donc pas été affiché !\n\n");
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
        buffer[strlen(buffer) - 1] = '\0';
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

MarkdownText charge_markdown(TTF_Font *fonts[NB_FONTS_MD], const char *filename, int marge) {
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
            for (int i = 1; i < NB_FONTS_MD-1; i++) {
                if (line[i] == '#') current_line->is_title++;
                else break;
            }
            if (line[current_line->is_title] == '#') { // Erreur : titre trop grand
                current_line->wrapped_text = text_wrapper(fonts[0], line, FEN_X - 2*marge);
            } else { // C'est bien un titre connu
                current_line->wrapped_text = text_wrapper(fonts[current_line->is_title], line + current_line->is_title + 1, FEN_X - 2*marge);
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
                current_line->wrapped_text = text_wrapper(fonts[0], ele_list, FEN_X - 2*marge);
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
                
                    current_line->wrapped_text = text_wrapper(fonts[0], ele_list, FEN_X - 2*marge);
                    free(ele_list);
                } else {
                    current_line->wrapped_text = text_wrapper(fonts[0], line, FEN_X - 2*marge);
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

void render_markdown(SDL_Renderer *renderer, MarkdownText* md_txt, int scroll_offset, int marge) {
    SDL_Color white = {255, 255, 255, 255};
    Line* current_line = &md_txt->fist_line;
    for (int i = 0; i < md_txt->nb_lines; i++) {
        if (current_line->y_position + current_line->wrapped_text.total_height - scroll_offset < 0){
            current_line = current_line->ligne_suivante;
            continue;
        }
        if (current_line->y_position - scroll_offset > FEN_Y) break;
        SDL_Rect rect = {marge, current_line->y_position - scroll_offset, FEN_X - 2*marge, FEN_Y};
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
        if (i > 0) free(current_line); // Car la première ligne est dans la structure WrappedText
        current_line = next_line;
    }
}

void free_MarkdownText (MarkdownText* md_txt){
    Line* current_line = &md_txt->fist_line;
    Line* next_line;
    for (int i = 0; i < md_txt->nb_lines; i++) {
        next_line = current_line->ligne_suivante;
        free_WrappedText(current_line->wrapped_text);
        if (i > 0) free(current_line); // Car la première ligne est dans la structure MarkdownText
        current_line = next_line;
    }
}

