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
    SDL_WINDOWPOS_CENTERED, winWidth, winHeigth, SDL_WINDOW_SHOWN);
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

char processKeyboard(int *running){
	char pdirection = ' ';
	SDL_Event e;
	int key;
	while (SDL_PollEvent(&e)){
		switch(e.type){
			case SDL_KEYUP://SDL_KEYDOWN:
				key=e.key.keysym.sym;
				switch(key){
					case SDLK_p:
						pdirection='L'; // lancement du jeu
						break;
					case SDLK_SPACE:
						pdirection='L'; // lancement du jeu
						break;
					case SDLK_BACKSPACE:
						pdirection='M'; // retour à l'accueil
						break;
					case SDLK_m:
						pdirection='m'; // aller au menu des musiques
						break;
					case SDLK_n:
						pdirection='n'; // aller au menu des niveaux
						break;
					case SDLK_r:
						pdirection='r'; // aller à l'écran des remerciements
						break;
					case SDLK_c:
						pdirection='c'; // aller au créateur de map
						break;
					case SDLK_LEFT:
						pdirection='g';
						break;
					case SDLK_q:
						pdirection='g';
						break;
					case SDLK_RIGHT:
						pdirection='d';
						break;
					case SDLK_d:
						pdirection='d';
						break;
					case SDLK_UP:
						pdirection='h';
						break;
					case SDLK_z:
						pdirection='h';
						break;
					case SDLK_DOWN:
						pdirection='b';
						break;
					case SDLK_s:
						pdirection='b';
						break;
					case SDLK_b: // Pour quitter la boucle
						*running = 0;
						break;
					default:
						break;
				}
				break;
			case SDL_QUIT: // L'utilisateur a cliqué sur la croix ou demandé la fermeture
				*running = 0;
				break;
		}
	}
	return pdirection;
}

void QuitSDL(SDL_Window *win, SDL_Renderer *ren){
	SDL_DestroyWindow(win);
	SDL_DestroyRenderer(ren);
	SDL_Quit(); 
}

