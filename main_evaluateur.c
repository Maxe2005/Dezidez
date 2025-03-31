#include "evaluateur.h"


//creation  d'un EXEMPLE :
Node creation_arbre(typejeton racine, Node* precedent, Node* suivant){
    Node arbre;
    arbre.jeton=racine;
    arbre.pjeton_preced=precedent;
    arbre.pjeton_suiv=suivant;
    return arbre;
}
int ERREUR_CODE=0;




int main(){
    int ERREUR_CODE = 0;
    
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
    
    // Test avec des valeurs qui pourraient causer des erreurs (y=0 pour log(y))
    printf("\nTest avec des valeurs problématiques - x=2 et y=0 :\n");
    ERREUR_CODE = 0; // Réinitialisation du code d'erreur
    printf("Résultat: %f\n", evaluateur(&EXPRESSION_FINALE_2_ARBRE, 2.0, 0.0, &ERREUR_CODE));
    printf("Code d'erreur: %d\n", ERREUR_CODE);
    
    return 0;
}
// FIN EXEMPLE