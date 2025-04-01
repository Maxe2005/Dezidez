#ifndef TEST_SYNTAXE_H
#define TEST_SYNTAXE_H

#include "priorite.h"

//#include <stdio.h>
//#include <stdlib.h>

#include "Structures.h"
// Prototypes des fonctions de test
void test_valide1();
void test_valide2();
void test_valide3();
void test_erreur_operateurs_a_la_suite1();
void test_erreur_operateurs_a_la_suite2();
void test_erreur_manque_operateur();
void test_erreur_manque_parenthese();
void test_probleme_parentheses_fonctions1();
void test_probleme_parentheses_fonctions2();
void test_probleme_apres_reel();
void test_membre_vide();
void test_parenthese_fermee_1er_jeton();
void test_expression_valide();

void tester_syntaxe();

#endif // TEST_SYNTAXE_H