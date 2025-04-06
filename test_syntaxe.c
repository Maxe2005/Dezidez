#include "test_syntaxe.h"
#include <stdio.h>
#include <stdlib.h>

static int nbTests = 0;

// Fonction principale qui exécute tous les tests
void tester_syntaxe() {
    test_5_plus_3_valide();
    test_parenthese_ouverte_7_fois_2_valide();
    test_x_puis_2_valide();
    test_5_plus_etoile_3_erreur();
    test_8_fois_plus_2_erreur();
    test_5_plus_erreur();
    test_parenthese_ouverte_5_erreur();
    test_cos_parenthese_ouverte_5_erreur();
    test_cos_3_fois_cos_2_valide();
    test_3_plus_6X_fois_cos_7_fois_parenthese_12_plus_3_valide();

    test_4_minus_53_erreur();
    test_0_plus_0_valide();
    test_5_3_minus_4_2_valide();
    test_10_div_33_valide();
    test_3_valide();
    test_parenthese_ouverte_erreur();
    test_X_valide();
    test_div_plus_moins_erreur();
    test_1_plus_2_plus_3_valide();
    test_cos_erreur();
    test_sin_53_valide();
    test_cos_sin_parenthese_ouverte_erreur();
    test_3_fois_cos_X_plus_X_fois_exp_2X_valide();
    test_cos_56_plus_sin_tan_12_valide();
    test_expression_longue_valide();
    test_cos_valeur_tres_grande_valide();
    test_valeur_precise_valide();
    test_XEXP_erreur();
    test_val_neg_valide();
    test_sinc();
    test_plus_3_valide();
    test_sin_neg_x_valide();
}

// Fonctions de test
void test_5_plus_3_valide() {
    typejeton tab[] = { {REEL, {.reel = 5.0}}, {OPERATEUR, {.operateur = PLUS}}, {REEL, {.reel = 3.0}}, {FIN, {0}} };
    typeerreur erreur = 0;
    Node* arbre_syntaxique = Syntaxique(tab, &erreur);

    // Construction manuelle de l'arbre attendu: 5 + 3
    Node* racine = (Node*)malloc(sizeof(Node));
    racine->jeton.lexem = OPERATEUR; racine->jeton.valeur.operateur = PLUS;

    Node* val5 = (Node*)malloc(sizeof(Node));
    val5->jeton.lexem = REEL; val5->jeton.valeur.reel = 5.0; val5->pjeton_preced = NULL; val5->pjeton_suiv = NULL;

    Node* val3 = (Node*)malloc(sizeof(Node));
    val3->jeton.lexem = REEL; val3->jeton.valeur.reel = 3.0; val3->pjeton_preced = NULL; val3->pjeton_suiv = NULL;

    racine->pjeton_preced = val5; racine->pjeton_suiv = val3;

    // Comparer les deux arbres
    bool arbres_identiques = comparer_arbres(arbre_syntaxique, racine);

    if (syntaxeVerbose >= 1) printf("Test %d : 5 + 3 : %s. Erreur = %s\n", ++nbTests, (erreur == 0 && arbres_identiques) ? "\033[0;32mOK\033[0m" : "\033[0;31mECHEC\033[0m", get_error_message[erreur]);

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

void test_parenthese_ouverte_7_fois_2_valide() {
    typejeton tab[] = { {PAR_OUV, {0}}, {REEL, {.reel = 7.0}}, {OPERATEUR, {.operateur = FOIS}}, {REEL, {.reel = 2.0}}, {PAR_FERM, {0}}, {FIN, {0}} };
    typeerreur erreur = 0;
    Node* arbre_syntaxique = Syntaxique(tab, &erreur);

    // Construction manuelle de l'arbre attendu: (7 * 2)
    Node* racine = (Node*)malloc(sizeof(Node));
    racine->jeton.lexem = OPERATEUR; racine->jeton.valeur.operateur = FOIS;

    Node* val7 = (Node*)malloc(sizeof(Node));
    val7->jeton.lexem = REEL; val7->jeton.valeur.reel = 7.0; val7->pjeton_preced = NULL; val7->pjeton_suiv = NULL;

    Node* val2 = (Node*)malloc(sizeof(Node));
    val2->jeton.lexem = REEL; val2->jeton.valeur.reel = 2.0; val2->pjeton_preced = NULL; val2->pjeton_suiv = NULL;

    racine->pjeton_preced = val7; racine->pjeton_suiv = val2;

    // Comparer les deux arbres
    bool arbres_identiques = comparer_arbres(arbre_syntaxique, racine);

    if (syntaxeVerbose >= 1) printf("Test %d : (7 * 2) : %s. Erreur = %s\n", ++nbTests, (erreur == 0 && arbres_identiques) ? "\033[0;32mOK\033[0m" : "\033[0;31mECHEC\033[0m", get_error_message[erreur]);

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

void test_x_puis_2_valide() {
    typejeton tab[] = { {VARIABLE, {.variable = 'x'}}, {OPERATEUR, {.operateur = PUIS}}, {REEL, {.reel = 2.0}}, {FIN, {0}} };
    typeerreur erreur = 0;
    Node* arbre_syntaxique = Syntaxique(tab, &erreur);

    // Construction manuelle de l'arbre attendu: x ^ 2
    Node* racine = (Node*)malloc(sizeof(Node));
    racine->jeton.lexem = OPERATEUR; racine->jeton.valeur.operateur = PUIS;

    Node* varX = (Node*)malloc(sizeof(Node));
    varX->jeton.lexem = VARIABLE; varX->jeton.valeur.variable = 'x'; varX->pjeton_preced = NULL; varX->pjeton_suiv = NULL;

    Node* val2 = (Node*)malloc(sizeof(Node));
    val2->jeton.lexem = REEL; val2->jeton.valeur.reel = 2.0; val2->pjeton_preced = NULL; val2->pjeton_suiv = NULL;

    racine->pjeton_preced = varX; racine->pjeton_suiv = val2;

    // Comparer les deux arbres
    bool arbres_identiques = comparer_arbres(arbre_syntaxique, racine);

    if (syntaxeVerbose >= 1) printf("Test %d : x ^ 2 : %s. Erreur = %s\n", ++nbTests, (erreur == 0 && arbres_identiques) ? "\033[0;32mOK\033[0m" : "\033[0;31mECHEC\033[0m", get_error_message[erreur]);

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

void test_5_plus_etoile_3_erreur() {
    // 5 + * 3
    typejeton tab[] = { {REEL, {.reel = 5.0}}, {OPERATEUR, {.operateur = PLUS}}, {OPERATEUR, {.operateur = FOIS}}, {REEL, {.reel = 3.0}}, {FIN, {0}} };
    typeerreur erreur = 0;
    Node* arbre_syntaxique = Syntaxique(tab, &erreur);

    // Dans ce cas, on attend une erreur, pas d'arbre à comparer
    bool arbres_identiques = true; // Non applicable mais nécessaire pour l'affichage

    if (syntaxeVerbose >= 1) printf("Test %d : 5 + * 3 : %s. Erreur = %s\n", ++nbTests, (erreur == MEMBRE_VIDE) ? "\033[0;32mOK\033[0m" : "\033[0;31mECHEC\033[0m", get_error_message[erreur]);

    if (arbre_syntaxique != NULL) {
        liberer_arbre(arbre_syntaxique);
    }
}

void test_8_fois_plus_2_erreur() {
    // 8 * + 2
    typejeton tab[] = { {REEL, {.reel = 8.0}}, {OPERATEUR, {.operateur = FOIS}}, {OPERATEUR, {.operateur = PUIS}}, {REEL, {.reel = 2.0}}, {FIN, {0}} };
    typeerreur erreur = 0;
    Node* arbre_syntaxique = Syntaxique(tab, &erreur);

    // Dans ce cas, on attend une erreur, pas d'arbre à comparer
    bool arbres_identiques = true; // Non applicable mais nécessaire pour l'affichage

    if (syntaxeVerbose >= 1) printf("Test %d : 8 * + 2 : %s. Erreur = %s\n", ++nbTests, (erreur == MEMBRE_VIDE) ? "\033[0;32mOK\033[0m" : "\033[0;31mECHEC\033[0m", get_error_message[erreur]);

    if (arbre_syntaxique != NULL) {
        liberer_arbre(arbre_syntaxique);
    }
}

void test_5_plus_erreur() {
    // 5 +
    typejeton tab[] = { {REEL, {.reel = 5.0}}, {OPERATEUR, {.operateur = PLUS}}, {FIN, {0}} };
    typeerreur erreur = 0;
    Node* arbre_syntaxique = Syntaxique(tab, &erreur);

    // Dans ce cas, on attend une erreur, pas d'arbre à comparer
    bool arbres_identiques = true; // Non applicable mais nécessaire pour l'affichage

    if (syntaxeVerbose >= 1) printf("Test %d : 5 + : %s. Erreur = %s\n", ++nbTests, (erreur == MEMBRE_VIDE) ? "\033[0;32mOK\033[0m" : "\033[0;31mECHEC\033[0m", get_error_message[erreur]);

    if (arbre_syntaxique != NULL) {
        liberer_arbre(arbre_syntaxique);
    }
}

void test_parenthese_ouverte_5_erreur() {
    // (5 +
    typejeton tab[] = { {PAR_OUV, {0}}, {REEL, {.reel = 5.0}}, {OPERATEUR, {.operateur = PLUS}}, {FIN, {0}} };
    typeerreur erreur = 0;
    Node* arbre_syntaxique = Syntaxique(tab, &erreur);

    // Dans ce cas, on attend une erreur, pas d'arbre à comparer
    bool arbres_identiques = true; // Non applicable mais nécessaire pour l'affichage

    if (syntaxeVerbose >= 1) printf("Test %d : (5 + : %s. Erreur = %s\n", ++nbTests, (erreur == PROBLEMES_NOMBRE_PARENTHESES) ? "\033[0;32mOK\033[0m" : "\033[0;31mECHEC\033[0m", get_error_message[erreur]);

    if (arbre_syntaxique != NULL) {
        liberer_arbre(arbre_syntaxique);
    }
}

void test_cos_parenthese_ouverte_5_erreur() {
    // cos(5
    typejeton tab[] = { {FONCTION, {.fonction = COS}}, {REEL, {.reel = 5.0}}, {FIN, {0}} };
    typeerreur erreur = 0;
    Node* arbre_syntaxique = Syntaxique(tab, &erreur);

    // Dans ce cas, on attend une erreur, pas d'arbre à comparer
    bool arbres_identiques = true; // Non applicable mais nécessaire pour l'affichage

    if (syntaxeVerbose >= 1) printf("Test %d : cos(5 : %s. Erreur = %s\n", ++nbTests, (erreur == PROBLEME_PARENTHESES_FONCTIONS) ? "\033[0;32mOK\033[0m" : "\033[0;31mECHEC\033[0m", get_error_message[erreur]);

    if (arbre_syntaxique != NULL) {
        liberer_arbre(arbre_syntaxique);
    }
}

void test_cos_3_fois_cos_2_valide() {
    // Tableau de jetons pour l'expression "sin(3) * cos(2)"
    typejeton tab[] = { {FONCTION, {.fonction = SIN}}, {PAR_OUV, {0}}, {REEL, {.reel = 3.0}}, {PAR_FERM, {0}}, {OPERATEUR, {.operateur = FOIS}}, {FONCTION, {.fonction = COS}}, {PAR_OUV, {0}}, {REEL, {.reel = 2.0}}, {PAR_FERM, {0}}, {FIN, {0}} };

    // Afficher la liste de jetons
    // if (syntaxeVerbose >= 1) printf("Expression: sin(3) * cos(2)\n");
    if (syntaxeVerbose >= 2) afficher_liste_jetons(tab, 0, 9);

    // Création de l'arbre par la fonction Syntaxique
    typeerreur erreur = 0;
    Node* arbre_syntaxique = Syntaxique(tab, &erreur);

    // Création manuelle de l'arbre attendu pour "sin(3) * cos(2)"
    // Niveau 1: Opérateur * principal
    Node* racine = (Node*)malloc(sizeof(Node));
    racine->jeton.lexem = OPERATEUR; racine->jeton.valeur.operateur = FOIS;

    // Branche gauche: sin(3)
    Node* sin = (Node*)malloc(sizeof(Node));
    sin->jeton.lexem = FONCTION; sin->jeton.valeur.fonction = SIN;

    Node* val3 = (Node*)malloc(sizeof(Node));
    val3->jeton.lexem = REEL; val3->jeton.valeur.reel = 3.0; val3->pjeton_preced = NULL; val3->pjeton_suiv = NULL;

    // Branche droite: cos(2)
    Node* cos = (Node*)malloc(sizeof(Node));
    cos->jeton.lexem = FONCTION; cos->jeton.valeur.fonction = COS;

    Node* val2 = (Node*)malloc(sizeof(Node));
    val2->jeton.lexem = REEL; val2->jeton.valeur.reel = 2.0; val2->pjeton_preced = NULL; val2->pjeton_suiv = NULL;

    // Connexions de l'arbre
    racine->pjeton_preced = sin; racine->pjeton_suiv = cos;

    sin->pjeton_preced = val3; sin->pjeton_suiv = NULL;

    cos->pjeton_preced = val2; cos->pjeton_suiv = NULL;

    // Comparaison des deux arbres
    bool arbres_identiques = comparer_arbres(arbre_syntaxique, racine);

    if (syntaxeVerbose >= 1) printf("Test %d : sin(3) * cos(2) : %s. Erreur = %s\n", ++nbTests, (erreur == 0 && arbres_identiques) ? "\033[0;32mOK\033[0m" : "\033[0;31mECHEC\033[0m", get_error_message[erreur]);

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

void test_3_plus_6X_fois_cos_7_fois_parenthese_12_plus_3_valide() {
    typejeton tab[] = { {FONCTION, {.fonction = COS}}, {PAR_OUV, {0}}, {REEL, {.reel = 3.0}}, {OPERATEUR, {.operateur = PLUS}}, {REEL, {.reel = 6.0}}, {OPERATEUR, {.operateur = FOIS}}, {VARIABLE, {.variable = 'X'}}, {PAR_FERM, {0}}, {OPERATEUR, {.operateur = PLUS}}, {FONCTION, {.fonction = COS}}, {PAR_OUV, {0}}, {REEL, {.reel = 7.0}}, {PAR_FERM, {0}}, {OPERATEUR, {.operateur = FOIS}}, {PAR_OUV, {0}}, {REEL, {.reel = 12.0}}, {OPERATEUR, {.operateur = PLUS}}, {REEL, {.reel = 3.0}}, {PAR_FERM, {0}}, {FIN, {0}} };

    typeerreur erreur = 0;
    Node* arbre_syntaxique = Syntaxique(tab, &erreur);

    // Construction manuelle de l'arbre attendu pour cos(3 + 6X) + cos(7) * (12 + 3)
    // Opérateur principal: +
    Node* racine = (Node*)malloc(sizeof(Node));
    racine->jeton.lexem = OPERATEUR; racine->jeton.valeur.operateur = PLUS;

    // Partie gauche: cos(3 + 6X)
    Node* cos1 = (Node*)malloc(sizeof(Node));
    cos1->jeton.lexem = FONCTION; cos1->jeton.valeur.fonction = COS;

    // Contenu de cos1: (3 + 6X)
    Node* addition = (Node*)malloc(sizeof(Node));
    addition->jeton.lexem = OPERATEUR; addition->jeton.valeur.operateur = PLUS;

    Node* val3 = (Node*)malloc(sizeof(Node));
    val3->jeton.lexem = REEL; val3->jeton.valeur.reel = 3.0; val3->pjeton_preced = NULL; val3->pjeton_suiv = NULL;

    // 6X
    Node* multiplication = (Node*)malloc(sizeof(Node));
    multiplication->jeton.lexem = OPERATEUR; multiplication->jeton.valeur.operateur = FOIS;

    Node* val6 = (Node*)malloc(sizeof(Node));
    val6->jeton.lexem = REEL; val6->jeton.valeur.reel = 6.0; val6->pjeton_preced = NULL; val6->pjeton_suiv = NULL;

    Node* varX = (Node*)malloc(sizeof(Node));
    varX->jeton.lexem = VARIABLE; varX->jeton.valeur.variable = 'X'; varX->pjeton_preced = NULL; varX->pjeton_suiv = NULL;

    multiplication->pjeton_preced = val6; multiplication->pjeton_suiv = varX;

    addition->pjeton_preced = val3; addition->pjeton_suiv = multiplication;

    cos1->pjeton_preced = addition; cos1->pjeton_suiv = NULL;

    // Partie droite: cos(7) * (12 + 3)
    Node* multiplication2 = (Node*)malloc(sizeof(Node));
    multiplication2->jeton.lexem = OPERATEUR; multiplication2->jeton.valeur.operateur = FOIS;

    // cos(7)
    Node* cos2 = (Node*)malloc(sizeof(Node));
    cos2->jeton.lexem = FONCTION; cos2->jeton.valeur.fonction = COS;

    Node* val7 = (Node*)malloc(sizeof(Node));
    val7->jeton.lexem = REEL; val7->jeton.valeur.reel = 7.0; val7->pjeton_preced = NULL; val7->pjeton_suiv = NULL;

    cos2->pjeton_preced = val7; cos2->pjeton_suiv = NULL;

    // (12 + 3)
    Node* addition2 = (Node*)malloc(sizeof(Node));
    addition2->jeton.lexem = OPERATEUR; addition2->jeton.valeur.operateur = PLUS;

    Node* val12 = (Node*)malloc(sizeof(Node));
    val12->jeton.lexem = REEL; val12->jeton.valeur.reel = 12.0; val12->pjeton_preced = NULL; val12->pjeton_suiv = NULL;

    Node* val3_2 = (Node*)malloc(sizeof(Node));
    val3_2->jeton.lexem = REEL; val3_2->jeton.valeur.reel = 3.0; val3_2->pjeton_preced = NULL; val3_2->pjeton_suiv = NULL;

    addition2->pjeton_preced = val12; addition2->pjeton_suiv = val3_2;

    multiplication2->pjeton_preced = cos2; multiplication2->pjeton_suiv = addition2;

    // Connexion globale
    racine->pjeton_preced = cos1; racine->pjeton_suiv = multiplication2;

    // Comparer les deux arbres
    bool arbres_identiques = comparer_arbres(arbre_syntaxique, racine);

    if (syntaxeVerbose >= 1) printf("Test %d : cos(3 + 6X) + cos(7) * (12 + 3) : %s. Erreur = %s\n", ++nbTests, (erreur == 0 && arbres_identiques) ? "\033[0;32mOK\033[0m" : "\033[0;31mECHEC\033[0m", get_error_message[erreur]);

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































void test_4_minus_53_erreur() {
    // Expression: 4 - 53
    typejeton tab[] = { {REEL, {.reel = 4.0}}, {OPERATEUR, {.operateur = MOINS}}, {REEL, {.reel = 53.0}}, {FIN, {0}} };
    typeerreur erreur = 0;
    Node* arbre_syntaxique = Syntaxique(tab, &erreur);

    // Comparer les arbres
    bool arbres_identiques = true; // Non applicable mais nécessaire pour l'affichage

    if (syntaxeVerbose >= 1) printf("Test %d : 4 - 53 : %s. Erreur = %s\n", ++nbTests, (erreur == 0) ? "\033[0;32mOK\033[0m" : "\033[0;31mECHEC\033[0m", get_error_message[erreur]);

    if (arbre_syntaxique != NULL) {
        liberer_arbre(arbre_syntaxique);
    }
}

void test_0_plus_0_valide() {
    // Expression: 0 + 0
    typejeton tab[] = { {REEL, {.reel = 0.0}}, {OPERATEUR, {.operateur = PLUS}}, {REEL, {.reel = 0.0}}, {FIN, {0}} };
    typeerreur erreur = 0;
    Node* arbre_syntaxique = Syntaxique(tab, &erreur);

    // Construction manuelle de l'arbre attendu: 0 + 0
    Node* racine = (Node*)malloc(sizeof(Node));
    racine->jeton.lexem = OPERATEUR; racine->jeton.valeur.operateur = PLUS;

    Node* val0_1 = (Node*)malloc(sizeof(Node));
    val0_1->jeton.lexem = REEL; val0_1->jeton.valeur.reel = 0.0; val0_1->pjeton_preced = NULL; val0_1->pjeton_suiv = NULL;

    Node* val0_2 = (Node*)malloc(sizeof(Node));
    val0_2->jeton.lexem = REEL; val0_2->jeton.valeur.reel = 0.0; val0_2->pjeton_preced = NULL; val0_2->pjeton_suiv = NULL;

    racine->pjeton_preced = val0_1; racine->pjeton_suiv = val0_2;

    // Comparer les arbres
    bool arbres_identiques = comparer_arbres(arbre_syntaxique, racine);

    if (syntaxeVerbose >= 1) printf("Test %d : 0 + 0 : %s. Erreur = %s\n", ++nbTests, (erreur == 0 && arbres_identiques) ? "\033[0;32mOK\033[0m" : "\033[0;31mECHEC\033[0m", get_error_message[erreur]);

    // Libération de la mémoire
    liberer_arbre(arbre_syntaxique);
    liberer_arbre(racine);
}

void test_5_3_minus_4_2_valide() {
    // Expression: 5.3 - 4.2
    typejeton tab[] = { {REEL, {.reel = 5.3}}, {OPERATEUR, {.operateur = MOINS}}, {REEL, {.reel = 4.2}}, {FIN, {0}} };
    typeerreur erreur = 0;
    Node* arbre_syntaxique = Syntaxique(tab, &erreur);

    // Construction manuelle de l'arbre attendu: 5.3 - 4.2
    Node* racine = (Node*)malloc(sizeof(Node));
    racine->jeton.lexem = OPERATEUR; racine->jeton.valeur.operateur = MOINS;

    Node* val5_3 = (Node*)malloc(sizeof(Node));
    val5_3->jeton.lexem = REEL; val5_3->jeton.valeur.reel = 5.3; val5_3->pjeton_preced = NULL; val5_3->pjeton_suiv = NULL;

    Node* val4_2 = (Node*)malloc(sizeof(Node));
    val4_2->jeton.lexem = REEL; val4_2->jeton.valeur.reel = 4.2; val4_2->pjeton_preced = NULL; val4_2->pjeton_suiv = NULL;

    racine->pjeton_preced = val5_3; racine->pjeton_suiv = val4_2;

    // Comparer les arbres
    bool arbres_identiques = comparer_arbres(arbre_syntaxique, racine);

    if (syntaxeVerbose >= 1) printf("Test %d : 5.3 - 4.2 : %s. Erreur = %s\n", ++nbTests, (erreur == 0 && arbres_identiques) ? "\033[0;32mOK\033[0m" : "\033[0;31mECHEC\033[0m", get_error_message[erreur]);

    // Libération de la mémoire
    liberer_arbre(arbre_syntaxique);
    liberer_arbre(racine);
}

void test_10_div_33_valide() {
    // Expression: 10 / 33
    typejeton tab[] = { {REEL, {.reel = 10.0}}, {OPERATEUR, {.operateur = DIV}}, {REEL, {.reel = 33.0}}, {FIN, {0}} };
    typeerreur erreur = 0;
    Node* arbre_syntaxique = Syntaxique(tab, &erreur);

    // Construction manuelle de l'arbre attendu: 10 / 33
    Node* racine = (Node*)malloc(sizeof(Node));
    racine->jeton.lexem = OPERATEUR; racine->jeton.valeur.operateur = DIV;

    Node* val10 = (Node*)malloc(sizeof(Node));
    val10->jeton.lexem = REEL; val10->jeton.valeur.reel = 10.0; val10->pjeton_preced = NULL; val10->pjeton_suiv = NULL;

    Node* val33 = (Node*)malloc(sizeof(Node));
    val33->jeton.lexem = REEL; val33->jeton.valeur.reel = 33.0; val33->pjeton_preced = NULL; val33->pjeton_suiv = NULL;

    racine->pjeton_preced = val10; racine->pjeton_suiv = val33;

    // Comparer les arbres
    bool arbres_identiques = comparer_arbres(arbre_syntaxique, racine);

    if (syntaxeVerbose >= 1) printf("Test %d : 10 / 33 : %s. Erreur = %s\n", ++nbTests, (erreur == 0 && arbres_identiques) ? "\033[0;32mOK\033[0m" : "\033[0;31mECHEC\033[0m", get_error_message[erreur]);

    // Libération de la mémoire
    liberer_arbre(arbre_syntaxique);
    liberer_arbre(racine);
}

void test_3_valide() {
    // Expression: 3
    typejeton tab[] = { {REEL, {.reel = 3.0}}, {FIN, {0}} };
    typeerreur erreur = 0;
    Node* arbre_syntaxique = Syntaxique(tab, &erreur);

    // Construction manuelle de l'arbre attendu: 3
    Node* racine = (Node*)malloc(sizeof(Node));
    racine->jeton.lexem = REEL; racine->jeton.valeur.reel = 3.0; racine->pjeton_preced = NULL; racine->pjeton_suiv = NULL;

    // Comparer les arbres
    bool arbres_identiques = comparer_arbres(arbre_syntaxique, racine);

    if (syntaxeVerbose >= 1) printf("Test %d : 3 : %s. Erreur = %s\n", ++nbTests, (erreur == 0 && arbres_identiques) ? "\033[0;32mOK\033[0m" : "\033[0;31mECHEC\033[0m", get_error_message[erreur]);

    // Libération de la mémoire
    liberer_arbre(arbre_syntaxique);
    liberer_arbre(racine);
}

void test_parenthese_ouverte_erreur() {
    // Expression: ()
    typejeton tab[] = { {PAR_OUV, {0}}, {PAR_FERM, {0}}, {FIN, {0}} };
    typeerreur erreur = 0;
    Node* arbre_syntaxique = Syntaxique(tab, &erreur);

    // Dans ce cas, on attend une erreur, pas d'arbre à comparer
    bool arbres_identiques = true; // Non applicable mais nécessaire pour l'affichage

    if (syntaxeVerbose >= 1) printf("Test %d : () : %s. Erreur = %s\n", ++nbTests, (erreur == MEMBRE_VIDE) ? "\033[0;32mOK\033[0m" : "\033[0;31mECHEC\033[0m", get_error_message[erreur]);

    if (arbre_syntaxique != NULL) {
        liberer_arbre(arbre_syntaxique);
    }
}

void test_X_valide() {
    // Expression: X
    typejeton tab[] = { {VARIABLE, {.variable = 'X'}}, {FIN, {0}} };
    typeerreur erreur = 0;
    Node* arbre_syntaxique = Syntaxique(tab, &erreur);

    // Construction manuelle de l'arbre attendu: X
    Node* racine = (Node*)malloc(sizeof(Node));
    racine->jeton.lexem = VARIABLE; racine->jeton.valeur.variable = 'X'; racine->pjeton_preced = NULL; racine->pjeton_suiv = NULL;

    // Comparer les arbres
    bool arbres_identiques = comparer_arbres(arbre_syntaxique, racine);

    if (syntaxeVerbose >= 1) printf("Test %d : X : %s. Erreur = %s\n", ++nbTests, (erreur == 0 && arbres_identiques) ? "\033[0;32mOK\033[0m" : "\033[0;31mECHEC\033[0m", get_error_message[erreur]);

    // Libération de la mémoire
    liberer_arbre(arbre_syntaxique);
    liberer_arbre(racine);
}

void test_div_plus_moins_erreur() {
    // Expression: /+-+6987525+-//*-+
    typejeton tab[] = { {OPERATEUR, {.operateur = DIV}}, {OPERATEUR, {.operateur = PLUS}}, {OPERATEUR, {.operateur = MOINS}}, {REEL, {.reel = 6987525}}, {OPERATEUR, {.operateur = PLUS}}, {OPERATEUR, {.operateur = MOINS}}, {OPERATEUR, {.operateur = FOIS}}, {OPERATEUR, {.operateur = MOINS}}, {FIN, {0}} };
    typeerreur erreur = 0;
    Node* arbre_syntaxique = Syntaxique(tab, &erreur);

    // Dans ce cas, on attend une erreur, pas d'arbre à comparer
    bool arbres_identiques = true; // Non applicable mais nécessaire pour l'affichage

    if (syntaxeVerbose >= 1) printf("Test %d : /+-+6987525+-//*-+ : %s. Erreur = %s\n", ++nbTests, (erreur == MEMBRE_VIDE) ? "\033[0;32mOK\033[0m" : "\033[0;31mECHEC\033[0m", get_error_message[erreur]);

    if (arbre_syntaxique != NULL) {
        liberer_arbre(arbre_syntaxique);
    }
}

void test_1_plus_2_plus_3_valide() {
    // Expression: 1 + 2 + 3
    typejeton tab[] = { {REEL, {.reel = 1.0}}, {OPERATEUR, {.operateur = PLUS}}, {REEL, {.reel = 2.0}}, {OPERATEUR, {.operateur = PLUS}}, {REEL, {.reel = 3.0}}, {FIN, {0}} };
    typeerreur erreur = 0;
    Node* arbre_syntaxique = Syntaxique(tab, &erreur);

    // Construction manuelle de l'arbre attendu: 1 + 2 + 3
    Node* racine = (Node*)malloc(sizeof(Node));
    racine->jeton.lexem = OPERATEUR; racine->jeton.valeur.operateur = PLUS;

    Node* val1 = (Node*)malloc(sizeof(Node));
    val1->jeton.lexem = REEL; val1->jeton.valeur.reel = 1.0; val1->pjeton_preced = NULL; val1->pjeton_suiv = NULL;

    Node* addition = (Node*)malloc(sizeof(Node));
    addition->jeton.lexem = OPERATEUR; addition->jeton.valeur.operateur = PLUS;

    Node* val2 = (Node*)malloc(sizeof(Node));
    val2->jeton.lexem = REEL; val2->jeton.valeur.reel = 2.0; val2->pjeton_preced = NULL; val2->pjeton_suiv = NULL;

    Node* val3 = (Node*)malloc(sizeof(Node));
    val3->jeton.lexem = REEL; val3->jeton.valeur.reel = 3.0; val3->pjeton_preced = NULL; val3->pjeton_suiv = NULL;

    addition->pjeton_preced = val2; addition->pjeton_suiv = val3;
    racine->pjeton_preced = val1; racine->pjeton_suiv = addition;

    // Comparer les arbres
    bool arbres_identiques = comparer_arbres(arbre_syntaxique, racine);

    if (syntaxeVerbose >= 1) printf("Test %d : 1 + 2 + 3 : %s. Erreur = %s\n", ++nbTests, (erreur == 0 && arbres_identiques) ? "\033[0;32mOK\033[0m" : "\033[0;31mECHEC\033[0m", get_error_message[erreur]);

    // Libération de la mémoire
    liberer_arbre(arbre_syntaxique);
    liberer_arbre(racine);
}

void test_cos_erreur() {
    // Expression: COS
    typejeton tab[] = { {FONCTION, {.fonction = COS}}, {FIN, {0}} };
    typeerreur erreur = 0;
    Node* arbre_syntaxique = Syntaxique(tab, &erreur);

    // Construction manuelle de l'arbre attendu: COS
    Node* racine = (Node*)malloc(sizeof(Node));
    racine->jeton.lexem = FONCTION; racine->jeton.valeur.fonction = COS; racine->pjeton_preced = NULL; racine->pjeton_suiv = NULL;

    // Comparer les arbres
    bool arbres_identiques = true;

    if (syntaxeVerbose >= 1) printf("Test %d : cos : %s. Erreur = %s\n", ++nbTests, (erreur == PROBLEME_PARENTHESES_FONCTIONS && arbres_identiques) ? "\033[0;32mOK\033[0m" : "\033[0;31mECHEC\033[0m", get_error_message[erreur]);

    // Libération de la mémoire
    liberer_arbre(arbre_syntaxique);
    liberer_arbre(racine);
}

void test_sin_53_valide() {
    // Expression: SIN(53)
    typejeton tab[] = { {FONCTION, {.fonction = SIN}}, {PAR_OUV, {0}}, {REEL, {.reel = 53.0}}, {PAR_FERM, {0}}, {FIN, {0}} };
    typeerreur erreur = 0;
    Node* arbre_syntaxique = Syntaxique(tab, &erreur);

    // Construction manuelle de l'arbre attendu: SIN(53)
    Node* racine = (Node*)malloc(sizeof(Node));
    racine->jeton.lexem = FONCTION; racine->jeton.valeur.fonction = SIN;

    Node* val53 = (Node*)malloc(sizeof(Node));
    val53->jeton.lexem = REEL; val53->jeton.valeur.reel = 53.0; val53->pjeton_preced = NULL; val53->pjeton_suiv = NULL;

    racine->pjeton_preced = val53; racine->pjeton_suiv = NULL;

    // Comparer les arbres
    bool arbres_identiques = comparer_arbres(arbre_syntaxique, racine);

    if (syntaxeVerbose >= 1) printf("Test %d : SIN(53) : %s. Erreur = %s\n", ++nbTests, (erreur == 0 && arbres_identiques) ? "\033[0;32mOK\033[0m" : "\033[0;31mECHEC\033[0m", get_error_message[erreur]);

    // Libération de la mémoire
    liberer_arbre(arbre_syntaxique);
    liberer_arbre(racine);
}

void test_cos_sin_parenthese_ouverte_erreur() {
    // Expression: COS(SIN())
    typejeton tab[] = { {FONCTION, {.fonction = COS}}, {PAR_OUV, {0}}, {FONCTION, {.fonction = SIN}}, {PAR_OUV, {0}}, {PAR_FERM, {0}}, {PAR_FERM, {0}}, {FIN, {0}} };
    typeerreur erreur = 0;
    Node* arbre_syntaxique = Syntaxique(tab, &erreur);

    // Dans ce cas, on attend une erreur, pas d'arbre à comparer
    bool arbres_identiques = true; // Non applicable mais nécessaire pour l'affichage

    if (syntaxeVerbose >= 1) printf("Test %d : COS(SIN()) : %s. Erreur = %s\n", ++nbTests, (erreur == MEMBRE_VIDE) ? "\033[0;32mOK\033[0m" : "\033[0;31mECHEC\033[0m", get_error_message[erreur]);

    if (arbre_syntaxique != NULL) {
        liberer_arbre(arbre_syntaxique);
    }
}

void test_3_fois_cos_X_plus_X_fois_exp_2X_valide() {
    // Expression: 3 * COS(X) + X * EXP(2 * X)
    typejeton tab[] = { {REEL, {.reel = 3.0}}, {OPERATEUR, {.operateur = FOIS}}, {FONCTION, {.fonction = COS}}, {PAR_OUV, {0}}, {VARIABLE, {.variable = 'X'}}, {PAR_FERM, {0}}, {OPERATEUR, {.operateur = PLUS}}, {VARIABLE, {.variable = 'X'}}, {OPERATEUR, {.operateur = FOIS}}, {FONCTION, {.fonction = EXP}}, {PAR_OUV, {0}}, {REEL, {.reel = 2.0}}, {OPERATEUR, {.operateur = FOIS}}, {VARIABLE, {.variable = 'X'}}, {PAR_FERM, {0}}, {FIN, {0}} };
    typeerreur erreur = 0;
    Node* arbre_syntaxique = Syntaxique(tab, &erreur);

    // Construction manuelle de l'arbre attendu: 3 * COS(X) + X * EXP(2 * X)
    Node* racine = (Node*)malloc(sizeof(Node));
    racine->jeton.lexem = OPERATEUR; racine->jeton.valeur.operateur = PLUS;

    // Partie gauche: 3 * COS(X)
    Node* multiplication1 = (Node*)malloc(sizeof(Node));
    multiplication1->jeton.lexem = OPERATEUR; multiplication1->jeton.valeur.operateur = FOIS;

    Node* val3 = (Node*)malloc(sizeof(Node));
    val3->jeton.lexem = REEL; val3->jeton.valeur.reel = 3.0; val3->pjeton_preced = NULL; val3->pjeton_suiv = NULL;

    Node* cosX = (Node*)malloc(sizeof(Node));
    cosX->jeton.lexem = FONCTION; cosX->jeton.valeur.fonction = COS;

    Node* varX = (Node*)malloc(sizeof(Node));
    varX->jeton.lexem = VARIABLE; varX->jeton.valeur.variable = 'X'; varX->pjeton_preced = NULL; varX->pjeton_suiv = NULL;

    cosX->pjeton_preced = varX; cosX->pjeton_suiv = NULL;
    multiplication1->pjeton_preced = val3; multiplication1->pjeton_suiv = cosX;

    // Partie droite: X * EXP(2 * X)
    Node* multiplication2 = (Node*)malloc(sizeof(Node));
    multiplication2->jeton.lexem = OPERATEUR; multiplication2->jeton.valeur.operateur = FOIS;

    Node* varX2 = (Node*)malloc(sizeof(Node));
    varX2->jeton.lexem = VARIABLE; varX2->jeton.valeur.variable = 'X'; varX2->pjeton_preced = NULL; varX2->pjeton_suiv = NULL;

    Node* expX = (Node*)malloc(sizeof(Node));
    expX->jeton.lexem = FONCTION; expX->jeton.valeur.fonction = EXP;

    Node* multiplication3 = (Node*)malloc(sizeof(Node));
    multiplication3->jeton.lexem = OPERATEUR; multiplication3->jeton.valeur.operateur = FOIS; multiplication3->pjeton_preced = NULL; multiplication3->pjeton_suiv = NULL;

    Node* val2 = (Node*)malloc(sizeof(Node));
    val2->jeton.lexem = REEL; val2->jeton.valeur.reel = 2.0; val2->pjeton_preced = NULL; val2->pjeton_suiv = NULL;

    Node* varX3 = (Node*)malloc(sizeof(Node));
    varX3->jeton.lexem = VARIABLE; varX3->jeton.valeur.variable = 'X'; varX3->pjeton_preced = NULL; varX3->pjeton_suiv = NULL;

    multiplication2->pjeton_preced = varX2; multiplication2->pjeton_suiv = expX;
    expX->pjeton_preced = multiplication3; expX->pjeton_suiv = NULL;

    multiplication3->pjeton_preced = val2; multiplication3->pjeton_suiv = varX3;

    // Connexion globale
    racine->pjeton_preced = multiplication1; racine->pjeton_suiv = multiplication2;

    // Comparer les arbres
    bool arbres_identiques = comparer_arbres(arbre_syntaxique, racine);

    if (syntaxeVerbose >= 1) printf("Test %d : 3 * COS(X) + X * EXP(2 * X) : %s. Erreur = %s\n", ++nbTests, (erreur == 0 && arbres_identiques) ? "\033[0;32mOK\033[0m" : "\033[0;31mECHEC\033[0m", get_error_message[erreur]);

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

void test_cos_56_plus_sin_tan_12_valide() {
    // Expression: COS(56) + SIN(TAN(12))
    typejeton tab[] = { {FONCTION, {.fonction = COS}}, {PAR_OUV, {0}}, {REEL, {.reel = 56.0}}, {PAR_FERM, {0}}, {OPERATEUR, {.operateur = PLUS}}, {FONCTION, {.fonction = SIN}}, {PAR_OUV, {0}}, {FONCTION, {.fonction = TAN}}, {PAR_OUV, {0}}, {REEL, {.reel = 12.0}}, {PAR_FERM, {0}}, {PAR_FERM, {0}}, {FIN, {0}} };
    typeerreur erreur = 0;
    Node* arbre_syntaxique = Syntaxique(tab, &erreur);

    // Construction manuelle de l'arbre attendu: COS(56) + SIN(TAN(12))
    Node* racine = (Node*)malloc(sizeof(Node));
    racine->jeton.lexem = OPERATEUR; racine->jeton.valeur.operateur = PLUS;

    // Partie gauche: COS(56)
    Node* cos56 = (Node*)malloc(sizeof(Node));
    cos56->jeton.lexem = FONCTION; cos56 ->jeton.valeur.fonction = COS;

    Node* val56 = (Node*)malloc(sizeof(Node));
    val56->jeton.lexem = REEL; val56->jeton.valeur.reel = 56.0; val56->pjeton_preced = NULL; val56->pjeton_suiv = NULL;

    cos56->pjeton_preced = val56; cos56->pjeton_suiv = NULL;

    // Partie droite: SIN(TAN(12))
    Node* sinTan12 = (Node*)malloc(sizeof(Node));
    sinTan12->jeton.lexem = FONCTION; sinTan12->jeton.valeur.fonction = SIN;

    Node* tan12 = (Node*)malloc(sizeof(Node));
    tan12->jeton.lexem = FONCTION; tan12->jeton.valeur.fonction = TAN;

    Node* val12 = (Node*)malloc(sizeof(Node));
    val12->jeton.lexem = REEL; val12->jeton.valeur.reel = 12.0; val12->pjeton_preced = NULL; val12->pjeton_suiv = NULL;

    tan12->pjeton_preced = val12; tan12->pjeton_suiv = NULL;
    sinTan12->pjeton_preced = tan12; sinTan12->pjeton_suiv = NULL;

    // Connexion globale
    racine->pjeton_preced = cos56; racine->pjeton_suiv = sinTan12;

    // Comparer les arbres
    bool arbres_identiques = comparer_arbres(arbre_syntaxique, racine);

    if (syntaxeVerbose >= 1) printf("Test %d : COS(56) + SIN(TAN(12)) : %s. Erreur = %s\n", ++nbTests, (erreur == 0 && arbres_identiques) ? "\033[0;32mOK\033[0m" : "\033[0;31mECHEC\033[0m", get_error_message[erreur]);

    // Libération de la mémoire
    liberer_arbre(arbre_syntaxique);
    liberer_arbre(racine);
}

void test_expression_longue_valide() {
    // Expression: 321659876844689961112
    typejeton tab[] = { {REEL, {.reel = 321659876844689961112.0}}, {FIN, {0}} };
    typeerreur erreur = 0;
    Node* arbre_syntaxique = Syntaxique(tab, &erreur);

    // Construction manuelle de l'arbre attendu: 321659876844689961112
    Node* racine = (Node*)malloc(sizeof(Node));
    racine->jeton.lexem = REEL; racine->jeton.valeur.reel = 321659876844689961112.0; racine->pjeton_preced = NULL; racine->pjeton_suiv = NULL;

    // Comparer les arbres
    bool arbres_identiques = comparer_arbres(arbre_syntaxique, racine);

    if (syntaxeVerbose >= 1) printf("Test %d : 321659876844689961112 : %s. Erreur = %s\n", ++nbTests, (erreur == 0 && arbres_identiques) ? "\033[0;32mOK\033[0m" : "\033[0;31mECHEC\033[0m", get_error_message[erreur]);

    // Libération de la mémoire
    liberer_arbre(arbre_syntaxique);
    liberer_arbre(racine);
}

void test_cos_valeur_tres_grande_valide() {
    // Expression: cos(69875658954712356157364)
    typejeton tab[] = { {FONCTION, {.fonction = COS}}, {PAR_OUV, {0}}, {REEL, {.reel = 69875658954712356157364.0}}, {PAR_FERM, {0}}, {FIN, {0}} };
    typeerreur erreur = 0;
    Node* arbre_syntaxique = Syntaxique(tab, &erreur);

    // Construction manuelle de l'arbre attendu: cos(69875658954712356157364)
    Node* racine = (Node*)malloc(sizeof(Node));
    racine->jeton.lexem = FONCTION; racine->jeton.valeur.fonction = COS;

    Node* valTresGrand = (Node*)malloc(sizeof(Node));
    valTresGrand->jeton.lexem = REEL; valTresGrand->jeton.valeur.reel = 69875658954712356157364.0; valTresGrand->pjeton_preced = NULL; valTresGrand->pjeton_suiv = NULL;

    racine->pjeton_preced = valTresGrand; racine->pjeton_suiv = NULL;

    // Comparer les arbres
    bool arbres_identiques = comparer_arbres(arbre_syntaxique, racine);

    if (syntaxeVerbose >= 1) printf("Test %d : cos(69875658954712356157364) : %s. Erreur = %s\n", ++nbTests, (erreur == 0 && arbres_identiques) ? "\033[0;32mOK\033[0m" : "\033[0;31mECHEC\033[0m", get_error_message[erreur]);

    // Libération de la mémoire
    liberer_arbre(arbre_syntaxique);
    liberer_arbre(racine);
}

void test_valeur_precise_valide() {
    // Expression: 5.31654645611654897965
    typejeton tab[] = { {REEL, {.reel = 5.31654645611654897965}}, {FIN, {0}} };
    typeerreur erreur = 0;
    Node* arbre_syntaxique = Syntaxique(tab, &erreur);

    // Construction manuelle de l'arbre attendu: 5.31654645611654897965
    Node* racine = (Node*)malloc(sizeof(Node));
    racine->jeton.lexem = REEL; racine->jeton.valeur.reel = 5.31654645611654897965; racine->pjeton_preced = NULL; racine->pjeton_suiv = NULL;

    // Comparer les arbres
    bool arbres_identiques = comparer_arbres(arbre_syntaxique, racine);

    if (syntaxeVerbose >= 1) printf("Test %d : 5.31654645611654897965 : %s. Erreur = %s\n", ++nbTests, (erreur == 0 && arbres_identiques) ? "\033[0;32mOK\033[0m" : "\033[0;31mECHEC\033[0m", get_error_message[erreur]);

    // Libération de la mémoire
    liberer_arbre(arbre_syntaxique);
    liberer_arbre(racine);
}

void test_sin_X_erreur() {
    // Expression: SIN( X )
    typejeton tab[] = { {FONCTION, {.fonction = SIN}}, {PAR_OUV, {0}}, {VARIABLE, {.variable = 'X'}}, {PAR_FERM, {0}}, {FIN, {0}} };
    typeerreur erreur = 0;
    Node* arbre_syntaxique = Syntaxique(tab, &erreur);

    // Construction manuelle de l'arbre attendu: SIN(X)
    Node* racine = (Node*)malloc(sizeof(Node));
    racine->jeton.lexem = FONCTION; racine->jeton.valeur.fonction = SIN;

    Node* varX = (Node*)malloc(sizeof(Node));
    varX->jeton.lexem = VARIABLE; varX->jeton.valeur.variable = 'X'; varX->pjeton_preced = NULL; varX->pjeton_suiv = NULL;

    racine->pjeton_preced = varX; racine->pjeton_suiv = NULL;

    // Comparer les arbres
    bool arbres_identiques = comparer_arbres(arbre_syntaxique, racine);

    if (syntaxeVerbose >= 1) printf("Test %d : SIN(X) : %s. Erreur = %s\n", ++nbTests, (erreur == 0 && arbres_identiques) ? "\033[0;32mOK\033[0m" : "\033[0;31mECHEC\033[0m", get_error_message[erreur]);

    // Libération de la mémoire
    liberer_arbre(arbre_syntaxique);
    liberer_arbre(racine);
}

void test_XEXP_erreur() {
    // Expression: XEXP
    typejeton tab[] = { {VARIABLE, {.variable = 'X'}}, {OPERATEUR, {.operateur = FOIS}}, {FONCTION, {.fonction = EXP}}, {FIN, {0}} };
    typeerreur erreur = 0;
    Node* arbre_syntaxique = Syntaxique(tab, &erreur);

    // Construction manuelle de l'arbre attendu: X * EXP
    Node* racine = (Node*)malloc(sizeof(Node));
    racine->jeton.lexem = OPERATEUR; racine->jeton.valeur.operateur = FOIS;

    Node* varX = (Node*)malloc(sizeof(Node));
    varX->jeton.lexem = VARIABLE; varX->jeton.valeur.variable = 'X'; varX->pjeton_preced = NULL; varX->pjeton_suiv = NULL;

    Node* expX = (Node*)malloc(sizeof(Node));
    expX->jeton.lexem = FONCTION; expX->jeton.valeur.fonction = EXP; expX->pjeton_preced = NULL; expX->pjeton_suiv = NULL;

    racine->pjeton_preced = varX; racine->pjeton_suiv = expX;

    // Comparer les arbres
    bool arbres_identiques = true;

    if (syntaxeVerbose >= 1) printf("Test %d : X * EXP : %s. Erreur = %s\n", ++nbTests, (erreur == PROBLEME_PARENTHESES_FONCTIONS && arbres_identiques) ? "\033[0;32mOK\033[0m" : "\033[0;31mECHEC\033[0m", get_error_message[erreur]);

    // Libération de la mémoire
    liberer_arbre(arbre_syntaxique);
    liberer_arbre(racine);
}

void test_val_neg_valide() {
    // Expression: VAL_NEG
    typejeton tab[] = { {VARIABLE, {.variable = 'V'}}, {OPERATEUR, {.operateur = MOINS}}, {FIN, {0}} };
    typeerreur erreur = 0;
    Node* arbre_syntaxique = Syntaxique(tab, &erreur);

    // Construction manuelle de l'arbre attendu: -V
    Node* racine = (Node*)malloc(sizeof(Node));
    racine->jeton.lexem = OPERATEUR; racine->jeton.valeur.operateur = MOINS;

    Node* varV = (Node*)malloc(sizeof(Node));
    varV->jeton.lexem = VARIABLE; varV->jeton.valeur.variable = 'V'; varV->pjeton_preced = NULL; varV->pjeton_suiv = NULL;

    racine->pjeton_preced = varV; racine->pjeton_suiv = NULL;

    // Comparer les arbres
    bool arbres_identiques = true;

    if (syntaxeVerbose >= 1) printf("Test %d : -V : %s. Erreur = %s\n", ++nbTests, (erreur == MEMBRE_VIDE && arbres_identiques) ? "\033[0;32mOK\033[0m" : "\033[0;31mECHEC\033[0m", get_error_message[erreur]);

    // Libération de la mémoire
    liberer_arbre(arbre_syntaxique);
    liberer_arbre(racine);
}

void test_sinc() {
    // Expression: SINC(3/0)
    typejeton tab[] = { {FONCTION, {.fonction = SINC}}, {PAR_OUV, {0}}, {REEL, {.reel = 3.0}}, {OPERATEUR, {.operateur = DIV}}, {REEL, {.reel = 0.0}}, {PAR_FERM, {0}}, {FIN, {0}} };
    typeerreur erreur = 0;
    Node* arbre_syntaxique = Syntaxique(tab, &erreur);

    // Construction manuelle de l'arbre attendu: SINC(3/0)


    Node* racine = (Node*)malloc(sizeof(Node));
    racine->jeton.lexem = FONCTION; racine->jeton.valeur.fonction = 1;

    Node* par_ouv = (Node*)malloc(sizeof(Node));
    par_ouv->jeton.lexem = PAR_OUV; par_ouv->pjeton_preced = NULL; par_ouv->pjeton_suiv = NULL;

    Node* division = (Node*)malloc(sizeof(Node));
    division->jeton.lexem = OPERATEUR; division->jeton.valeur.operateur = DIV;

    Node* val3 = (Node*)malloc(sizeof(Node));
    val3->jeton.lexem = REEL; val3->jeton.valeur.reel = 3.0; val3->pjeton_preced = NULL; val3->pjeton_suiv = NULL;

    Node* val0 = (Node*)malloc(sizeof(Node));
    val0->jeton.lexem = REEL; val0->jeton.valeur.reel = 0.0; val0->pjeton_preced = NULL; val0->pjeton_suiv = NULL;

    // Connecter les nœuds
    racine->pjeton_preced = par_ouv; // SINC(
    par_ouv->pjeton_preced = division; // SINC(3/0
    division->pjeton_preced = val3; // 3
    division->pjeton_suiv = val0; // 0

    // Comparer les arbres
    bool arbres_identiques = comparer_arbres(arbre_syntaxique, racine);

    if (syntaxeVerbose >= 1) printf("Test %d : SINC(3/0) : %s. Erreur = %s\n", ++nbTests, (erreur == 0) ? "\033[0;32mOK\033[0m" : "\033[0;31mECHEC\033[0m", get_error_message[erreur]);

    if (arbre_syntaxique != NULL) {
        liberer_arbre(arbre_syntaxique);
    }
}

void test_plus_3_valide() {
    // Expression: +3
    typejeton tab[] = { {OPERATEUR, {.operateur = PLUS}}, {REEL, {.reel = 3.0}}, {FIN, {0}} };
    typeerreur erreur = 0;
    Node* arbre_syntaxique = Syntaxique(tab, &erreur);

    // Construction manuelle de l'arbre attendu: +3
    Node* racine = (Node*)malloc(sizeof(Node));
    racine->jeton.lexem = OPERATEUR; racine->jeton.valeur.operateur = PLUS;

    Node* val3 = (Node*)malloc(sizeof(Node));
    val3->jeton.lexem = REEL; val3->jeton.valeur.reel = 3.0; val3->pjeton_preced = NULL; val3->pjeton_suiv = NULL;

    racine->pjeton_preced = NULL; racine->pjeton_suiv = val3;

    // Comparer les arbres
    bool arbres_identiques = true;

    if (syntaxeVerbose >= 1) printf("Test %d : +3 : %s. Erreur = %s\n", ++nbTests, (erreur == MEMBRE_VIDE && arbres_identiques) ? "\033[0;32mOK\033[0m" : "\033[0;31mECHEC\033[0m", get_error_message[erreur]);

    // Libération de la mémoire
    liberer_arbre(arbre_syntaxique);
    liberer_arbre(racine);
}

void test_sin_neg_x_valide() {
    // Expression: sin(-x)
    typejeton tab[] = { {FONCTION, {.fonction = SIN}}, {PAR_OUV, {0}}, {OPERATEUR, {.operateur = MOINS}}, {VARIABLE, {.variable = 'X'}}, {PAR_FERM, {0}}, {FIN, {0}} };
    typeerreur erreur = 0;
    Node* arbre_syntaxique = Syntaxique(tab, &erreur);

    // Construction manuelle de l'arbre attendu: sin(-X)
    Node* racine = (Node*)malloc(sizeof(Node));
    racine->jeton.lexem = FONCTION; racine->jeton.valeur.fonction = SIN;

    Node* negX = (Node*)malloc(sizeof(Node));
    negX->jeton.lexem = OPERATEUR; negX->jeton.valeur.operateur = MOINS;

    Node* varX = (Node*)malloc(sizeof(Node));
    varX->jeton.lexem = VARIABLE; varX->jeton.valeur.variable = 'X'; varX->pjeton_preced = NULL; varX->pjeton_suiv = NULL;

    negX->pjeton_preced = varX;
    negX->pjeton_suiv = NULL;
    racine->pjeton_preced = negX;
    racine->pjeton_suiv = NULL;

    // Comparer les arbres
    bool arbres_identiques = true;

    if (syntaxeVerbose >= 1) printf("Test %d : sin(-X) : %s. Erreur = %s\n", ++nbTests, (erreur == MEMBRE_VIDE && arbres_identiques) ? "\033[0;32mOK\033[0m" : "\033[0;31mECHEC\033[0m", get_error_message[erreur]);

    // Libération de la mémoire
    liberer_arbre(arbre_syntaxique);
    liberer_arbre(racine);
}