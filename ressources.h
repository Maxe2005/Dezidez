#ifndef RESSOURCES_H
#define RESSOURCES_H

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>
#include <stdbool.h>
#include <ctype.h>
#include "gestionGraphique.h"
#include "cJSON/cJSON.h"
#include "evaluateur.h"
#include "Analyse_lexicale.h"
#include "syntaxe.h"

#define TAILLE_BANDE_DESCRIPTIONS 40 
#define TAILLE_BANDE_HAUT (TAILLE_BANDE_DESCRIPTIONS + 90)
#define TAILLE_BANDE_DROITE 150

#define NB_FONTS 8
extern TTF_Font* fonts[NB_FONTS];

extern int FEN_X;
extern int FEN_Y;

typedef struct {
    SDL_Color bg;
    SDL_Color axes;
    SDL_Color texte_axes;
    SDL_Color bande_droite;
    SDL_Color bande_haute_expressions;
    SDL_Color bande_haute_description;
    SDL_Color texte_champ_entree;
    SDL_Color texte_descriptifs_bande_haut;
    SDL_Color bg_bandes_expression_1;
    SDL_Color bg_bandes_expression_2;
    SDL_Color bande_bas_de_bande_haut;
    SDL_Color button_new_expression;
    SDL_Color button_new_expression_hover;
} Colors;
extern Colors* colors;

typedef enum {
    _2D, _3D
} Dimention;
extern Dimention dimention;

typedef struct {
    char *message;
    char *severity;
} ErrorInfo;

typedef enum {
    FR, EN, ES, AL, IT, PT, RU, UK
} Langue;
extern Langue langue;

typedef struct {
    cJSON *json_erreurs;
    cJSON *json_textes;
} Tous_les_JSON;
extern Tous_les_JSON tous_les_JSON;

typedef struct {
    Button button_base;
    ImageButton boutton_quitter;
    int temps_affichage; // en secondes
    bool is_visible;
    time_t start_time;
} Message;

typedef enum {
    LOW, MEDIUM, HIGH, CRITICAL
} Severity;

typedef struct {
    WrappedText text;
    bool is_visible;
    Severity severity;
} Probleme;

/**
 * Initialise les différentes polices de caractère avec les tailles correspondantes
 * @param font Le tableau de police vide à remplire
 */
void init_font (TTF_Font* font[NB_FONTS]);

/**
 * Libère la mémoire allouée pour les polices de caractère
 * @param font Le tableau de police à libérer
 * @note Il est important de libérer la mémoire allouée pour les polices de caractère
 *       pour éviter les fuites de mémoire. Cette fonction doit être appelée
 *       lorsque les polices ne sont plus nécessaires.
 */
void free_font (TTF_Font* font[NB_FONTS]);

/**
 * Initialisation des constantes pour la structure message
 * @param ren Un pointeur sur une structure contenant l'état du rendu
 */
void init_const_message(SDL_Renderer* ren);

/**
 * Prépare le message d'erreur à afficher
 * @param text_erreur message d'erreur affiché
 * @param endroit_erreur rectangle où l'erreur a été enregestré
 */
void set_message (const char* text_erreur, SDL_Rect endroit_erreur);

/**
 * Prépare le problème à afficher
 * @param code_erreur Le code d'erreur correspondant au problème à afficher
 */
void set_probleme (int code_erreur);

/**
 * Affiche le message d'erreur des champs des bornes d'entrées s'il y en a un et le message problème s'il y en a un
 * @param ren Un pointeur sur une structure contenant l'état du rendu
 */
void affiche_avertissements (SDL_Renderer* ren);

/**
 * Change le mode de couleur
 * @param color_mode Le mode de couleur à appliquer
 */
void change_color_mode (int color_mode);

/**
 * Génère un nombre aléatoire compris entre les valeurs min et max (incluses).
 * @param min La valeur minimale possible (borne incluse).
 * @param max La valeur maximale possible (borne incluse).
 * @return Un entier aléatoire compris entre min et max.
 */
int nb_alea(int min, int max);

/**
 * Renvoie la langue actuelle sous forme de chaîne de caractères
 * @return "fr" si la langue est le français, "en" si la langue est l'anglais, "Problème de langue" sinon
 */
char* get_lang_str();

/**
 * Lit un fichier JSON et renvoie un pointeur sur la structure cJSON correspondante
 * @param filename Le nom du fichier à lire
 * @return Un pointeur sur la structure cJSON correspondante
 */
cJSON* read_file_json (const char *filename);

/**
 * Renvoie un message d'erreur en fonction du code d'erreur et du JSON correspondant
 * @param code Le code d'erreur
 * @return Un message d'erreur
 */
ErrorInfo get_error_message(int code);

/**
 * Initialise tous les fichiers JSON
 */
void init_tous_les_json ();

/**
 * Renvoie le texte correspondant à une entité et un identifiant donnés dans le fichier JSON "textes.json"
 * @param entitee_name Le nom de l'entité
 * @param id L'identifiant de l'entité
 * @return Le texte correspondant à l'entité et l'identifiant donnés
 */
char* get_texte (const char* entitee_name, const char* id);

/**
 * Libère la mémoire allouée pour tous les fichiers JSON
 */
void free_tous_les_json ();

#include "color_picker.h"

#endif
