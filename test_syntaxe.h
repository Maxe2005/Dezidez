#ifndef TEST_SYNTAXE_H
#define TEST_SYNTAXE_H

#include "Structures.h"

// Prototype de la fonction à tester
void syntaxe(typejeton *tabjetons, typeerreur *erreur);

// Prototypes des fonctions de test
void test_valide1();
void test_valide2();
void test_valide3();

void test_erreur_operateurs_a_la_suite1();
void test_erreur_operateurs_a_la_suite2();
void test_erreur_manque_operateur();

void tester_syntaxe();

#endif // TEST_SYNTAXE_H