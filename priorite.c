#include "priorite.h"

Node* arbrevide(){
    Node* arbre = (Node*)malloc(sizeof(Node));
    *arbre = (Node){
        .pjeton_preced = NULL,
        .pjeton_suiv = NULL,
        .jeton = FIN
    };
    return arbre;
}

Node* operateur( typejeton *tab, int debut, int fin, typeerreur *erreur){

    printf("\n-------DEBUT OPERATEUR --------\n");
    if(tab[fin].lexem==FIN) {
        printf("FIN------------\n");
        fin--;
    } else {
        printf("fin=%s\n", lexem_string[tab[fin].lexem]);
    }
    printf("debut =%d fin %d\n", debut,fin);
    afficher_liste_jetons(tab, debut, fin);
    int  indoputile = minIndice(tab, debut, fin, erreur);
    printf("voir indoputile %d \n", indoputile);
    if (*erreur != 0){
        printf("dans op erreur != 0 = %d\n", *erreur);
        return arbrevide();
    }
    if (debut>fin){
        printf("debut>fin\n");
        *erreur = MEMBRE_VIDE;
        return arbrevide();
    }
    if (indoputile != -1){
        printf("indoputile = %d\n", indoputile);
        Node* Fg = (Node*)malloc(sizeof(Node));
        Fg= operateur(tab, debut, indoputile - 1, erreur);
        Node* Fd = (Node*)malloc(sizeof(Node));
        Fd = operateur(tab,indoputile + 1, fin, erreur);
        Node* arbre = (Node*)malloc(sizeof(Node));
        *arbre = (Node){
            .pjeton_preced = Fg,
            .pjeton_suiv = Fd,
            .jeton = tab[indoputile]
        };
        printf("aff arbre operateur\n");
        afficher_arbre(arbre);
        printf("fin aff arbre operateur\n");
        return arbre;
    }
    else{
        switch (tab[debut].lexem){
            case FONCTION:
                printf("coucou je suis une putain de fonction\n");
                if (tab[debut + 1].lexem != PAR_OUV){
                        *erreur = PROBLEME_PARENTHESES_FONCTIONS;
                        return arbrevide();
                }
                if ((tab[debut+2].lexem!=FONCTION) && (tab[debut+2].lexem!=REEL) && (tab[debut+2].lexem !=VARIABLE)){
                    printf("je suis une fonction qui n'aime pas ce que tu me fait bouffer !!! debut+2=%d \n", debut+2);
                    *erreur = MEMBRE_VIDE;
                    return arbrevide();
                }

                Node* Fg = (Node*)malloc(sizeof(Node));
                Fg = operateur(tab, debut + 2, fin - 1, erreur);
                Node* arbre = (Node*)malloc(sizeof(Node));
                *arbre = (Node){
                    .pjeton_preced = Fg,
                    .pjeton_suiv = NULL,
                    .jeton = tab[debut]   
                }; 
                printf("aff arbre func\n");
                afficher_arbre(arbre);
                printf("fin aff arbre func\n");
                return arbre;
                break;
            case PAR_OUV:
                if (tab[fin].lexem != PAR_FERM){
                    printf("MEMBRE_VIDE tab[fin].lexem != PAR_FERM\n");
                    *erreur = MEMBRE_VIDE;
                    return arbrevide();
                }
                if ((tab[debut+2].lexem!=FONCTION) && (tab[debut+2].lexem!=REEL) && (tab[debut+2].lexem !=VARIABLE)){
                    printf("MEMBRE_VIDE tab[debut + 2].lexem !=(FONCTION||REEL||VARIABLE\n");
                    *erreur = MEMBRE_VIDE;
                    return arbrevide();
                }
                else{
                    Node* FG = (Node*)malloc(sizeof(Node));
                    FG = operateur(tab, debut + 1, fin - 1, erreur);
                    Node* arbre = (Node*)malloc(sizeof(Node));
                    *arbre = (Node){
                        .pjeton_preced = FG,
                        .pjeton_suiv = NULL,
                        .jeton = tab[indoputile]
                    };
                    return arbre;
                }
                break;
            case PAR_FERM:
                *erreur = PARENTHESE_FERMEE_1_ER_JETON;
                return arbrevide(); 
                break;
            case REEL:
                if (fin - debut == 0){
                    Node* arbre = (Node*)malloc(sizeof(Node));
                    *arbre = (Node){
                        .pjeton_preced = NULL,
                        .pjeton_suiv = NULL,
                        .jeton = tab[debut]
                    };
                    printf("case reel abre cree :)\n");
                    return arbre;
                }
                else{
                    *erreur = PROBLEME_APRES_REEL;
                    return arbrevide();
                }
                break;
            default:
                printf("default de ses grands morst\n");
                break;
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
    for (int i = debut; i < fin - debut + 1; i++){
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

Node* Syntaxique(typejeton *tab, typeerreur *erreur){
    int fin = calculTaille(tab);
    Node* arbrePlein = (Node*)malloc(sizeof(Node));
    arbrePlein = operateur( tab, 0, fin, erreur);
    return arbrePlein;
}

void afficher_liste_jetons(typejeton* liste, int debut, int fin) {
    if (liste == NULL) {
        printf("Liste de jetons vide\n");
        return;
    }

    int i = debut;
    printf("Liste des jetons :\n");
    printf("---------------------------------------------------\n");
    printf("| Indice |    Type    |           Valeur |\n");
    printf("---------------------------------------------------\n");

    while (i<=fin) {
    //while ((liste[i].lexem != FIN || i == 0) && i<=fin) {
        printf("| %6d | ", i);

        // Affichage du type de lexem
        printf("%-10s | ", lexem_string[liste[i].lexem]);

        // Affichage de la valeur selon le type de lexem
        switch (liste[i].lexem) {
            case REEL:
                printf("%-25.4f |\n", liste[i].valeur.reel);
                break;
            case OPERATEUR:
                printf("%-25s |\n",
                operateur_string[liste[i].valeur.operateur]);
                break;
            case FONCTION:
                printf("%-25s |\n",
                fonction_string[liste[i].valeur.fonction]);
                break;
            case VARIABLE:
                printf("%-25c |\n", liste[i].valeur.variable);
                break;
            case PAR_OUV:
                printf("%-25s |\n", "(");
                break;
            case PAR_FERM:
                printf("%-25s |\n", ")");
                break;
            case BAR_OUV:
                printf("%-25s |\n", "|");
                break;
            case BAR_FERM:
                printf("%-25s |\n", "|");
                break;
            case ABSOLU:
                printf("%-25s |\n", "abs");
                break;
            case FIN:
                printf("%-25s |\n", "FIN");
                break;
            case ERREUR:
                printf("%-25s |\n", "ERREUR");
                break;
            default:
                printf("%-25s |\n", "Type inconnu");
        }

        i++;

        // Vérification pour éviter la boucle infinie
        if (i > 100) {  // Limite arbitraire pour éviter les boucles infinies
            printf("...\n");
            printf("Attention: Plus de 100 jetons, affichage tronqué\n");
            break;
        }
    }

    // Afficher le jeton FIN si présent et pas déjà affiché
    if (liste[i].lexem == FIN && i > 0) {
        printf("| %6d | %-10s | %-25s |\n", i, lexem_string[FIN], "FIN");
    }

printf("---------------------------------------------------\n");
}


/**
 * Convertit un jeton en chaîne de caractères lisible
 * @param jeton Le jeton à convertir
 * @param buffer Le buffer où stocker la chaîne
 * @param size La taille du buffer
 */
void jeton_to_string(typejeton jeton, char* buffer, size_t size) {
    printf("debut jeton to string\n");
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
            break;
    }
    printf("buffer=%s", buffer);
    printf("fin jeton to string\n");
}

/**
 * Affiche un arbre de façon récursive
 * @param racine Nœud racine à afficher
 * @param prefixe Préfixe pour l'indentation
 * @param est_dernier Indique si le nœud est le dernier de sa fratrie
 */
void afficher_arbre_rec(Node* racine, char* prefixe, int est_dernier) {
    if (racine == NULL || (&(racine->jeton.lexem) == NULL)) {
        printf("aff rec null\n");
        return;}
    
    char nouveau_prefixe[MAX_DEPTH];
    char valeur_jeton[50];
    
    // Afficher le préfixe actuel
    printf("%s", prefixe);
    
    // Afficher le connecteur approprié selon si c'est le dernier élément ou non
    printf("%s", est_dernier ? "\\__ " : "\\-- ");
    
    // Convertir le jeton en chaîne et l'afficher
    printf("aff rec avant jeton to string\n");
    jeton_to_string(racine->jeton, valeur_jeton, sizeof(valeur_jeton));
    printf("racine->jeton.lexem=%d\n", racine->jeton.lexem);
    //printf("%s [%s]\n", lexem_string[racine->jeton.lexem], valeur_jeton);
    
    // Créer un nouveau préfixe pour les enfants
    snprintf(nouveau_prefixe, MAX_DEPTH, "%s%s", prefixe, est_dernier ? "    " : "│   ");
    
    printf("aff rec nv pref cree\n");
    
    // Traiter d'abord le nœud précédent (considéré comme le fils gauche)
    if (racine->pjeton_preced != NULL) {
        printf("aff rec fg pas null\n");
        afficher_arbre_rec(racine->pjeton_preced, nouveau_prefixe, racine->pjeton_suiv == NULL);
    }else {
        printf("aff rec fg null\n");
    }
    
    // Puis traiter le nœud suivant (considéré comme le fils droit)
    if (racine->pjeton_suiv != NULL) {
        printf("aff rec fd pas null\n");
        afficher_arbre_rec(racine->pjeton_suiv, nouveau_prefixe, 1);
    }else {
        printf("aff rec fd null\n");
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

void afficher_arbre2(Node* racine, int niveau) {
    // Vérifier si l'arbre est vide
    if (racine == NULL) {
        printf("Arbre vide!\n");
        return;
    }

    // Indentation pour montrer la hiérarchie
    for (int i = 0; i < niveau; i++) {
        printf("  ");
    }

    // Afficher le type de jeton
    printf("|-- ");

    // Afficher le contenu du nœud selon son type
    switch (racine->jeton.lexem) {
        case REEL:
            printf("REEL: %.4f\n", racine->jeton.valeur.reel);
            break;
        case OPERATEUR:
            printf("OPERATEUR: %s\n",
            operateur_string[racine->jeton.valeur.operateur]);
            break;
        case FONCTION:
            printf("FONCTION: %s\n",
            fonction_string[racine->jeton.valeur.fonction]);
            break;
        case VARIABLE:
            printf("VARIABLE: %c\n", racine->jeton.valeur.variable);
            break;
        case PAR_OUV:
            printf("PAR_OUV\n");
            break;
        case PAR_FERM:
            printf("PAR_FERM\n");
            break;
        case BAR_OUV:
            printf("BAR_OUV\n");
            break;
        case BAR_FERM:
            printf("BAR_FERM\n");
            break;
        case ABSOLU:
            printf("ABSOLU\n");
            break;
        case FIN:
            printf("FIN\n");
            break;
        case ERREUR:
            printf("ERREUR\n");
            break;
        default:
            printf("TYPE INCONNU\n");
    }

    // Afficher les fils gauche et droit s'ils existent
    if (racine->pjeton_preced != NULL) {
        for (int i = 0; i < niveau; i++) {
            printf("  ");
        }
        printf("|  (gauche)\n");
        afficher_arbre2(racine->pjeton_preced, niveau + 1);
    }

    if (racine->pjeton_suiv != NULL) {
        for (int i = 0; i < niveau; i++) {
            printf("  ");
        }
        printf("|  (droit)\n");
        afficher_arbre2(racine->pjeton_suiv, niveau + 1);
    }
}

// Fonction wrapper pour simplifier l'appel
void afficher_arbre_complet(Node* racine) {
    printf("\n=== AFFICHAGE DE L'ARBRE ===\n");
    afficher_arbre2(racine, 0);
    printf("===========================\n\n");
}


/**
 * Version alternative avec couleurs ANSI pour un affichage plus joli
 * @param racine Nœud racine de l'arbre à afficher
 */

// Définir les codes de couleur ANSI
const char* RESET = "\033[0m";
const char* VERT = "\033[32m";
const char* BLEU = "\033[34m";
const char* ROUGE = "\033[31m";
const char* JAUNE = "\033[33m";
const char* CYAN = "\033[36m";

// Fonction locale pour choisir la couleur selon le type de lexem
const char* choisir_couleur(typelexem lexem){
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
    printf("%s", est_dernier ? "\\__ " : "\\-- ");
    
    // Convertir le jeton en chaîne et l'afficher avec couleur
    jeton_to_string(noeud->jeton, valeur_jeton, sizeof(valeur_jeton));
    const char* couleur = choisir_couleur(noeud->jeton.lexem);
    printf("%s%s%s [%s%s%s]\n", couleur, lexem_string[noeud->jeton.lexem], RESET, couleur, valeur_jeton, RESET);
    
    // Créer un nouveau préfixe pour les enfants
    snprintf(nouveau_prefixe, MAX_DEPTH, "%s%s", prefixe, est_dernier ? "    " : "|   ");
    
    // Traiter d'abord le nœud précédent
    if (noeud->pjeton_preced != NULL) {
        afficher_rec_couleur(noeud->pjeton_preced, nouveau_prefixe, noeud->pjeton_suiv == NULL);
    }
    
    // Puis traiter le nœud suivant
    if (noeud->pjeton_suiv != NULL) {
        afficher_rec_couleur(noeud->pjeton_suiv, nouveau_prefixe, 1);
    }
}


void afficher_arbre_couleur(Node* racine) {
    if (racine == NULL) {
        printf("Arbre vide\n");
        return;
    }
    // Commencer l'affichage récursif avec couleurs
    char prefixe_initial[1] = "";
    printf("Arbre :\n");
    afficher_rec_couleur(racine, prefixe_initial, 1);
}

bool comparer_arbres(Node* arbre1, Node* arbre2) {
    // Si les deux pointeurs sont NULL, les arbres sont identiques
    if (arbre1 == NULL && arbre2 == NULL) {
        return true;
    }

    // Si un seul des pointeurs est NULL, les arbres sont différents
    if (arbre1 == NULL || arbre2 == NULL) {
        return false;
    }

    // Comparaison des jetons
    if (arbre1->jeton.lexem != arbre2->jeton.lexem) {
        return false;
    }

    // Comparaison des valeurs selon le type de lexem
    switch (arbre1->jeton.lexem) {
        case REEL:
            if (arbre1->jeton.valeur.reel -arbre2->jeton.valeur.reel > 0.0001
            || arbre2->jeton.valeur.reel -arbre1->jeton.valeur.reel > 0.0001){
                return false;
            }
            break;
        case OPERATEUR:
            if (arbre1->jeton.valeur.operateur !=arbre2->jeton.valeur.operateur) {
                return false;
            }
            break;
        case FONCTION:
            if (arbre1->jeton.valeur.fonction !=arbre2->jeton.valeur.fonction) {
                return false;
            }
            break;
        case VARIABLE:
            if (arbre1->jeton.valeur.variable !=arbre2->jeton.valeur.variable) {
                return false;
            }
            break;
        default:
            break;
    }

    // Comparaison récursive des sous-arbres
    return comparer_arbres(arbre1->pjeton_preced,arbre2->pjeton_preced) && comparer_arbres(arbre1->pjeton_suiv, arbre2->pjeton_suiv);
}

// Fonction pour libérer la mémoire d'un arbre
void liberer_arbre(Node* racine) {
    if (racine == NULL) {
        return;
    }

    liberer_arbre(racine->pjeton_preced);
    liberer_arbre(racine->pjeton_suiv);
    free(racine);
}
