{
    "theme": {
        "background_color": { "r": 30, "g": 30, "b": 30, "a": 255 }
    },
    "buttons": [
        {
            "id": "play",
            "text": "Jouer",
            "color": { "r": 255, "g": 0, "b": 0, "a": 255 }
        },
        {
            "id": "settings",
            "text": "Options",
            "color": { "r": 0, "g": 255, "b": 0, "a": 255 }
        }
    ]
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

