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
    Syntaxique(tab, &erreur);
    printf("Test valide 1 : %s\n", erreur == 0 ? "OK" : "ÉCHEC");
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
    Syntaxique(tab, &erreur);
    printf("Test valide 2 : %s : erreur %d\n", erreur ==0 ? "OK" : "ÉCHEC", erreur);
}

void test_valide3() {
    typejeton tab[] = {
        {VARIABLE, {.reel = 0}},
        {OPERATEUR, {.operateur = PUIS}},
        {REEL, {.reel = 2.0}},
        {FIN, {0}}
    };
    typeerreur erreur = 0;
    Syntaxique(tab, &erreur);
    printf("Test valide 3 : %s\n", erreur == 0 ? "OK" : "ÉCHEC");
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
    printf("Test erreur operateurs a la suite 1 : %s : erreur %d\n", erreur == MEMBRE_VIDE ? "OK" : "ECHEC", erreur);
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
    printf("Test erreur opérateurs à la suite 2 : %s\n", erreur == MEMBRE_VIDE ? "OK" : "ÉCHEC");
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
    printf("Test erreur manque opérateur : %s. Erreur=%d\n", erreur == MEMBRE_VIDE ? "OK" : "ÉCHEC", erreur );
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
    printf("Test erreur manque opérateur : %s. Erreur=%d\n", erreur == PROBLEMES_NOMBRE_PARENTHESES ? "OK" : "ÉCHEC", erreur );
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
}
