#include "ressources.h"

TTF_Font* fonts[NB_FONTS] = {NULL};
Colors* colors = NULL;
Langue langue = FR;
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
}

int nb_alea(int min, int max){
    return min + rand() % (max-min +1);
}

char* get_lang_str(){
    if (langue == FR) return "fr";
    if (langue == EN) return "en";
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
}

ErrorInfo get_error_message(int code) {
    ErrorInfo error_info;
    strcpy(error_info.message, "Erreur inconnue");
    strcpy(error_info.severity, "unknown");

    char code_str[10];
    sprintf(code_str, "%d", code);  // Convertit l'entier en chaîne

    cJSON *lang_json = cJSON_GetObjectItem(tous_les_JSON.json_erreurs, get_lang_str());
    if (!lang_json) return error_info; // Langue non trouvée

    cJSON *error_json = cJSON_GetObjectItem(lang_json, code_str);
    if (!error_json) return error_info; // Code erreur non trouvé

    cJSON *message_json = cJSON_GetObjectItem(error_json, "message");
    cJSON *severity_json = cJSON_GetObjectItem(error_json, "severity");

    if (message_json) {
        free(error_info.message); // Libérer l'ancienne valeur
        error_info.message = strdup(message_json->valuestring);
    }
    if (severity_json) {
        free(error_info.severity);
        error_info.severity = strdup(severity_json->valuestring);
    }
    return error_info;
}

// exemple :
typedef struct {
    char *id;
    char *text;
    SDL_Color color;
} Button_2;

typedef struct {
    SDL_Color background_color;
    Button_2 *buttons;
    int button_count;
} AppSettings;

AppSettings load_settings(const char *filename) {
    AppSettings settings = {0};
    cJSON *json = read_file_json(filename);
    if (!json) return settings;

    // Charger la couleur du fond
    cJSON *theme = cJSON_GetObjectItem(json, "theme");
    if (theme) {
        cJSON *bg_color = cJSON_GetObjectItem(theme, "background_color");
        if (bg_color) {
            settings.background_color.r = cJSON_GetObjectItem(bg_color, "r")->valueint;
            settings.background_color.g = cJSON_GetObjectItem(bg_color, "g")->valueint;
            settings.background_color.b = cJSON_GetObjectItem(bg_color, "b")->valueint;
            settings.background_color.a = cJSON_GetObjectItem(bg_color, "a")->valueint;
        }
    }

    // Charger les boutons
    cJSON *buttons_array = cJSON_GetObjectItem(json, "buttons");
    if (buttons_array) {
        settings.button_count = cJSON_GetArraySize(buttons_array);
        settings.buttons = malloc(settings.button_count * sizeof(Button_2));

        for (int i = 0; i < settings.button_count; i++) {
            cJSON *button_json = cJSON_GetArrayItem(buttons_array, i);
            settings.buttons[i].id = strdup(cJSON_GetObjectItem(button_json, "id")->valuestring);
            settings.buttons[i].text = strdup(cJSON_GetObjectItem(button_json, "text")->valuestring);

            cJSON *color_json = cJSON_GetObjectItem(button_json, "color");
            settings.buttons[i].color.r = cJSON_GetObjectItem(color_json, "r")->valueint;
            settings.buttons[i].color.g = cJSON_GetObjectItem(color_json, "g")->valueint;
            settings.buttons[i].color.b = cJSON_GetObjectItem(color_json, "b")->valueint;
            settings.buttons[i].color.a = cJSON_GetObjectItem(color_json, "a")->valueint;
        }
    }

    cJSON_Delete(json);
    return settings;
}
// Fonction pour libérer la mémoire des paramètres
void free_settings(AppSettings *settings) {
    for (int i = 0; i < settings->button_count; i++) {
        free(settings->buttons[i].id);
        free(settings->buttons[i].text);
    }
    free(settings->buttons);
}
void save_settings(const char *filename, const AppSettings *settings) {
    cJSON *json = cJSON_CreateObject();

    // Enregistrement de la couleur du fond
    cJSON *theme = cJSON_CreateObject();
    cJSON *bg_color = cJSON_CreateObject();
    cJSON_AddNumberToObject(bg_color, "r", settings->background_color.r);
    cJSON_AddNumberToObject(bg_color, "g", settings->background_color.g);
    cJSON_AddNumberToObject(bg_color, "b", settings->background_color.b);
    cJSON_AddNumberToObject(bg_color, "a", settings->background_color.a);
    cJSON_AddItemToObject(theme, "background_color", bg_color);
    cJSON_AddItemToObject(json, "theme", theme);

    // Enregistrement des boutons
    cJSON *buttons_array = cJSON_CreateArray();
    for (int i = 0; i < settings->button_count; i++) {
        cJSON *button_json = cJSON_CreateObject();
        cJSON_AddStringToObject(button_json, "id", settings->buttons[i].id);
        cJSON_AddStringToObject(button_json, "text", settings->buttons[i].text);

        cJSON *color_json = cJSON_CreateObject();
        cJSON_AddNumberToObject(color_json, "r", settings->buttons[i].color.r);
        cJSON_AddNumberToObject(color_json, "g", settings->buttons[i].color.g);
        cJSON_AddNumberToObject(color_json, "b", settings->buttons[i].color.b);
        cJSON_AddNumberToObject(color_json, "a", settings->buttons[i].color.a);
        cJSON_AddItemToObject(button_json, "color", color_json);

        cJSON_AddItemToArray(buttons_array, button_json);
    }
    cJSON_AddItemToObject(json, "buttons", buttons_array);

    // Sauvegarde dans un fichier
    char *json_string = cJSON_Print(json);
    FILE *file = fopen(filename, "w");
    if (file) {
        fprintf(file, "%s", json_string);
        fclose(file);
    }

    free(json_string);
    cJSON_Delete(json);
}

