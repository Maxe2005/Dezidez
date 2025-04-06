#include "test_syntaxe.h"

// Fonctions de test
void test_valide1() {
    typejeton tab[] = {
        {REEL, {.reel = 5.0}},
        {OPERATEUR, {.operateur = PLUS}},
        {REEL, {.reel = 3.0}},
        {FIN, {0}}
    };
    typeerreur erreur = 0;
    Node* a = Syntaxique(tab, &erreur);
    if (syntaxeVerbose >= 1) printf("Test valide 1 : %s\n", erreur == 0 ? "OK" : "ÉCHEC");
    //afficher_arbre(a);
    //afficher_arbre2(a, 0);
    if (syntaxeVerbose >= 2) afficher_arbre_couleur(a);
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
    Node* a = Syntaxique(tab, &erreur);
    if (syntaxeVerbose >= 1) printf("Test valide 2 : %s : erreur %d\n", erreur ==0 ? "OK" : "ECHEC", erreur);
    //printf("fg a = %d\n", a.pjeton_preced->jeton.lexem);
    //afficher_arbre(&a);
    //afficher_arbre2(a, 0);
    if (syntaxeVerbose >= 2) afficher_arbre_couleur(a);
}

void test_valide3() {
    typejeton tab[] = {
        {VARIABLE, {.variable = 'x'}},
        {OPERATEUR, {.operateur = PUIS}},
        {REEL, {.reel = 2.0}},
        {FIN, {0}}
    };
    typeerreur erreur = 0;
    Node* a = Syntaxique(tab, &erreur);
    if (syntaxeVerbose >= 1) printf("Test valide 3 : %s\n", erreur == 0 ? "OK" : "ÉCHEC");
    //afficher_arbre2(a, 0);
    if (syntaxeVerbose >= 2) afficher_arbre_couleur(a);
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
    Syntaxique(tab, &erreur);
    if (syntaxeVerbose >= 1) printf("Test erreur operateurs a la suite 1 : %s : erreur %d\n", erreur == MEMBRE_VIDE ? "OK" : "ECHEC", erreur);
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
    Syntaxique(tab, &erreur);
    if (syntaxeVerbose >= 1) printf("Test erreur opérateurs à la suite 2 : %s\n", erreur == MEMBRE_VIDE ? "OK" : "ÉCHEC");
}

void test_erreur_manque_operateur() {
    // 5 +
    typejeton tab[] = {
        {REEL, {.reel = 5.0}},
        {OPERATEUR, {.operateur = PLUS}},
        {FIN, {0}}
    };
    typeerreur erreur = 0;
    Syntaxique(tab, &erreur);
    if (syntaxeVerbose >= 1) printf("Test erreur manque opérateur : %s. Erreur=%d\n", erreur == MEMBRE_VIDE ? "OK" : "ÉCHEC", erreur );
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
    Syntaxique(tab, &erreur);
    if (syntaxeVerbose >= 1) printf("Test erreur manque opérateur : %s. Erreur=%d\n", erreur == PROBLEMES_NOMBRE_PARENTHESES ? "OK" : "ÉCHEC", erreur );
}

// Fonction principale qui exécute tous les tests
void tester_syntaxe() {
    test_valide1();
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
    test_parenthese_fermee_1er_jeton();
    test_expression_valide();
    test_cos_plus_sin();
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
    Node* a = Syntaxique(tab, &erreur);
    if (syntaxeVerbose >= 1) printf("Test expression valide : %s. Erreur=%d\n", erreur == 0 ? "OK" : "ÉCHEC", erreur);
    //afficher_arbre2(a, 0);
    if (syntaxeVerbose >= 2) afficher_arbre_couleur(a);

}

void test_parenthese_fermee_1er_jeton() {
    typejeton tab[] = {
        {PAR_FERM, {0}},
        {FIN, {0}}
    };
    typeerreur erreur = 0;
    Syntaxique(tab, &erreur);
    if (syntaxeVerbose >= 1) printf("Test parenthèse fermée 1er jeton : %s. Erreur=%d\n", erreur == PROBLEMES_NOMBRE_PARENTHESES ? "OK" : "ÉCHEC", erreur);
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
    Syntaxique(tab, &erreur);
    if (syntaxeVerbose >= 1) printf("Test membre vide : %s. Erreur=%d\n", erreur == MEMBRE_VIDE ? "OK" : "ÉCHEC", erreur);
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
    Syntaxique(tab, &erreur);
    if (syntaxeVerbose >= 1) printf("Test problème après réel : %s. Erreur=%d\n", erreur == PROBLEME_APRES_REEL ? "OK" : "ÉCHEC", erreur);
}

void test_probleme_parentheses_fonctions1() {
    typejeton tab[] = {
        {FONCTION, {.fonction = COS}},
        {REEL, {.reel = 5.0}},
        {FIN, {0}}
    };
    
    typeerreur erreur = 0;
    Syntaxique(tab, &erreur);
    if (syntaxeVerbose >= 1) printf("Test problème parenthèses fonctions (cos5) : %s. Erreur=%d\n", erreur == PROBLEME_PARENTHESES_FONCTIONS ? "OK" : "ÉCHEC", erreur);
}

void test_probleme_parentheses_fonctions2() {

    typejeton tab[] = {
            {FONCTION, {.fonction = COS}},
            {PAR_OUV, {0}},
            {REEL, {.reel = 5.0}},
            {FIN, {0}}
        };
    typeerreur erreur = 0;
    Syntaxique(tab, &erreur);
    if (syntaxeVerbose >= 1) printf("Test problème parenthèses fonctions (cos(5) non fermé) : %s. Erreur=%d\n", erreur == PROBLEMES_NOMBRE_PARENTHESES ? "OK" : "ÉCHEC", erreur);
}

void test_cos_plus_sin() {
    // Tableau de jetons pour l'expression "cos(3) + sin(2)"
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
    if (syntaxeVerbose >= 1) printf("Expression: cos(3) + sin(2)\n");
    if (syntaxeVerbose >= 2) afficher_liste_jetons(tab, 0, 9);

    // Création de l'arbre par la fonction Syntaxique
    typeerreur erreur = 0;
    Node* arbre_syntaxique = Syntaxique(tab, &erreur);

    // Création manuelle de l'arbre attendu pour "cos(3) + sin(2)"

    // Niveau 1: Opérateur + principal
    Node* racine = (Node*)malloc(sizeof(Node));
    racine->jeton.lexem = OPERATEUR;
    racine->jeton.valeur.operateur = FOIS;

    // Branche gauche: cos(3)
    Node* cos = (Node*)malloc(sizeof(Node));
    cos->jeton.lexem = FONCTION;
    cos->jeton.valeur.fonction = SIN;

    Node* val3 = (Node*)malloc(sizeof(Node));
    val3->jeton.lexem = REEL;
    val3->jeton.valeur.reel = 3.0;
    val3->pjeton_preced = NULL;
    val3->pjeton_suiv = NULL;

    // Branche droite: sin(2)
    Node* sin = (Node*)malloc(sizeof(Node));
    sin->jeton.lexem = FONCTION;
    sin->jeton.valeur.fonction = COS;

    Node* val2 = (Node*)malloc(sizeof(Node));
    val2->jeton.lexem = REEL;
    val2->jeton.valeur.reel = 2.0;
    val2->pjeton_preced = NULL;
    val2->pjeton_suiv = NULL;

    // Connexions de l'arbre
    racine->pjeton_preced = cos;
    racine->pjeton_suiv = sin;

    cos->pjeton_preced = val3;
    cos->pjeton_suiv = NULL;

    sin->pjeton_preced = val2;
    sin->pjeton_suiv = NULL;

    // Comparaison des deux arbres
    bool arbres_identiques = comparer_arbres(arbre_syntaxique, racine);

    if (syntaxeVerbose >= 1) printf("Test cos(3) + sin(2) : %s. Erreur=%d\n",
           (erreur == 0 && arbres_identiques) ? "OK" : "ÉCHEC", erreur);

    // Afficher arbre
    //afficher_arbre2(arbre_syntaxique,0);
    if (syntaxeVerbose >= 2) {
        printf("affichange arbre racine :\n");
        afficher_arbre_couleur(racine);
        printf("affichange arbre syntaxique :\n");
        afficher_arbre_couleur(arbre_syntaxique);
        printf("comparaison =%d :\n", arbres_identiques);
        
    }

    // Libération de la mémoire
    liberer_arbre(arbre_syntaxique);
    liberer_arbre(racine);
}