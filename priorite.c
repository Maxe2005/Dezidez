#include "priorite.h"

Node arbrevide(){
    Node arbre = (Node){
        .pjeton_preced = NULL,
        .pjeton_suiv = NULL,
        .jeton = FIN
    };
    return arbre;
}

Node operateur( typejeton *tab, int debut, int fin, typeerreur *erreur){
    printf("debut =%d fin %d\n", debut,fin);
    affiche_liste_jeton(tab, debut, fin);
    int  indoputile = minIndice(tab, debut, fin, erreur);
    printf("voir indoputile %d \n", indoputile);
    if (*erreur != 0){
        printf("erreur != 0 = %d\n", *erreur);
        return arbrevide();
    }
    // if (parenthese(debut, fin, tab)!= true){
    //     *erreur = PROBLEMES_NOMBRE_PARENTHESES;
    //     return arbrevide(); 
    // }
    if (debut>fin){
        printf("debut>fin\n");
        *erreur = MEMBRE_VIDE;
        return arbrevide();
    }
    if (indoputile != -1){
        printf("indoputile != -1\n");
        Node Fg= operateur(tab, debut, indoputile -1, erreur);
        Node Fd = operateur(tab,indoputile + 1, fin, erreur);
        Node arbre = (Node){
            .pjeton_preced = &Fg,
            .pjeton_suiv = &Fd,
            .jeton = tab[indoputile]
        };
        return arbre;
    }
    else{
        switch (tab[debut].lexem){
            case FONCTION:
                if (tab[debut + 1].lexem != PAR_OUV){
                        *erreur = PROBLEME_PARENTHESES_FONCTIONS;
                        return arbrevide();
                }
                Node Fg = operateur(tab, debut + 2, fin - 1, erreur);
                Node arbre = (Node){
                    .pjeton_preced = &Fg,
                    .jeton = tab[indoputile]
                }; 
                if (tab[debut + 2].lexem !=(FONCTION||REEL||VARIABLE)){
                    *erreur = MEMBRE_VIDE;
                    return arbrevide();
                }
                break;
            case PAR_OUV:
                if (tab[fin].lexem != PAR_FERM){
                    *erreur = MEMBRE_VIDE;
                    return arbrevide();
                }
                if (tab[debut + 2].lexem !=(FONCTION||REEL||VARIABLE)){
                    *erreur = MEMBRE_VIDE;
                    return arbrevide();
                }
                else{
                    Fg = operateur(tab, debut + 1, fin - 1, erreur);
                    Node arbre = (Node){
                        .pjeton_preced = &Fg,
                        .jeton = tab[indoputile]
                    };
                }
                break;
            case PAR_FERM:
                *erreur = PARENTHESE_FERMEE_1_ER_JETON;
                return arbrevide(); 
            case REEL:
                if (fin - debut == 0){
                    // Node Fg = arbrevide();
                    Node arbre = (Node){
                        .pjeton_preced = NULL,
                        .pjeton_suiv = NULL,
                        .jeton = tab[debut]
                       };
                }
                else{
                    *erreur = PROBLEME_APRES_REEL;
                    return arbrevide();
                }
        } 
    }
}

int minIndice(typejeton *tab,  int debut, int fin, typeerreur *erreur){
    int prof = 0;
    int operateurutile = 0;
    int profoperutile =0;
    int indoputile = -1;
    bool continuer = true;
    for (int i = debut; i< fin+1; i++){
        switch (tab[i].lexem){
            case OPERATEUR:
                if (prof == 0){
                    if (operateurutile < tab[i].lexem){
                        profoperutile = prof;
                        operateurutile = tab[i].lexem;
                        indoputile = i;
                    }
                }
                break;
                
            case PAR_OUV:
                prof = prof + 1;
                break;

            case PAR_FERM:
                prof = prof - 1;
                break;   

            default:
                break;
            }
        }
    if (&tab[0]){
        continuer = false;
    }
    if (prof!= 0){
        *erreur = PROBLEMES_NOMBRE_PARENTHESES;
        printf("test mindice. prof=%d\n", prof);
        return -1;
    }
    return (indoputile);
}


int calculTaille(typejeton *tab){
    for (int i = 0; i<TAILLE_MAX; i++){
        if (tab[i].lexem == FIN){
            return (i-1);
        }
    }
    return -1;
}

bool parenthese(int debut, int fin, typejeton *tab){
    int nbouv = 0;
    int nbfer = 0;
    for (int i = 0; i < fin - debut + 1; i++){
        if (tab[i].lexem == PAR_OUV){
            nbouv = nbouv + 1;
        }
        if(tab[i].lexem == PAR_FERM){
            nbfer = nbfer + 1;
        }
    }
    printf ("parentheses, nbouv=%d, nbfer=%d\n", nbouv, nbfer);
    // printf(nbouv);
    // printf(nbfer);
    return nbouv == nbfer;
}

Node Syntaxique(typejeton *tab, typeerreur *erreur){
    int fin = calculTaille(tab);
    Node arbrePlein = operateur( tab, 0, fin, erreur);
    return arbrePlein;
}

void affiche_liste_jeton(typejeton *tab, int debut, int fin){
    for(int i=debut;i<fin+1;i++){
        printf(", %d", tab[i].lexem);
    }
    printf("\n");
}

/**
 * Convertit un jeton en chaîne de caractères lisible
 * @param jeton Le jeton à convertir
 * @param buffer Le buffer où stocker la chaîne
 * @param size La taille du buffer
 */
void jeton_to_string(typejeton jeton, char* buffer, size_t size) {
    switch (jeton.lexem) {
        case REEL:
            snprintf(buffer, size, "%.4f", jeton.valeur.reel);
            break;
        case OPERATEUR:
            snprintf(buffer, size, "%s", operateur_string[jeton.valeur.operateur]);
            break;
        case FONCTION:
            snprintf(buffer, size, "%s", fonction_string[jeton.valeur.fonction]);
            break;
        case VARIABLE:
            snprintf(buffer, size, "VAR(%c)", jeton.valeur.variable);
            break;
        case PAR_OUV:
            snprintf(buffer, size, "(");
            break;
        case PAR_FERM:
            snprintf(buffer, size, ")");
            break;
        case BAR_OUV:
            snprintf(buffer, size, "|");
            break;
        case BAR_FERM:
            snprintf(buffer, size, "|");
            break;
        case ABSOLU:
            snprintf(buffer, size, "ABS");
            break;
        case ERREUR:
            snprintf(buffer, size, "ERREUR");
            break;
        case FIN:
            snprintf(buffer, size, "FIN");
            break;
        default:
            snprintf(buffer, size, "???");
    }
}

/**
 * Affiche un arbre de façon récursive
 * @param racine Nœud racine à afficher
 * @param prefixe Préfixe pour l'indentation
 * @param est_dernier Indique si le nœud est le dernier de sa fratrie
 */
void afficher_arbre_rec(Node* racine, char* prefixe, int est_dernier) {
    if (racine == NULL) return;
    
    char nouveau_prefixe[MAX_DEPTH];
    char valeur_jeton[50];
    
    // Afficher le préfixe actuel
    printf("%s", prefixe);
    
    // Afficher le connecteur approprié selon si c'est le dernier élément ou non
    printf("%s", est_dernier ? "└── " : "├── ");
    
    // Convertir le jeton en chaîne et l'afficher
    jeton_to_string(racine->jeton, valeur_jeton, sizeof(valeur_jeton));
    printf("%s [%s]\n", lexem_string[racine->jeton.lexem], valeur_jeton);
    
    // Créer un nouveau préfixe pour les enfants
    snprintf(nouveau_prefixe, MAX_DEPTH, "%s%s", prefixe, est_dernier ? "    " : "│   ");
    
    // Traiter d'abord le nœud précédent (considéré comme le fils gauche)
    if (racine->pjeton_preced != NULL) {
        afficher_arbre_rec(racine->pjeton_preced, nouveau_prefixe, racine->pjeton_suiv == NULL);
    }
    
    // Puis traiter le nœud suivant (considéré comme le fils droit)
    if (racine->pjeton_suiv != NULL) {
        afficher_arbre_rec(racine->pjeton_suiv, nouveau_prefixe, 1);
    }
}

/**
 * Fonction principale pour afficher un arbre
 * @param racine Nœud racine de l'arbre à afficher
 */
void afficher_arbre(Node* racine) {
    if (racine == NULL) {
        printf("Arbre vide\n");
        return;
    }
    
    // Commencer l'affichage récursif avec un préfixe vide
    char prefixe_initial[1] = "";
    printf("Arbre :\n");
    afficher_arbre_rec(racine, prefixe_initial, 1);
}

/**
 * Version alternative avec couleurs ANSI pour un affichage plus joli
 * @param racine Nœud racine de l'arbre à afficher
 */
void afficher_arbre_couleur(Node* racine) {
    if (racine == NULL) {
        printf("Arbre vide\n");
        return;
    }
    
    // Définir les codes de couleur ANSI
    const char* RESET = "\033[0m";
    const char* VERT = "\033[32m";
    const char* BLEU = "\033[34m";
    const char* ROUGE = "\033[31m";
    const char* JAUNE = "\033[33m";
    const char* CYAN = "\033[36m";
    
    // Fonction locale pour choisir la couleur selon le type de lexem
    const char* choisir_couleur(typelexem lexem) {
        switch (lexem) {
            case REEL: return VERT;       // Réels en vert
            case OPERATEUR: return ROUGE; // Opérateurs en rouge
            case FONCTION: return BLEU;   // Fonctions en bleu
            case VARIABLE: return JAUNE;  // Variables en jaune
            default: return CYAN;         // Autres en cyan
        }
    }
    
    // Fonction récursive locale avec couleurs
    void afficher_rec_couleur(Node* noeud, char* prefixe, int est_dernier) {
        if (noeud == NULL) return;
        
        char nouveau_prefixe[MAX_DEPTH];
        char valeur_jeton[50];
        
        // Afficher le préfixe actuel
        printf("%s", prefixe);
        
        // Afficher le connecteur approprié
        printf("%s", est_dernier ? "└── " : "├── ");
        
        // Convertir le jeton en chaîne et l'afficher avec couleur
        jeton_to_string(noeud->jeton, valeur_jeton, sizeof(valeur_jeton));
        const char* couleur = choisir_couleur(noeud->jeton.lexem);
        printf("%s%s%s [%s%s%s]\n", couleur, lexem_string[noeud->jeton.lexem], RESET, couleur, valeur_jeton, RESET);
        
        // Créer un nouveau préfixe pour les enfants
        snprintf(nouveau_prefixe, MAX_DEPTH, "%s%s", prefixe, est_dernier ? "    " : "│   ");
        
        // Traiter d'abord le nœud précédent
        if (noeud->pjeton_preced != NULL) {
            afficher_rec_couleur(noeud->pjeton_preced, nouveau_prefixe, noeud->pjeton_suiv == NULL);
        }
        
        // Puis traiter le nœud suivant
        if (noeud->pjeton_suiv != NULL) {
            afficher_rec_couleur(noeud->pjeton_suiv, nouveau_prefixe, 1);
        }
    }
    
    // Commencer l'affichage récursif avec couleurs
    char prefixe_initial[1] = "";
    printf("Arbre :\n");
    afficher_rec_couleur(racine, prefixe_initial, 1);
}