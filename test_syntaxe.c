#include "test_syntaxe.h"
#include <stdio.h>
#include <stdlib.h>

// Fonction principale qui exécute tous les tests
void tester_syntaxe() {
    /*test_valide1();
    test_valide2();
    test_valide3();
    test_erreur_operateurs_a_la_suite1();
    test_erreur_operateurs_a_la_suite2();
    test_erreur_manque_operateur();
    test_erreur_manque_parenthese();
    test_probleme_parentheses_fonctions1();
    test_probleme_parentheses_fonctions2();
    test_probleme_apres_reel();
    test_membre_vide();
    test_parenthese_fermee_1er_jeton();*/
    test_expression_valide();
    //test_cos_plus_sin();
}

// Fonctions de test
void test_valide1() {
    typejeton tab[] = {
        {REEL, {.reel = 5.0}},
        {OPERATEUR, {.operateur = PLUS}},
        {REEL, {.reel = 3.0}},
        {FIN, {0}}
    };
    typeerreur erreur = 0;
    Node* arbre_syntaxique = Syntaxique(tab, &erreur);
    
    // Construction manuelle de l'arbre attendu: 5 + 3
    Node* racine = (Node*)malloc(sizeof(Node));
    racine->jeton.lexem = OPERATEUR;
    racine->jeton.valeur.operateur = PLUS;
    
    Node* val5 = (Node*)malloc(sizeof(Node));
    val5->jeton.lexem = REEL;
    val5->jeton.valeur.reel = 5.0;
    val5->pjeton_preced = NULL;
    val5->pjeton_suiv = NULL;
    
    Node* val3 = (Node*)malloc(sizeof(Node));
    val3->jeton.lexem = REEL;
    val3->jeton.valeur.reel = 3.0;
    val3->pjeton_preced = NULL;
    val3->pjeton_suiv = NULL;
    
    racine->pjeton_preced = val5;
    racine->pjeton_suiv = val3;
    
    // Comparer les deux arbres
    bool arbres_identiques = comparer_arbres(arbre_syntaxique, racine);
    
    if (syntaxeVerbose >= 1) printf("Test valide 1 (5 + 3)               : %s. Erreur=%d\n", 
                                     (erreur == 0 && arbres_identiques) ? "OK" : "ÉCHEC", erreur);
    
    if (syntaxeVerbose >= 2) {
        printf("Affichage arbre attendu :\n");
        afficher_arbre_couleur(racine);
        printf("Affichage arbre syntaxique :\n");
        afficher_arbre_couleur(arbre_syntaxique);
    }
    
    // Libération de la mémoire
    liberer_arbre(arbre_syntaxique);
    liberer_arbre(racine);
}

void test_valide2() {
    typejeton tab[] = {
        {PAR_OUV, {0}},
        {REEL, {.reel = 7.0}},
        {OPERATEUR, {.operateur = FOIS}},
        {REEL, {.reel = 2.0}},
        {PAR_FERM, {0}},
        {FIN, {0}}
    };
    typeerreur erreur = 0;
    Node* arbre_syntaxique = Syntaxique(tab, &erreur);
    
    // Construction manuelle de l'arbre attendu: (7 * 2)
    Node* racine = (Node*)malloc(sizeof(Node));
    racine->jeton.lexem = OPERATEUR;
    racine->jeton.valeur.operateur = FOIS;
    
    Node* val7 = (Node*)malloc(sizeof(Node));
    val7->jeton.lexem = REEL;
    val7->jeton.valeur.reel = 7.0;
    val7->pjeton_preced = NULL;
    val7->pjeton_suiv = NULL;
    
    Node* val2 = (Node*)malloc(sizeof(Node));
    val2->jeton.lexem = REEL;
    val2->jeton.valeur.reel = 2.0;
    val2->pjeton_preced = NULL;
    val2->pjeton_suiv = NULL;
    
    racine->pjeton_preced = val7;
    racine->pjeton_suiv = val2;
    
    // Comparer les deux arbres
    bool arbres_identiques = comparer_arbres(arbre_syntaxique, racine);
    
    if (syntaxeVerbose >= 1) printf("Test valide 2 ((7 * 2))             : %s. Erreur=%d\n", 
                                     (erreur == 0 && arbres_identiques) ? "OK" : "ÉCHEC", erreur);
    
    if (syntaxeVerbose >= 2) {
        printf("Affichage arbre attendu :\n");
        afficher_arbre_couleur(racine);
        printf("Affichage arbre syntaxique :\n");
        afficher_arbre_couleur(arbre_syntaxique);
    }
    
    // Libération de la mémoire
    liberer_arbre(arbre_syntaxique);
    liberer_arbre(racine);
}

void test_valide3() {
    typejeton tab[] = {
        {VARIABLE, {.variable = 'x'}},
        {OPERATEUR, {.operateur = PUIS}},
        {REEL, {.reel = 2.0}},
        {FIN, {0}}
    };
    typeerreur erreur = 0;
    Node* arbre_syntaxique = Syntaxique(tab, &erreur);
    
    // Construction manuelle de l'arbre attendu: x ^ 2
    Node* racine = (Node*)malloc(sizeof(Node));
    racine->jeton.lexem = OPERATEUR;
    racine->jeton.valeur.operateur = PUIS;
    
    Node* varX = (Node*)malloc(sizeof(Node));
    varX->jeton.lexem = VARIABLE;
    varX->jeton.valeur.variable = 'x';
    varX->pjeton_preced = NULL;
    varX->pjeton_suiv = NULL;
    
    Node* val2 = (Node*)malloc(sizeof(Node));
    val2->jeton.lexem = REEL;
    val2->jeton.valeur.reel = 2.0;
    val2->pjeton_preced = NULL;
    val2->pjeton_suiv = NULL;
    
    racine->pjeton_preced = varX;
    racine->pjeton_suiv = val2;
    
    // Comparer les deux arbres
    bool arbres_identiques = comparer_arbres(arbre_syntaxique, racine);
    
    if (syntaxeVerbose >= 1) printf("Test valide 3 (x ^ 2)               : %s. Erreur=%d\n", 
                                     (erreur == 0 && arbres_identiques) ? "OK" : "ÉCHEC", erreur);
    
    if (syntaxeVerbose >= 2) {
        printf("Affichage arbre attendu :\n");
        afficher_arbre_couleur(racine);
        printf("Affichage arbre syntaxique :\n");
        afficher_arbre_couleur(arbre_syntaxique);
    }
    
    // Libération de la mémoire
    liberer_arbre(arbre_syntaxique);
    liberer_arbre(racine);
}

void test_erreur_operateurs_a_la_suite1() {
    // 5 + * 3
    typejeton tab[] = {
        {REEL, {.reel = 5.0}},
        {OPERATEUR, {.operateur = PLUS}},
        {OPERATEUR, {.operateur = FOIS}},
        {REEL, {.reel = 3.0}},
        {FIN, {0}}
    };
    typeerreur erreur = 0;
    Node* arbre_syntaxique = Syntaxique(tab, &erreur);
    
    // Dans ce cas, on attend une erreur, pas d'arbre à comparer
    bool arbres_identiques = true; // Non applicable mais nécessaire pour l'affichage
    
    if (syntaxeVerbose >= 1) printf("Test erreur opérateurs à la suite 1  : %s. Erreur=%d\n", 
                                     (erreur == MEMBRE_VIDE) ? "OK" : "ÉCHEC", erreur);
    
    if (arbre_syntaxique != NULL) {
        liberer_arbre(arbre_syntaxique);
    }
}

void test_erreur_operateurs_a_la_suite2() {
    // 8 * + 2
    typejeton tab[] = {
        {REEL, {.reel = 8.0}},
        {OPERATEUR, {.operateur = FOIS}},
        {OPERATEUR, {.operateur = PUIS}},
        {REEL, {.reel = 2.0}},
        {FIN, {0}}
    };
    typeerreur erreur = 0;
    Node* arbre_syntaxique = Syntaxique(tab, &erreur);
    
    // Dans ce cas, on attend une erreur, pas d'arbre à comparer
    bool arbres_identiques = true; // Non applicable mais nécessaire pour l'affichage
    
    if (syntaxeVerbose >= 1) printf("Test erreur opérateurs à la suite 2  : %s. Erreur=%d\n", 
                                     (erreur == MEMBRE_VIDE) ? "OK" : "ÉCHEC", erreur);
    
    if (arbre_syntaxique != NULL) {
        liberer_arbre(arbre_syntaxique);
    }
}

void test_erreur_manque_operateur() {
    // 5 +
    typejeton tab[] = {
        {REEL, {.reel = 5.0}},
        {OPERATEUR, {.operateur = PLUS}},
        {FIN, {0}}
    };
    typeerreur erreur = 0;
    Node* arbre_syntaxique = Syntaxique(tab, &erreur);
    
    // Dans ce cas, on attend une erreur, pas d'arbre à comparer
    bool arbres_identiques = true; // Non applicable mais nécessaire pour l'affichage
    
    if (syntaxeVerbose >= 1) printf("Test erreur manque opérateur         : %s. Erreur=%d\n", 
                                     (erreur == MEMBRE_VIDE) ? "OK" : "ÉCHEC", erreur);
    
    if (arbre_syntaxique != NULL) {
        liberer_arbre(arbre_syntaxique);
    }
}

void test_erreur_manque_parenthese() {
    // (5 +
    typejeton tab[] = {
        {PAR_OUV, {0}},
        {REEL, {.reel = 5.0}},
        {OPERATEUR, {.operateur = PLUS}},
        {FIN, {0}}
    };
    typeerreur erreur = 0;
    Node* arbre_syntaxique = Syntaxique(tab, &erreur);
    
    // Dans ce cas, on attend une erreur, pas d'arbre à comparer
    bool arbres_identiques = true; // Non applicable mais nécessaire pour l'affichage
    
    if (syntaxeVerbose >= 1) printf("Test erreur manque parenthèse        : %s. Erreur=%d\n", 
                                     (erreur == PROBLEMES_NOMBRE_PARENTHESES) ? "OK" : "ÉCHEC", erreur);
    
    if (arbre_syntaxique != NULL) {
        liberer_arbre(arbre_syntaxique);
    }
}

void test_probleme_parentheses_fonctions1() {
    typejeton tab[] = {
        {FONCTION, {.fonction = COS}},
        {REEL, {.reel = 5.0}},
        {FIN, {0}}
    };
    
    typeerreur erreur = 0;
    Node* arbre_syntaxique = Syntaxique(tab, &erreur);
    
    // Dans ce cas, on attend une erreur, pas d'arbre à comparer
    bool arbres_identiques = true; // Non applicable mais nécessaire pour l'affichage
    
    if (syntaxeVerbose >= 1) printf("Test problème parenthèses fonctions 1: %s. Erreur=%d\n", 
                                     (erreur == PROBLEME_PARENTHESES_FONCTIONS) ? "OK" : "ÉCHEC", erreur);
    
    if (arbre_syntaxique != NULL) {
        liberer_arbre(arbre_syntaxique);
    }
}

void test_probleme_parentheses_fonctions2() {
    typejeton tab[] = {
        {FONCTION, {.fonction = COS}},
        {PAR_OUV, {0}},
        {REEL, {.reel = 5.0}},
        {FIN, {0}}
    };
    
    typeerreur erreur = 0;
    Node* arbre_syntaxique = Syntaxique(tab, &erreur);
    
    // Dans ce cas, on attend une erreur, pas d'arbre à comparer
    bool arbres_identiques = true; // Non applicable mais nécessaire pour l'affichage
    
    if (syntaxeVerbose >= 1) printf("Test problème parenthèses fonctions 2: %s. Erreur=%d\n", 
                                     (erreur == PROBLEMES_NOMBRE_PARENTHESES) ? "OK" : "ÉCHEC", erreur);
    
    if (arbre_syntaxique != NULL) {
        liberer_arbre(arbre_syntaxique);
    }
}

void test_probleme_apres_reel() {
    typejeton tab[] = {
        {REEL, {.reel = 3.0}},
        {REEL, {.reel = 3.0}},
        {OPERATEUR, {.operateur = PLUS}},
        {REEL, {.reel = 6.0}},
        {FIN, {0}}
    };
    
    typeerreur erreur = 0;
    Node* arbre_syntaxique = Syntaxique(tab, &erreur);
    
    // Dans ce cas, on attend une erreur, pas d'arbre à comparer
    bool arbres_identiques = true; // Non applicable mais nécessaire pour l'affichage
    
    if (syntaxeVerbose >= 1) printf("Test problème après réel            : %s. Erreur=%d\n", 
                                     (erreur == PROBLEME_APRES_REEL) ? "OK" : "ÉCHEC", erreur);
    
    if (arbre_syntaxique != NULL) {
        liberer_arbre(arbre_syntaxique);
    }
}

void test_membre_vide() {
    typejeton tab[] = {
        {REEL, {.reel = 3.0}},
        {OPERATEUR, {.operateur = PLUS}},
        {PAR_OUV, {0}},
        {PAR_FERM, {0}},
        {FIN, {0}}
    };
    
    typeerreur erreur = 0;
    Node* arbre_syntaxique = Syntaxique(tab, &erreur);
    
    // Dans ce cas, on attend une erreur, pas d'arbre à comparer
    bool arbres_identiques = true; // Non applicable mais nécessaire pour l'affichage
    
    if (syntaxeVerbose >= 1) printf("Test membre vide                    : %s. Erreur=%d\n", 
                                     (erreur == MEMBRE_VIDE) ? "OK" : "ÉCHEC", erreur);
    
    if (arbre_syntaxique != NULL) {
        liberer_arbre(arbre_syntaxique);
    }
}

void test_parenthese_fermee_1er_jeton() {
    typejeton tab[] = {
        {PAR_FERM, {0}},
        {FIN, {0}}
    };
    
    typeerreur erreur = 0;
    Node* arbre_syntaxique = Syntaxique(tab, &erreur);
    
    // Dans ce cas, on attend une erreur, pas d'arbre à comparer
    bool arbres_identiques = true; // Non applicable mais nécessaire pour l'affichage
    
    if (syntaxeVerbose >= 1) printf("Test parenthèse fermée 1er jeton    : %s. Erreur=%d\n", 
                                     (erreur == PROBLEMES_NOMBRE_PARENTHESES) ? "OK" : "ÉCHEC", erreur);
    
    if (arbre_syntaxique != NULL) {
        liberer_arbre(arbre_syntaxique);
    }
}

void test_expression_valide() {
    typejeton tab[] = {
        {FONCTION, {.fonction = COS}},
        {PAR_OUV, {0}},
        {REEL, {.reel = 3.0}},
        {OPERATEUR, {.operateur = PLUS}},
        {REEL, {.reel = 6.0}},
        {OPERATEUR, {.operateur = FOIS}},
        {VARIABLE, {.variable = 'X'}},
        {PAR_FERM, {0}},
        {OPERATEUR, {.operateur = PLUS}},
        {FONCTION, {.fonction = COS}},
        {PAR_OUV, {0}},
        {REEL, {.reel = 7.0}},
        {PAR_FERM, {0}},
        {OPERATEUR, {.operateur = FOIS}},
        {PAR_OUV, {0}},
        {REEL, {.reel = 12.0}},
        {OPERATEUR, {.operateur = PLUS}},
        {REEL, {.reel = 3.0}},
        {PAR_FERM, {0}},
        {FIN, {0}}
    };
    
    typeerreur erreur = 0;
    Node* arbre_syntaxique = Syntaxique(tab, &erreur);
    
    // Construction manuelle de l'arbre attendu pour cos(3 + 6*X) + cos(7) * (12 + 3)
    // Opérateur principal: +
    Node* racine = (Node*)malloc(sizeof(Node));
    racine->jeton.lexem = OPERATEUR;
    racine->jeton.valeur.operateur = PLUS;
    
    // Partie gauche: cos(3 + 6*X)
    Node* cos1 = (Node*)malloc(sizeof(Node));
    cos1->jeton.lexem = FONCTION;
    cos1->jeton.valeur.fonction = COS;
    
    // Contenu de cos1: (3 + 6*X)
    Node* addition = (Node*)malloc(sizeof(Node));
    addition->jeton.lexem = OPERATEUR;
    addition->jeton.valeur.operateur = PLUS;
    
    Node* val3 = (Node*)malloc(sizeof(Node));
    val3->jeton.lexem = REEL;
    val3->jeton.valeur.reel = 3.0;
    val3->pjeton_preced = NULL;
    val3->pjeton_suiv = NULL;
    
    // 6*X
    Node* multiplication = (Node*)malloc(sizeof(Node));
    multiplication->jeton.lexem = OPERATEUR;
    multiplication->jeton.valeur.operateur = FOIS;
    
    Node* val6 = (Node*)malloc(sizeof(Node));
    val6->jeton.lexem = REEL;
    val6->jeton.valeur.reel = 6.0;
    val6->pjeton_preced = NULL;
    val6->pjeton_suiv = NULL;
    
    Node* varX = (Node*)malloc(sizeof(Node));
    varX->jeton.lexem = VARIABLE;
    varX->jeton.valeur.variable = 'X';
    varX->pjeton_preced = NULL;
    varX->pjeton_suiv = NULL;
    
    multiplication->pjeton_preced = val6;
    multiplication->pjeton_suiv = varX;
    
    addition->pjeton_preced = val3;
    addition->pjeton_suiv = multiplication;
    
    cos1->pjeton_preced = addition;
    cos1->pjeton_suiv = NULL;
    
    // Partie droite: cos(7) * (12 + 3)
    Node* multiplication2 = (Node*)malloc(sizeof(Node));
    multiplication2->jeton.lexem = OPERATEUR;
    multiplication2->jeton.valeur.operateur = FOIS;
    
    // cos(7)
    Node* cos2 = (Node*)malloc(sizeof(Node));
    cos2->jeton.lexem = FONCTION;
    cos2->jeton.valeur.fonction = COS;
    
    Node* val7 = (Node*)malloc(sizeof(Node));
    val7->jeton.lexem = REEL;
    val7->jeton.valeur.reel = 7.0;
    val7->pjeton_preced = NULL;
    val7->pjeton_suiv = NULL;
    
    cos2->pjeton_preced = val7;
    cos2->pjeton_suiv = NULL;
    
    // (12 + 3)
    Node* addition2 = (Node*)malloc(sizeof(Node));
    addition2->jeton.lexem = OPERATEUR;
    addition2->jeton.valeur.operateur = PLUS;
    
    Node* val12 = (Node*)malloc(sizeof(Node));
    val12->jeton.lexem = REEL;
    val12->jeton.valeur.reel = 12.0;
    val12->pjeton_preced = NULL;
    val12->pjeton_suiv = NULL;
    
    Node* val3_2 = (Node*)malloc(sizeof(Node));
    val3_2->jeton.lexem = REEL;
    val3_2->jeton.valeur.reel = 3.0;
    val3_2->pjeton_preced = NULL;
    val3_2->pjeton_suiv = NULL;
    
    addition2->pjeton_preced = val12;
    addition2->pjeton_suiv = val3_2;
    
    multiplication2->pjeton_preced = cos2;
    multiplication2->pjeton_suiv = addition2;
    
    // Connexion globale
    racine->pjeton_preced = cos1;
    racine->pjeton_suiv = multiplication2;
    
    // Comparer les deux arbres
    bool arbres_identiques = comparer_arbres(arbre_syntaxique, racine);
    
    if (syntaxeVerbose >= 1) printf("Test expression valide              : %s. Erreur=%d\n", 
                                     (erreur == 0 && arbres_identiques) ? "OK" : "ÉCHEC", erreur);
    
    if (syntaxeVerbose >= 2) {
        printf("Affichage arbre attendu :\n");
        afficher_arbre_couleur(racine);
        printf("Affichage arbre syntaxique :\n");
        afficher_arbre_couleur(arbre_syntaxique);
    }
    
    // Libération de la mémoire
    liberer_arbre(arbre_syntaxique);
    liberer_arbre(racine);
}

void test_cos_plus_sin() {
    // Tableau de jetons pour l'expression "sin(3) * cos(2)"
    typejeton tab[] = {
        {FONCTION, {.fonction = SIN}},
        {PAR_OUV, {0}},
        {REEL, {.reel = 3.0}},
        {PAR_FERM, {0}},
        {OPERATEUR, {.operateur = FOIS}},
        {FONCTION, {.fonction = COS}},
        {PAR_OUV, {0}},
        {REEL, {.reel = 2.0}},
        {PAR_FERM, {0}},
        {FIN, {0}}
    };

    // Afficher la liste de jetons
    if (syntaxeVerbose >= 1) printf("Expression: sin(3) * cos(2)\n");
    if (syntaxeVerbose >= 2) afficher_liste_jetons(tab, 0, 9);

    // Création de l'arbre par la fonction Syntaxique
    typeerreur erreur = 0;
    Node* arbre_syntaxique = Syntaxique(tab, &erreur);

    // Création manuelle de l'arbre attendu pour "sin(3) * cos(2)"
    // Niveau 1: Opérateur * principal
    Node* racine = (Node*)malloc(sizeof(Node));
    racine->jeton.lexem = OPERATEUR;
    racine->jeton.valeur.operateur = FOIS;

    // Branche gauche: sin(3)
    Node* sin = (Node*)malloc(sizeof(Node));
    sin->jeton.lexem = FONCTION;
    sin->jeton.valeur.fonction = SIN;

    Node* val3 = (Node*)malloc(sizeof(Node));
    val3->jeton.lexem = REEL;
    val3->jeton.valeur.reel = 3.0;
    val3->pjeton_preced = NULL;
    val3->pjeton_suiv = NULL;

    // Branche droite: cos(2)
    Node* cos = (Node*)malloc(sizeof(Node));
    cos->jeton.lexem = FONCTION;
    cos->jeton.valeur.fonction = COS;

    Node* val2 = (Node*)malloc(sizeof(Node));
    val2->jeton.lexem = REEL;
    val2->jeton.valeur.reel = 2.0;
    val2->pjeton_preced = NULL;
    val2->pjeton_suiv = NULL;

    // Connexions de l'arbre
    racine->pjeton_preced = sin;
    racine->pjeton_suiv = cos;

    sin->pjeton_preced = val3;
    sin->pjeton_suiv = NULL;

    cos->pjeton_preced = val2;
    cos->pjeton_suiv = NULL;

    // Comparaison des deux arbres
    bool arbres_identiques = comparer_arbres(arbre_syntaxique, racine);

    if (syntaxeVerbose >= 1) printf("Test sin(3) * cos(2)                : %s. Erreur=%d\n",
           (erreur == 0 && arbres_identiques) ? "OK" : "ÉCHEC", erreur);

    // Afficher arbre
    if (syntaxeVerbose >= 2) {
        printf("Affichage arbre attendu :\n");
        afficher_arbre_couleur(racine);
        printf("Affichage arbre syntaxique :\n");
        afficher_arbre_couleur(arbre_syntaxique);
    }

    // Libération de la mémoire
    liberer_arbre(arbre_syntaxique);
    liberer_arbre(racine);
}