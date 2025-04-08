#include "affiche_syntaxe.h"

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
                printf("%-25s |\n", operateur_string[liste[i].valeur.operateur]);
                break;
            case FONCTION:
                printf("%-25s |\n", fonction_string[liste[i].valeur.fonction]);
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
}

/**
 * Affiche un arbre de façon récursive
 * @param racine Nœud racine à afficher
 * @param prefixe Préfixe pour l'indentation
 * @param est_dernier Indique si le nœud est le dernier de sa fratrie
 */
void afficher_arbre_rec(Node* racine, char* prefixe, int est_dernier) {
    if (racine == NULL || (&(racine->jeton.lexem) == NULL)) {
        return;}
    
    char nouveau_prefixe[MAX_DEPTH];
    char valeur_jeton[50];
    
    // Afficher le préfixe actuel
    printf("%s", prefixe);
    
    // Afficher le connecteur approprié selon si c'est le dernier élément ou non
    printf("%s", est_dernier ? "\\__ " : "\\__ ");
    
    // Convertir le jeton en chaîne et l'afficher

    jeton_to_string(racine->jeton, valeur_jeton, sizeof(valeur_jeton));
     if (syntaxeVerbose >= 10) printf("racine->jeton.lexem=%d\n", racine->jeton.lexem);
    //printf("%s [%s]\n", lexem_string[racine->jeton.lexem], valeur_jeton);
    
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
        if (syntaxeVerbose >= 10) printf("un seul des pointeurs est NULL\n");
        if (syntaxeVerbose >= 10) printf("arbre1 == NULL = %d\n", arbre1 == NULL);
        if (syntaxeVerbose >= 10) printf("arbre2 == NULL = %d\n", arbre2 == NULL);
        return false;
    }

    // Comparaison des jetons
    if (arbre1->jeton.lexem != arbre2->jeton.lexem) {
        if (syntaxeVerbose >= 10) printf("arbre1->jeton.lexem != arbre2->jeton.lexem\n");
        return false;
    }

    // Comparaison des valeurs selon le type de lexem
    switch (arbre1->jeton.lexem) {
        case REEL:
            if (syntaxeVerbose >= 10) printf("REEL\n");
            if (arbre1->jeton.valeur.reel - arbre2->jeton.valeur.reel > 0.0001
             || arbre2->jeton.valeur.reel - arbre1->jeton.valeur.reel > 0.0001){
                return false;
            }
            break;
        case OPERATEUR:
            if (syntaxeVerbose >= 10) printf("OPERATEUR\n");
            if (arbre1->jeton.valeur.operateur !=arbre2->jeton.valeur.operateur) {
                return false;
            }
            break;
        case FONCTION:
            if (syntaxeVerbose >= 10) printf("FONCTION\n");
            if (arbre1->jeton.valeur.fonction !=arbre2->jeton.valeur.fonction) {
                return false;
            }
            break;
        case VARIABLE:
            if (syntaxeVerbose >= 10) printf("VARIABLE\n");
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