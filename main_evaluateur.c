#include "evaluateur.h"
#include <stdbool.h>
#include <stdlib.h>
#define TAILLE_MAX 100
#define MAX_DEPTH TAILLE_MAX
 
//creation  d'un EXEMPLE :
Node creation_arbre(typejeton racine, Node* precedent, Node* suivant){
    Node arbre;
    arbre.jeton=racine;
    arbre.pjeton_preced=precedent;
    arbre.pjeton_suiv=suivant;
    return arbre;
}
int ERREUR_CODE=0;
static const int syntaxeVerbose=10;

static const char* lexem_string[] = {
    [REEL] = "REEL",
    [OPERATEUR] = "OPERATEUR",
    [FONCTION] = "FONCTION",
    [ERREUR] = "ERREUR",
    [FIN] = "FIN",
    [PAR_OUV] = "PAR_OUV",
    [PAR_FERM] = "PAR_FERM",
    [VARIABLE] = "VARIABLE",
    [BAR_OUV] = "BAR_OUV",
    [BAR_FERM] = "BAR_FERM",
    [ABSOLU] = "ABSOLU"
};

static const char* operateur_string[] = {
    [PLUS] = "PLUS",
    [MOINS] = "MOINS",
    [FOIS] = "FOIS",
    [DIV] = "DIV",
    [PUIS] = "PUIS"
};

static const char* fonction_string[] = {
    [ABS] = "ABS",
    [SIN] = "SIN",
    [SQRT] = "SQRT",
    [LOG] = "LOG",
    [COS] = "COS",
    [TAN] = "TAN",
    [EXP] = "EXP",
    [ENTIER] = "ENTIER",
    [VAL_NEG] = "VAL_NEG",
    [SINC] = "SINC"
};



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

// Fonction pour libérer la mémoire d'un arbre
void liberer_arbre(Node* racine) {
    if (racine == NULL) {
        return;
    }

    liberer_arbre(racine->pjeton_preced);
    liberer_arbre(racine->pjeton_suiv);
    free(racine);
}
/** 
int main(){


    

        int ERREUR_CODE=0;


    // Déclaration des variables x et y
    typejeton x;
    x.lexem = VARIABLE;
    x.valeur.variable = 'x';
    
    typejeton y;
    y.lexem = VARIABLE;
    y.valeur.variable = 'y';
    
    // Constantes numériques
    typejeton deux;
    deux.lexem = REEL;
    deux.valeur.reel = 2.0;
    
    // Opérateurs
    typejeton plus;
    plus.lexem = OPERATEUR;
    plus.valeur.operateur = PLUS;
    
    typejeton moins;
    moins.lexem = OPERATEUR;
    moins.valeur.operateur = MOINS;
    
    typejeton mult;
    mult.lexem = OPERATEUR;
    mult.valeur.operateur = FOIS;
    
    typejeton div;
    div.lexem = OPERATEUR;
    div.valeur.operateur = DIV;
    
    typejeton puissance;
    puissance.lexem = OPERATEUR;
    puissance.valeur.operateur = PUIS;
    
    // Fonctions
    typejeton racine_carree;
    racine_carree.lexem = FONCTION;
    racine_carree.valeur.fonction = SQRT;
    
    typejeton log_nat;
    log_nat.lexem = FONCTION;
    log_nat.valeur.fonction = LOG;
    
    typejeton sinus;
    sinus.lexem = FONCTION;
    sinus.valeur.fonction = SIN;
    
    typejeton cosinus;
    cosinus.lexem = FONCTION;
    cosinus.valeur.fonction = COS;
    
    typejeton exponentielle;
    exponentielle.lexem = FONCTION;
    exponentielle.valeur.fonction = EXP;
    
    typejeton valeur_absolue;
    valeur_absolue.lexem = FONCTION;
    valeur_absolue.valeur.fonction = ABS;
    
    // Construction de l'arbre pour l'expression log(sqrt(x^2 + y^2)) + sin(x*cos(y)) / exp(abs(x-y))
    
    // Partie 1: log(sqrt(x^2 + y^2))
    // Sous-expression: x^2
    Node X_ARBRE = creation_arbre(x, NULL, NULL);
    Node DEUX_ARBRE = creation_arbre(deux, NULL, NULL);
    Node X_CARRE_ARBRE = creation_arbre(puissance, &X_ARBRE, &DEUX_ARBRE);
    
    // Sous-expression: y^2
    Node Y_ARBRE = creation_arbre(y, NULL, NULL);
    Node Y_CARRE_ARBRE = creation_arbre(puissance, &Y_ARBRE, &DEUX_ARBRE);
    
    // Sous-expression: x^2 + y^2
    Node SOMME_CARRES_ARBRE = creation_arbre(plus, &X_CARRE_ARBRE, &Y_CARRE_ARBRE);
    
    // Sous-expression: sqrt(x^2 + y^2)
    Node RACINE_SOMME_ARBRE = creation_arbre(racine_carree, &SOMME_CARRES_ARBRE, NULL);
    
    // Expression: log(sqrt(x^2 + y^2))
    Node LOG_RACINE_ARBRE = creation_arbre(log_nat, &RACINE_SOMME_ARBRE, NULL);
    
    // Partie 2: sin(x*cos(y))
    // Sous-expression: cos(y)
    Node COS_Y_ARBRE = creation_arbre(cosinus, &Y_ARBRE, NULL);
    
    // Sous-expression: x*cos(y)
    Node X_COS_Y_ARBRE = creation_arbre(mult, &X_ARBRE, &COS_Y_ARBRE);
    
    // Expression: sin(x*cos(y))
    Node SIN_X_COS_Y_ARBRE = creation_arbre(sinus, &X_COS_Y_ARBRE, NULL);
    
    // Partie 3: exp(abs(x-y))
    // Sous-expression: x-y
    Node X_MOINS_Y_ARBRE = creation_arbre(moins, &X_ARBRE, &Y_ARBRE);
    
    // Sous-expression: abs(x-y)
    Node ABS_X_MOINS_Y_ARBRE = creation_arbre(valeur_absolue, &X_MOINS_Y_ARBRE, NULL);
    
    // Expression: exp(abs(x-y))
    Node EXP_ABS_ARBRE = creation_arbre(exponentielle, &ABS_X_MOINS_Y_ARBRE, NULL);
    
    // Partie 4: sin(x*cos(y)) / exp(abs(x-y))
    Node QUOTIENT_ARBRE = creation_arbre(div, &SIN_X_COS_Y_ARBRE, &EXP_ABS_ARBRE);
    
    // Expression finale: log(sqrt(x^2 + y^2)) + sin(x*cos(y)) / exp(abs(x-y))
    Node EXPRESSION_FINALE_ARBRE = creation_arbre(plus, &LOG_RACINE_ARBRE, &QUOTIENT_ARBRE);
    
    // Évaluation de l'expression pour les valeurs x=2 et y=1

    printf("\nÉvaluation de log(sqrt(x^2 + y^2)) + sin(x*cos(y)) / exp(abs(x-y)) pour x=2 et y=1 :\n");
    printf("Résultat: %f\n", evaluateur(&EXPRESSION_FINALE_ARBRE, 2.0, 1.0, &ERREUR_CODE));
    printf("Code d'erreur: %d\n", ERREUR_CODE);
    afficher_arbre_couleur(&EXPRESSION_FINALE_ARBRE);
    
    // Créons aussi une autre expression complexe pour tester les cas limites
    // tan(sqrt(x)/log(y)) + sinc(x-y) * exp(cos(x*y))
    
    // Partie 1: tan(sqrt(x)/log(y))
    typejeton tangente;
    tangente.lexem = FONCTION;
    tangente.valeur.fonction = TAN;
    
    // Sous-expression: sqrt(x)
    Node RACINE_X_ARBRE = creation_arbre(racine_carree, &X_ARBRE, NULL);
    
    // Sous-expression: log(y)
    Node LOG_Y_ARBRE = creation_arbre(log_nat, &Y_ARBRE, NULL);
    
    // Sous-expression: sqrt(x)/log(y)
    Node RACINE_X_DIV_LOG_Y_ARBRE = creation_arbre(div, &RACINE_X_ARBRE, &LOG_Y_ARBRE);
    
    // Expression: tan(sqrt(x)/log(y))
    Node TAN_ARBRE = creation_arbre(tangente, &RACINE_X_DIV_LOG_Y_ARBRE, NULL);
    
    // Partie 2: sinc(x-y)
    typejeton sinc;
    sinc.lexem = FONCTION;
    sinc.valeur.fonction = SINC;
    
    // Sous-expression: sinc(x-y)
    Node SINC_X_MOINS_Y_ARBRE = creation_arbre(sinc, &X_MOINS_Y_ARBRE, NULL);
    
    // Partie 3: exp(cos(x*y))
    // Sous-expression: x*y
    Node X_MULT_Y_ARBRE = creation_arbre(mult, &X_ARBRE, &Y_ARBRE);
    
    // Sous-expression: cos(x*y)
    Node COS_X_MULT_Y_ARBRE = creation_arbre(cosinus, &X_MULT_Y_ARBRE, NULL);
    
    // Expression: exp(cos(x*y))
    Node EXP_COS_ARBRE = creation_arbre(exponentielle, &COS_X_MULT_Y_ARBRE, NULL);
    
    // Partie 4: sinc(x-y) * exp(cos(x*y))
    Node SINC_MULT_EXP_ARBRE = creation_arbre(mult, &SINC_X_MOINS_Y_ARBRE, &EXP_COS_ARBRE);
    
    // Expression finale: tan(sqrt(x)/log(y)) + sinc(x-y) * exp(cos(x*y))
    Node EXPRESSION_FINALE_2_ARBRE = creation_arbre(plus, &TAN_ARBRE, &SINC_MULT_EXP_ARBRE);
    
    // Évaluation de la deuxième expression pour les valeurs x=3 et y=2
    printf("\nÉvaluation de tan(sqrt(x)/log(y)) + sinc(x-y) * exp(cos(x*y)) pour x=3 et y=2 :\n");
    printf("Résultat: %f\n", evaluateur(&EXPRESSION_FINALE_2_ARBRE, 3.0, 2.0, &ERREUR_CODE));
    printf("Code d'erreur: %d\n", ERREUR_CODE);
    afficher_arbre_couleur(&EXPRESSION_FINALE_2_ARBRE);
    // Test avec des valeurs qui pourraient causer des erreurs (y=0 pour log(y))
    printf("\nTest avec des valeurs problématiques - x=2 et y=0 :\n");
    ERREUR_CODE = 0; // Réinitialisation du code d'erreur
    printf("Résultat: %f\n", evaluateur(&EXPRESSION_FINALE_2_ARBRE, 2.0, 0.0, &ERREUR_CODE));
    printf("Code d'erreur: %d\n", ERREUR_CODE);
    afficher_arbre_couleur(&EXPRESSION_FINALE_2_ARBRE);
    return 0;
}
// FIN EXEMPLE


**/

// Test d'expressions mathématiques valides
void tester_expressions_valides() {
    int ERREUR_CODE = 0;
    
    // Déclaration des jetons
    typejeton x, y, deux, pi, plus, moins, mult, div, puissance;
    typejeton racine_carree, log_nat, sinus, cosinus, exponentielle, valeur_absolue;
    
    // Variables
    x.lexem = VARIABLE;
    x.valeur.variable = 'x';
    
    y.lexem = VARIABLE;
    y.valeur.variable = 'y';
    
    // Constantes
    deux.lexem = REEL;
    deux.valeur.reel = 2.0;
    
    pi.lexem = REEL;
    pi.valeur.reel = 3.14159;
    
    // Opérateurs
    plus.lexem = OPERATEUR;
    plus.valeur.operateur = PLUS;
    
    moins.lexem = OPERATEUR;
    moins.valeur.operateur = MOINS;
    
    mult.lexem = OPERATEUR;
    mult.valeur.operateur = FOIS;
    
    div.lexem = OPERATEUR;
    div.valeur.operateur = DIV;
    
    puissance.lexem = OPERATEUR;
    puissance.valeur.operateur = PUIS;
    
    // Fonctions
    racine_carree.lexem = FONCTION;
    racine_carree.valeur.fonction = SQRT;
    
    log_nat.lexem = FONCTION;
    log_nat.valeur.fonction = LOG;
    
    sinus.lexem = FONCTION;
    sinus.valeur.fonction = SIN;
    
    cosinus.lexem = FONCTION;
    cosinus.valeur.fonction = COS;
    
    exponentielle.lexem = FONCTION;
    exponentielle.valeur.fonction = EXP;
    
    valeur_absolue.lexem = FONCTION;
    valeur_absolue.valeur.fonction = ABS;
    
    // Exemple 1: x + y
    printf("\n=== Test expression simple: x + y ===\n");
    Node X_ARBRE = creation_arbre(x, NULL, NULL);
    Node Y_ARBRE = creation_arbre(y, NULL, NULL);
    Node SOMME_XY = creation_arbre(plus, &X_ARBRE, &Y_ARBRE);
    
    printf("Évaluation pour x=3.5 et y=2.5:\n");
    float resultat = evaluateur(&SOMME_XY, 3.5, 2.5, &ERREUR_CODE);
    printf("Résultat: %f (attendu: 6.0)\n", resultat);
    printf("Code d'erreur: %d (attendu: 0)\n", ERREUR_CODE);
    afficher_arbre_couleur(&SOMME_XY);
    
    // Exemple 2: sin(x) * cos(y)
    printf("\n=== Test fonctions trigonométriques: sin(x) * cos(y) ===\n");
    Node SIN_X = creation_arbre(sinus, &X_ARBRE, NULL);
    Node COS_Y = creation_arbre(cosinus, &Y_ARBRE, NULL);
    Node PROD_TRIG = creation_arbre(mult, &SIN_X, &COS_Y);
    
    ERREUR_CODE = 0;
    printf("Évaluation pour x=0 et y=0:\n");
    resultat = evaluateur(&PROD_TRIG, 0.0, 0.0, &ERREUR_CODE);
    printf("Résultat: %f (attendu: 0.0)\n", resultat);
    printf("Code d'erreur: %d (attendu: 0)\n", ERREUR_CODE);
    afficher_arbre_couleur(&PROD_TRIG);
    
    // Exemple 3: exp(x) / (1 + y^2)
    printf("\n=== Test expression complexe: exp(x) / (1 + y^2) ===\n");
    Node UN = creation_arbre(deux, NULL, NULL);
    UN.jeton.valeur.reel = 1.0;
    
    Node EXP_X = creation_arbre(exponentielle, &X_ARBRE, NULL);
    Node Y_CARRE = creation_arbre(puissance, &Y_ARBRE, &UN);
    Node UN_PLUS_Y2 = creation_arbre(plus, &UN, &Y_CARRE);
    Node DIVISION_EXPR = creation_arbre(div, &EXP_X, &UN_PLUS_Y2);
    
    ERREUR_CODE = 0;
    printf("Évaluation pour x=1 et y=2:\n");
    resultat = evaluateur(&DIVISION_EXPR, 1.0, 2.0, &ERREUR_CODE);
    printf("Résultat: %f (attendu: ≈0.5)\n", resultat);
    printf("Code d'erreur: %d (attendu: 0)\n", ERREUR_CODE);
    afficher_arbre_couleur(&DIVISION_EXPR);
}

// Test des erreurs possibles avec la fonction SQRT
void tester_erreur_racine_negative() {
    int ERREUR_CODE = 0;
    
    // Création des jetons
    typejeton x, moins, neg_un;
    
    x.lexem = VARIABLE;
    x.valeur.variable = 'x';
    
    moins.lexem = OPERATEUR;
    moins.valeur.operateur = MOINS;
    
    neg_un.lexem = REEL;
    neg_un.valeur.reel = -1.0;
    
    typejeton racine_carree;
    racine_carree.lexem = FONCTION;
    racine_carree.valeur.fonction = SQRT;
    
    // Expression: sqrt(-1)
    printf("\n=== Test erreur RACINE_NEGATIVE: sqrt(-1) ===\n");
    Node NEG_UN_ARBRE = creation_arbre(neg_un, NULL, NULL);
    Node RACINE_NEG = creation_arbre(racine_carree, &NEG_UN_ARBRE, NULL);
    
    float resultat = evaluateur(&RACINE_NEG, 0.0, 0.0, &ERREUR_CODE);
    printf("Résultat: %f\n", resultat);
    printf("Code d'erreur: %d (attendu: %d)\n", ERREUR_CODE, RACINE_NEGATIVE);
    afficher_arbre_couleur(&RACINE_NEG);
    
    // Expression: sqrt(x) avec x=-2
    printf("\n=== Test erreur RACINE_NEGATIVE: sqrt(x) avec x=-2 ===\n");
    Node X_ARBRE = creation_arbre(x, NULL, NULL);
    Node RACINE_X = creation_arbre(racine_carree, &X_ARBRE, NULL);
    
    ERREUR_CODE = 0;
    resultat = evaluateur(&RACINE_X, -2.0, 0.0, &ERREUR_CODE);
    printf("Résultat: %f\n", resultat);
    printf("Code d'erreur: %d (attendu: %d)\n", ERREUR_CODE, RACINE_NEGATIVE);
    afficher_arbre_couleur(&RACINE_X);
}

// Test des erreurs possibles avec la fonction LOG
void tester_erreurs_logarithme() {
    int ERREUR_CODE = 0;
    
    // Création des jetons
    typejeton x, y, neg_un, zero;
    
    x.lexem = VARIABLE;
    x.valeur.variable = 'x';
    
    y.lexem = VARIABLE;
    y.valeur.variable = 'y';
    
    neg_un.lexem = REEL;
    neg_un.valeur.reel = -1.0;
    
    zero.lexem = REEL;
    zero.valeur.reel = 0.0;
    
    typejeton log_nat;
    log_nat.lexem = FONCTION;
    log_nat.valeur.fonction = LOG;
    
    // Expression: log(-1)
    printf("\n=== Test erreur LOG_NEGATIF: log(-1) ===\n");
    Node NEG_UN_ARBRE = creation_arbre(neg_un, NULL, NULL);
    Node LOG_NEG = creation_arbre(log_nat, &NEG_UN_ARBRE, NULL);
    
    float resultat = evaluateur(&LOG_NEG, 0.0, 0.0, &ERREUR_CODE);
    printf("Résultat: %f\n", resultat);
    printf("Code d'erreur: %d (attendu: %d)\n", ERREUR_CODE, LOG_NEGATIF);
    afficher_arbre_couleur(&LOG_NEG);
    
    // Expression: log(0)
    printf("\n=== Test erreur LOG_ZERO: log(0) ===\n");
    Node ZERO_ARBRE = creation_arbre(zero, NULL, NULL);
    Node LOG_ZERO_ARBRE = creation_arbre(log_nat, &ZERO_ARBRE, NULL);
    
    ERREUR_CODE = 0;
    resultat = evaluateur(&LOG_ZERO_ARBRE, 0.0, 0.0, &ERREUR_CODE);
    printf("Résultat: %f\n", resultat);
    printf("Code d'erreur: %d (attendu: %d)\n", ERREUR_CODE, LOG_ZERO);
    afficher_arbre_couleur(&LOG_ZERO_ARBRE);
    
    // Expression: log(y) avec y=-3
    printf("\n=== Test erreur LOG_NEGATIF: log(y) avec y=-3 ===\n");
    Node Y_ARBRE = creation_arbre(y, NULL, NULL);
    Node LOG_Y = creation_arbre(log_nat, &Y_ARBRE, NULL);
    
    ERREUR_CODE = 0;
    resultat = evaluateur(&LOG_Y, 0.0, -3.0, &ERREUR_CODE);
    printf("Résultat: %f\n", resultat);
    printf("Code d'erreur: %d (attendu: %d)\n", ERREUR_CODE, LOG_NEGATIF);
    afficher_arbre_couleur(&LOG_Y);
}

// Test des erreurs possibles avec la fonction TAN
void tester_erreur_tangente() {
    int ERREUR_CODE = 0;
    
    // Création des jetons
    typejeton x, pi_sur_2;
    
    x.lexem = VARIABLE;
    x.valeur.variable = 'x';
    
    pi_sur_2.lexem = REEL;
    pi_sur_2.valeur.reel = 1.57079632679; // π/2
    
    typejeton tangente;
    tangente.lexem = FONCTION;
    tangente.valeur.fonction = TAN;
    
    // Expression: tan(π/2)
    printf("\n=== Test erreur TAN_PI_SUR_2: tan(π/2) ===\n");
    Node PI_SUR_2_ARBRE = creation_arbre(pi_sur_2, NULL, NULL);
    Node TAN_PI_SUR_2_ARBRE = creation_arbre(tangente, &PI_SUR_2_ARBRE, NULL);
    
    float resultat = evaluateur(&TAN_PI_SUR_2_ARBRE, 0.0, 0.0, &ERREUR_CODE);
    printf("Résultat: %f\n", resultat);
    printf("Code d'erreur: %d (attendu: %d)\n", ERREUR_CODE, TAN_PI_SUR_2);
    afficher_arbre_couleur(&TAN_PI_SUR_2_ARBRE);
    
    // Expression: tan(x) avec x=π/2
    printf("\n=== Test erreur TAN_PI_SUR_2: tan(x) avec x=π/2 ===\n");
    Node X_ARBRE = creation_arbre(x, NULL, NULL);
    Node TAN_X = creation_arbre(tangente, &X_ARBRE, NULL);
    
    ERREUR_CODE = 0;
    resultat = evaluateur(&TAN_X, 1.57079632679, 0.0, &ERREUR_CODE);
    printf("Résultat: %f\n", resultat);
    printf("Code d'erreur: %d (attendu: %d)\n", ERREUR_CODE, TAN_PI_SUR_2);
    afficher_arbre_couleur(&TAN_X);
}

// Test des erreurs possibles avec la fonction SINC
void tester_erreur_sinc() {
    int ERREUR_CODE = 0;
    
    // Création des jetons
    typejeton x, zero;
    
    x.lexem = VARIABLE;
    x.valeur.variable = 'x';
    
    zero.lexem = REEL;
    zero.valeur.reel = 0.0;
    
    typejeton sinc;
    sinc.lexem = FONCTION;
    sinc.valeur.fonction = SINC;
    
    // Note: Normalement SINC(0) est défini comme lim(sin(x)/x) quand x->0, qui vaut 1
    // Mais supposons que l'implémentation vérifie explicitement la division par zéro
    
    // Expression: sinc(0)
    printf("\n=== Test erreur potentielle SINC_DIVISIION_PAR_ZERO: sinc(0) ===\n");
    Node ZERO_ARBRE = creation_arbre(zero, NULL, NULL);
    Node SINC_ZERO = creation_arbre(sinc, &ZERO_ARBRE, NULL);
    
    float resultat = evaluateur(&SINC_ZERO, 0.0, 0.0, &ERREUR_CODE);
    if (ERREUR_CODE == SINC_DIVISIION_PAR_ZERO) {
        printf("Résultat: %f\n", resultat);
        printf("Code d'erreur: %d (attendu: %d)\n", ERREUR_CODE, SINC_DIVISIION_PAR_ZERO);
    } else {
        printf("Résultat: %f (Si sinc(0) est défini comme 1, c'est correct)\n", resultat);
        printf("Code d'erreur: %d (Si l'implémentation gère le cas limite, c'est normal)\n", ERREUR_CODE);
    }
    afficher_arbre_couleur(&SINC_ZERO);
    
    // Expression: sinc(x) avec x=0
    printf("\n=== Test avec sinc(x) où x=0 ===\n");
    Node X_ARBRE = creation_arbre(x, NULL, NULL);
    Node SINC_X = creation_arbre(sinc, &X_ARBRE, NULL);
    
    ERREUR_CODE = 0;
    resultat = evaluateur(&SINC_X, 0.0, 0.0, &ERREUR_CODE);
    if (ERREUR_CODE == SINC_DIVISIION_PAR_ZERO) {
        printf("Résultat: %f\n", resultat);
        printf("Code d'erreur: %d (attendu: %d)\n", ERREUR_CODE, SINC_DIVISIION_PAR_ZERO);
    } else {
        printf("Résultat: %f (Si sinc(0) est défini comme 1, c'est correct)\n", resultat);
        printf("Code d'erreur: %d (Si l'implémentation gère le cas limite, c'est normal)\n", ERREUR_CODE);
    }
    afficher_arbre_couleur(&SINC_X);
}

// Test de division par zéro
void tester_division_par_zero() {
    int ERREUR_CODE = 0;
    
    // Création des jetons
    typejeton x, y, un, zero, div_op;
    
    x.lexem = VARIABLE;
    x.valeur.variable = 'x';
    
    y.lexem = VARIABLE;
    y.valeur.variable = 'y';
    
    un.lexem = REEL;
    un.valeur.reel = 1.0;
    
    zero.lexem = REEL;
    zero.valeur.reel = 0.0;
    
    div_op.lexem = OPERATEUR;
    div_op.valeur.operateur = DIV;
    
    // Expression: 1/0
    printf("\n=== Test division par zéro: 1/0 ===\n");
    Node UN_ARBRE = creation_arbre(un, NULL, NULL);
    Node ZERO_ARBRE = creation_arbre(zero, NULL, NULL);
    Node DIV_PAR_ZERO = creation_arbre(div_op, &UN_ARBRE, &ZERO_ARBRE);
    
    float resultat = evaluateur(&DIV_PAR_ZERO, 0.0, 0.0, &ERREUR_CODE);
    printf("Résultat: %f\n", resultat);
    printf("Code d'erreur: %d (attendu: 311)\n", ERREUR_CODE);
    afficher_arbre_couleur(&DIV_PAR_ZERO);
    
    // Expression: x/y avec y=0
    printf("\n=== Test division par zéro: x/y avec y=0 ===\n");
    Node X_ARBRE = creation_arbre(x, NULL, NULL);
    Node Y_ARBRE = creation_arbre(y, NULL, NULL);
    Node X_DIV_Y = creation_arbre(div_op, &X_ARBRE, &Y_ARBRE);
    
    ERREUR_CODE = 0;
    resultat = evaluateur(&X_DIV_Y, 5.0, 0.0, &ERREUR_CODE);
    printf("Résultat: %f\n", resultat);
    printf("Code d'erreur: %d (attendu: 311)\n", ERREUR_CODE);
    afficher_arbre_couleur(&X_DIV_Y);
}

// Test d'expressions complexes avec imbrications de fonctions
void tester_expressions_imbriquees() {
    int ERREUR_CODE = 0;
    
    // Création des jetons
    typejeton x, y, deux, plus, moins, mult, div, puissance;
    typejeton racine_carree, log_nat, sinus, cosinus, exponentielle, valeur_absolue, tangente;
    
    x.lexem = VARIABLE;
    x.valeur.variable = 'x';
    
    y.lexem = VARIABLE;
    y.valeur.variable = 'y';
    
    deux.lexem = REEL;
    deux.valeur.reel = 2.0;
    
    plus.lexem = OPERATEUR;
    plus.valeur.operateur = PLUS;
    
    moins.lexem = OPERATEUR;
    moins.valeur.operateur = MOINS;
    
    mult.lexem = OPERATEUR;
    mult.valeur.operateur = FOIS;
    
    div.lexem = OPERATEUR;
    div.valeur.operateur = DIV;
    
    puissance.lexem = OPERATEUR;
    puissance.valeur.operateur = PUIS;
    
    racine_carree.lexem = FONCTION;
    racine_carree.valeur.fonction = SQRT;
    
    log_nat.lexem = FONCTION;
    log_nat.valeur.fonction = LOG;
    
    sinus.lexem = FONCTION;
    sinus.valeur.fonction = SIN;
    
    cosinus.lexem = FONCTION;
    cosinus.valeur.fonction = COS;
    
    exponentielle.lexem = FONCTION;
    exponentielle.valeur.fonction = EXP;
    
    valeur_absolue.lexem = FONCTION;
    valeur_absolue.valeur.fonction = ABS;
    
    tangente.lexem = FONCTION;
    tangente.valeur.fonction = TAN;
    
    // Expression: sin(sqrt(x^2 + y^2)) / (1 + exp(-abs(x*y)))
    printf("\n=== Test expression imbriquée complexe ===\n");
    
    // Construction de x^2
    Node X_ARBRE = creation_arbre(x, NULL, NULL);
    Node DEUX_ARBRE = creation_arbre(deux, NULL, NULL);
    Node X_CARRE = creation_arbre(puissance, &X_ARBRE, &DEUX_ARBRE);
    
    // Construction de y^2
    Node Y_ARBRE = creation_arbre(y, NULL, NULL);
    Node Y_CARRE = creation_arbre(puissance, &Y_ARBRE, &DEUX_ARBRE);
    
    // Construction de x^2 + y^2
    Node SOMME_CARRES = creation_arbre(plus, &X_CARRE, &Y_CARRE);
    
    // Construction de sqrt(x^2 + y^2)
    Node RACINE_SOMME = creation_arbre(racine_carree, &SOMME_CARRES, NULL);
    
    // Construction de sin(sqrt(x^2 + y^2))
    Node SIN_RACINE = creation_arbre(sinus, &RACINE_SOMME, NULL);
    
    // Construction de x*y
    Node X_MULT_Y = creation_arbre(mult, &X_ARBRE, &Y_ARBRE);
    
    // Construction de abs(x*y)
    Node ABS_PROD = creation_arbre(valeur_absolue, &X_MULT_Y, NULL);
    
    // Construction de -abs(x*y)
    typejeton neg_un;
    neg_un.lexem = REEL;
    neg_un.valeur.reel = -1.0;
    Node NEG_UN_ARBRE = creation_arbre(neg_un, NULL, NULL);
    Node NEG_ABS_PROD = creation_arbre(mult, &NEG_UN_ARBRE, &ABS_PROD);
    
    // Construction de exp(-abs(x*y))
    Node EXP_NEG_ABS = creation_arbre(exponentielle, &NEG_ABS_PROD, NULL);
    
    // Construction de 1 + exp(-abs(x*y))
    typejeton un;
    un.lexem = REEL;
    un.valeur.reel = 1.0;
    Node UN_ARBRE = creation_arbre(un, NULL, NULL);
    Node UN_PLUS_EXP = creation_arbre(plus, &UN_ARBRE, &EXP_NEG_ABS);
    
    // Construction de sin(sqrt(x^2 + y^2)) / (1 + exp(-abs(x*y)))
    Node EXPRESSION_FINALE = creation_arbre(div, &SIN_RACINE, &UN_PLUS_EXP);
    
    // Évaluation pour x=3 et y=4
    printf("Évaluation pour x=3 et y=4:\n");
    float resultat = evaluateur(&EXPRESSION_FINALE, 3.0, 4.0, &ERREUR_CODE);
    printf("Résultat: %f\n", resultat);
    printf("Code d'erreur: %d (attendu: 0)\n", ERREUR_CODE);
    afficher_arbre_couleur(&EXPRESSION_FINALE);
}

// Test d'expressions mettant plusieurs cas limites ensemble
void tester_expressions_cas_limites() {
    int ERREUR_CODE = 0;
    
    // Création des jetons
    typejeton x, y, un, div_op, log_nat, racine_carree, mult;
    
    x.lexem = VARIABLE;
    x.valeur.variable = 'x';
    
    y.lexem = VARIABLE;
    y.valeur.variable = 'y';
    
    un.lexem = REEL;
    un.valeur.reel = 1.0;
    
    div_op.lexem = OPERATEUR;
    div_op.valeur.operateur = DIV;
    
    log_nat.lexem = FONCTION;
    log_nat.valeur.fonction = LOG;
    
    racine_carree.lexem = FONCTION;
    racine_carree.valeur.fonction = SQRT;
    
    mult.lexem = OPERATEUR;
    mult.valeur.operateur = FOIS;
    
    // Expression: sqrt(log(x) / y)
    // Cette expression peut échouer de plusieurs façons:
    // - Si x <= 0, log(x) donne une erreur
    // - Si y = 0, division par zéro
    // - Si log(x)/y < 0, racine carrée d'un nombre négatif
    printf("\n=== Test expression avec plusieurs cas limites: sqrt(log(x) / y) ===\n");
    
    // Construction de log(x)
    Node X_ARBRE = creation_arbre(x, NULL, NULL);
    Node LOG_X = creation_arbre(log_nat, &X_ARBRE, NULL);
    
    // Construction de log(x) / y
    Node Y_ARBRE = creation_arbre(y, NULL, NULL);
    Node LOG_X_DIV_Y = creation_arbre(div_op, &LOG_X, &Y_ARBRE);
    
    // Construction de sqrt(log(x) / y)
    Node RACINE_LOG_DIV = creation_arbre(racine_carree, &LOG_X_DIV_Y, NULL);
    
    // Cas 1: x = 2, y = 1 (cas valide)
    printf("Cas 1 - x=2, y=1 (cas valide):\n");
    ERREUR_CODE = 0;
    float resultat = evaluateur(&RACINE_LOG_DIV, 2.0, 1.0, &ERREUR_CODE);
    printf("Résultat: %f\n", resultat);
    printf("Code d'erreur: %d (attendu: 0)\n", ERREUR_CODE);
    afficher_arbre_couleur(&RACINE_LOG_DIV);
    // Cas 2: x = 2, y = 0 (division par zéro)
    printf("\nCas 2 - x=2, y=0 (erreur: division par zéro):\n");
    ERREUR_CODE = 0;
    resultat = evaluateur(&RACINE_LOG_DIV, 2.0, 0.0, &ERREUR_CODE);
    printf("Résultat: %f\n", resultat);
    printf("Code d'erreur: %d (attendu: 311)\n", ERREUR_CODE);
    afficher_arbre_couleur(&RACINE_LOG_DIV);
    // Cas 3: x = 2, y = -1 (racine carrée d'un nombre négatif)
    printf("\nCas 3 - x=2, y=-1 (erreur: RACINE_NEGATIVE):\n");
    ERREUR_CODE = 0;
    resultat = evaluateur(&RACINE_LOG_DIV, 2.0, -1.0, &ERREUR_CODE);
    printf("Résultat: %f\n", resultat);
    printf("Code d'erreur: %d (attendu: %d)\n", ERREUR_CODE, RACINE_NEGATIVE);
    
    afficher_arbre_couleur(&RACINE_LOG_DIV);
}

// Fonction principale qui appelle tous les tests
int main() {
    printf("===== TESTS D'EXPRESSIONS MATHÉMATIQUES ET CAS D'ERREUR =====\n");
    
    // Test des expressions valides
    tester_expressions_valides();
    
    // Test des erreurs avec SQRT
    tester_erreur_racine_negative();
    
    // Test des erreurs avec LOG
    tester_erreurs_logarithme();
    
    // Test des erreurs avec TAN
    tester_erreur_tangente();
    
    // Test des erreurs avec SINC
    tester_erreur_sinc();
    
    // Test de division par zéro
    tester_division_par_zero();
    
    // Test d'expressions complexes imbriquées
    tester_expressions_imbriquees();
    
    // Test d'expressions avec cas limites multiples
    tester_expressions_cas_limites();
    
    return 0;
}