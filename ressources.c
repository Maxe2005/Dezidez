#include "ressources.h"

TTF_Font* fonts[NB_FONTS] = {NULL};
Colors* colors = NULL;
Dimention dimention = _2D;
Langue langue = EN;
Tous_les_JSON tous_les_JSON;
int FEN_X = 1100;
int FEN_Y = 800;


void init_font (TTF_Font* font[NB_FONTS]) {
    // ! A faire correspondre avec <NB_FONTS>
    font[0] = createFont("Ressources/Fonts/DejaVuSans-Bold.ttf", 40); //Font de titres
    font[1] = createFont("Ressources/Fonts/DejaVuSans-Bold.ttf", 25); //Font de bouton normal
    font[2] = createFont("Ressources/Fonts/DejaVuSans-Bold.ttf", 30); //Font de bouton avec souris dessus
    font[3] = createFont("Ressources/Fonts/DejaVuSans-Bold.ttf", 35);
    font[4] = createFont("Ressources/Fonts/DejaVuSans-Bold.ttf", 15); //Font de texte descriptif de bande haute
    font[5] = createFont("Ressources/Fonts/DejaVuSans-Bold.ttf", 12); //Font de texte de graduation
    font[6] = createFont("Ressources/Fonts/DejaVuSans-Bold.ttf", 18); //Font de texte de graduation
    font[7] = createFont("Ressources/Fonts/DejaVuSans-Bold.ttf", 20); //Font de texte de graduation
}


void change_color_mode (int color_mode){
    if (color_mode == 0){
        colors->bg = (SDL_Color){255, 255, 255, 255};
        colors->axes = (SDL_Color){0, 0, 0, 255};
        colors->texte_axes = (SDL_Color){0, 0, 0, 255};
    } else {
        colors->bg = (SDL_Color){0, 0, 0, 255};
        colors->axes = (SDL_Color){255, 255, 255, 255};
        colors->texte_axes = (SDL_Color){255, 255, 255, 255};
    }
    colors->bande_droite = (SDL_Color){100, 100, 100, 255};
    colors->bande_haute_expressions = (SDL_Color){200, 200, 200, 255};
    colors->bande_haute_description = (SDL_Color){150, 150, 150, 255};
    colors->texte_champ_entree = (SDL_Color){0, 0, 0, 255};
    colors->texte_descriptifs_bande_haut = (SDL_Color){255, 255, 255, 255};
    colors->bg_bandes_expression_1 = colors->bande_haute_expressions;
    colors->bg_bandes_expression_2 = (SDL_Color){150, 150, 150, 255};
    colors->bande_bas_de_bande_haut = (SDL_Color){200, 200, 200, 255};
    colors->button_new_expression = (SDL_Color){125, 125, 125, 255};
    colors->button_new_expression_hover = (SDL_Color){150, 150, 150, 255};
}


int nb_alea(int min, int max){
    return min + rand() % (max-min +1);
}

char* get_lang_str(){
    if (langue == FR) return "fr";
    if (langue == EN) return "en";
    if (langue == ES) return "es";
    if (langue == AL) return "al";
    return "Problème de langue";
}

cJSON* read_file_json (const char *filename) {
    FILE *file = fopen(filename, "rb");
    if (!file) {
        perror("Erreur lors de l'ouverture du fichier JSON");
        return NULL;
    }

    fseek(file, 0, SEEK_END);
    long length = ftell(file);
    rewind(file);

    char *buffer = malloc(length + 1);
    if (!buffer) {
        fclose(file);
        return NULL;
    }

    fread(buffer, 1, length, file);
    buffer[length] = '\0';  // Ajouter le terminateur de chaîne

    fclose(file);

    if (!buffer) {
        return NULL;
    }

    cJSON *json = cJSON_Parse(buffer);
    free(buffer);

    if (!json) {
        fprintf(stderr, "Erreur de parsing JSON\n");
        return NULL;
    }
    return json;
}

void init_tous_les_json () {
    tous_les_JSON.json_erreurs = read_file_json("Ressources/JSON/erreurs.json");
    tous_les_JSON.json_textes = read_file_json("Ressources/JSON/textes.json");
}

ErrorInfo get_error_message(int code) {
    ErrorInfo error_info;
    error_info.message = "Erreur inconnue";
    error_info.severity = "unknown";

    char code_str[10];
    sprintf(code_str, "%d", code);  // Convertit l'entier en chaîne

    cJSON *lang_json = cJSON_GetObjectItem(tous_les_JSON.json_erreurs, get_lang_str());
    if (!lang_json) return error_info; // Langue non trouvée

    cJSON *error_json = cJSON_GetObjectItem(lang_json, code_str);
    if (!error_json) return error_info; // Code erreur non trouvé

    cJSON *message_json = cJSON_GetObjectItem(error_json, "message");
    cJSON *severity_json = cJSON_GetObjectItem(error_json, "severity");

    if (message_json) {
        error_info.message = strdup(message_json->valuestring);
    }
    if (severity_json) {
        error_info.severity = strdup(severity_json->valuestring);
    }
    return error_info;
}

char* get_texte (const char* entitee_name, const char* id){
    cJSON *entitee = cJSON_GetObjectItem(tous_les_JSON.json_textes, entitee_name);
    if (!entitee) return NULL; // Langue non trouvée

    cJSON *objet = cJSON_GetObjectItem(entitee, id);
    if (!objet) return NULL; // Entitée non trouvée

    cJSON *lang = cJSON_GetObjectItem(objet, get_lang_str());
    if (!lang) return NULL; // ID non trouvé

    return strdup(lang->valuestring);
}

void free_tous_les_json () {
    cJSON_Delete(tous_les_JSON.json_erreurs);
    cJSON_Delete(tous_les_JSON.json_textes);
}
