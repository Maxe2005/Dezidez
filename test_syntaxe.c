// Fonctions de test
void test_valide1() {
    // 5 + 3
    typejeton tab[] = {
        {LEXEM_NOMBRE, {.entier = 5}},
        {LEXEM_OPERATEUR, {.operateur = '+'}},
        {LEXEM_NOMBRE, {.entier = 3}},
        {LEXEM_FIN, {0}}
    };
    typeerreur erreur;
    syntaxe(tab, &erreur);
    printf("Test valide 1 : %s\n", erreur == ERREUR_NONE ? "OK" : "ÉCHEC");
}

void test_valide2() {
    // ( 7 * 2 )
    typejeton tab[] = {
        {LEXEM_PARENTHESE_OUVRANTE, {0}},
        {LEXEM_NOMBRE, {.entier = 7}},
        {LEXEM_OPERATEUR, {.operateur = '*'}},
        {LEXEM_NOMBRE, {.entier = 2}},
        {LEXEM_PARENTHESE_FERMANTE, {0}},
        {LEXEM_FIN, {0}}
    };
    typeerreur erreur;
    syntaxe(tab, &erreur);
    printf("Test valide 2 : %s\n", erreur == ERREUR_NONE ? "OK" : "ÉCHEC");
}

void test_valide3() {
    // x = 10
    typejeton tab[] = {
        {LEXEM_VARIABLE, {.nom = "x"}},
        {LEXEM_AFFECTATION, {.operateur = '='}},
        {LEXEM_NOMBRE, {.entier = 10}},
        {LEXEM_FIN, {0}}
    };
    typeerreur erreur;
    syntaxe(tab, &erreur);
    printf("Test valide 3 : %s\n", erreur == ERREUR_NONE ? "OK" : "ÉCHEC");
}

void test_erreur_operateur_debut() {
    // + 5
    typejeton tab[] = {
        {LEXEM_OPERATEUR, {.operateur = '+'}},
        {LEXEM_NOMBRE, {.entier = 5}},
        {LEXEM_FIN, {0}}
    };
    typeerreur erreur;
    syntaxe(tab, &erreur);
    printf("Test erreur opérateur début : %s\n", erreur == ERREUR_OPERATEUR_DEBUT ? "OK" : "ÉCHEC");
}

void test_erreur_parenthese_non_fermee() {
    // ( 4 + 2
    typejeton tab[] = {
        {LEXEM_PARENTHESE_OUVRANTE, {0}},
        {LEXEM_NOMBRE, {.entier = 4}},
        {LEXEM_OPERATEUR, {.operateur = '+'}},
        {LEXEM_NOMBRE, {.entier = 2}},
        {LEXEM_FIN, {0}}
    };
    typeerreur erreur;
    syntaxe(tab, &erreur);
    printf("Test erreur parenthèse non fermée : %s\n", erreur == ERREUR_PARENTHESE_NON_FERMEE ? "OK" : "ÉCHEC");
}

void test_erreur_double_operateur() {
    // 8 * + 3
    typejeton tab[] = {
        {LEXEM_NOMBRE, {.entier = 8}},
        {LEXEM_OPERATEUR, {.operateur = '+'}},
        {LEXEM_OPERATEUR, {.operateur = '*'}},
        {LEXEM_NOMBRE, {.entier = 3}},
        {LEXEM_FIN, {0}}
    };
    typeerreur erreur;
    syntaxe(tab, &erreur);
    printf("Test erreur double opérateur : %s\n", erreur == ERREUR_DOUBLE_OPERATEUR ? "OK" : "ÉCHEC");
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
    typeerreur erreur;
    syntaxe(tab, &erreur);
    printf("Test erreur opérateurs à la suite 1 : %s\n", erreur == OPERATEURS_A_LA_SUITE ? "OK" : "ÉCHEC");
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
    typeerreur erreur;
    syntaxe(tab, &erreur);
    printf("Test erreur opérateurs à la suite 2 : %s\n", erreur == OPERATEURS_A_LA_SUITE ? "OK" : "ÉCHEC");
}

void test_erreur_manque_operateur() {
    // 5 +
    typejeton tab[] = {
        {REEL, {.reel = 5.0}},
        {OPERATEUR, {.operateur = PLUS}},
        {FIN, {0}}
    };
    typeerreur erreur;
    syntaxe(tab, &erreur);
    printf("Test erreur manque opérateur : %s\n", erreur == MANQUE_OPERATEUR ? "OK" : "ÉCHEC");
}

// Fonction principale qui exécute tous les tests
void tester_syntaxe() {
    test_valide1();
    test_valide2();
    test_valide3();
    test_erreur_operateur_debut();
    test_erreur_parenthese_non_fermee();
    test_erreur_double_operateur();
    test_erreur_operateurs_a_la_suite1();
    test_erreur_operateurs_a_la_suite2();
    test_erreur_manque_operateur();
}
