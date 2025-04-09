#include "ressources.h"

TTF_Font* fonts[NB_FONTS] = {NULL};
Colors* colors = NULL;
Dimention dimention = _2D;
Langue langue = FR;
Tous_les_JSON tous_les_JSON;
Message message;
Probleme probleme;
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

void free_font (TTF_Font* font[NB_FONTS]) {
    for (int i = 0; i < NB_FONTS; i++) {
        if (font[i] != NULL) {
            TTF_CloseFont(font[i]);
            font[i] = NULL;
        }
    }
}

void init_const_message(SDL_Renderer* ren){
    message.temps_affichage = 3;
    message.button_base.is_survolable = 0;
    message.button_base.color_base = (SDL_Color) {255,0,0,255};
    message.button_base.radius = 15;
    message.button_base.font_text = fonts[4];
    message.button_base.color_text = (SDL_Color) {255,255,255,255};

    message.boutton_quitter.image = load_image(ren, "Icons/croix.png");
    message.boutton_quitter.is_survolable = 1;
    message.boutton_quitter.color_base = message.button_base.color_base;
    message.boutton_quitter.color_hover = (SDL_Color){255, 130, 0, 255};
    message.boutton_quitter.hovered = 0;
    message.boutton_quitter.pourcentage_place = 50;
    message.boutton_quitter.taille_bonus_hover_x = 0;
    message.boutton_quitter.taille_bonus_hover_y = 0;
}

void free_message(){
    if (message.boutton_quitter.image != NULL){
        SDL_DestroyTexture(message.boutton_quitter.image);
        message.boutton_quitter.image = NULL;
    }
}

void set_message (const char* text_erreur, SDL_Rect endroit_erreur){
    int marge_x = 10;
    int marge_y = 20;
    int taille_texte_x, taille_texte_y;
    TTF_SizeUTF8(message.button_base.font_text, text_erreur, &taille_texte_x, &taille_texte_y);
    taille_texte_x += taille_texte_y + 2*marge_y;

    message.start_time = time(NULL);
    message.button_base.label = text_erreur;
    message.is_visible = 1;
    message.button_base.rect.x = endroit_erreur.x + (endroit_erreur.w - taille_texte_x)/2 - marge_x;
    message.button_base.rect.y = endroit_erreur.y + endroit_erreur.h;
    message.button_base.rect.w = taille_texte_x + 2*marge_x;
    message.button_base.rect.h = taille_texte_y + 2*marge_y;
    if (message.button_base.rect.x < 0) message.button_base.rect.x = 0;
    if (message.button_base.rect.x + message.button_base.rect.w > FEN_X) message.button_base.rect.x = FEN_X - message.button_base.rect.w;
    if (message.button_base.rect.y < 0) message.button_base.rect.y = 0;
    if (message.button_base.rect.y + message.button_base.rect.h > FEN_Y) message.button_base.rect.y = FEN_Y - message.button_base.rect.h;

    message.boutton_quitter.rect.h = message.button_base.rect.h/2;
    message.boutton_quitter.rect.w = message.boutton_quitter.rect.h;
    message.boutton_quitter.rect.x = message.button_base.rect.x + message.button_base.rect.w - message.boutton_quitter.rect.w;
    message.boutton_quitter.rect.y = message.button_base.rect.y;
    message.boutton_quitter.radius = message.boutton_quitter.rect.w / 3;
}

void set_probleme (int code_erreur){
    ErrorInfo error = get_error_message(code_erreur);

    probleme.text = text_wrapper(fonts[1], error.message, FEN_X/2);
    probleme.is_visible = true;
    if (strcmp(error.severity, "low") == 0){
        probleme.severity = LOW;
    } else if (strcmp(error.severity, "medium") == 0){
        probleme.severity = MEDIUM;
    } else if (strcmp(error.severity, "high") == 0){
        probleme.severity = HIGH;
    } else if (strcmp(error.severity, "critical") == 0){
        probleme.severity = CRITICAL;
    } else {
        probleme.severity = CRITICAL;
    }
}

void affiche_avertissements (SDL_Renderer* ren){
    if (message.is_visible){
        if (time(NULL) - message.start_time > message.temps_affichage){
            message.is_visible = 0;
        } else {
            renderButton(ren, &message.button_base);
            renderImageButton(ren, &message.boutton_quitter);
        }
    }
    if (probleme.is_visible){
        SDL_Color color = {255, 0, 0, 100};
        if (probleme.severity == LOW){
            color = (SDL_Color){0, 255, 0, 100};
        } else if (probleme.severity == MEDIUM){
            color = (SDL_Color){255, 255, 0, 100};
        } else if (probleme.severity == HIGH){
            color = (SDL_Color){255, 128, 0, 100};
        } else if (probleme.severity == CRITICAL){
            color = (SDL_Color){255, 0, 0, 100};
        }
        boxRGBA(ren, 0, 0, FEN_X, FEN_Y, color.r, color.g, color.b, color.a);

        int marge_x = 20;
        int marge_y = 10;
        int probreme_width = 0;
        int w;
        Ligne_texte* current_line = &probleme.text.fist_ligne;
        for (int i = 0; i < probleme.text.nb_lines; i++) {
            TTF_SizeUTF8(fonts[0], current_line->text, &w, NULL);
            if (w > probreme_width) probreme_width = w;
            current_line = current_line->ligne_suivante;
        }

        // Fond de l'en-tête
        SDL_SetRenderDrawColor(ren, color.r, color.g, color.b, 200);
        SDL_Rect fond = {(FEN_X - probreme_width)/2 - marge_x, FEN_Y/2 - probleme.text.total_height/2 - marge_y, probreme_width + 2*marge_x, probleme.text.total_height + 2*marge_y};
        SDL_RenderFillRect(ren, &fond);

        render_text_wrapped(ren, probleme.text, (SDL_Rect){0, 0, FEN_X, FEN_Y}, (SDL_Color){255,255,255,255}, ALIGN_CENTER_X, ALIGN_MIDDLE_Y);
        renderText(ren, "Cliquez pour fermer", FEN_X/2, FEN_Y - 100, (SDL_Color){255,255,255,255}, fonts[6]);
    }
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
    if (langue == IT) return "it";
    if (langue == PT) return "pt";
    if (langue == RU) return "ru";
    if (langue == UK) return "uk";
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
