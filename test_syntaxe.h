#ifndef TEST_SYNTAXE_H
#define TEST_SYNTAXE_H

//#include <stdio.h>
//#include <stdlib.h>

#include "Structures.h"
#include "affiche_syntaxe.h"
#include "syntaxe.h"

void tester_syntaxe();

// Prototypes des fonctions de test
// void test_valide1();
// void test_valide2();
// void test_valide3();
// void test_erreur_operateurs_a_la_suite1();
// void test_erreur_operateurs_a_la_suite2();
// void test_erreur_manque_operateur();
// void test_erreur_manque_parenthese();
// void test_probleme_parentheses_fonctions1();
// void test_probleme_parentheses_fonctions2();
// void test_PROBLEME_APRES_REEL_OU_VARIABLE();
// void test_membre_vide();
// void test_parenthese_fermee_1er_jeton();
// void test_expression_valide();
// void test_cos_plus_sin();
void test_5_plus_3_valide();
void test_parenthese_ouverte_7_fois_2_valide();
void test_x_puis_2_valide();
void test_5_plus_etoile_3_erreur();
void test_8_fois_plus_2_erreur();
void test_5_plus_erreur();
void test_parenthese_ouverte_5_plus_erreur();
void test_cos_parenthese_ouverte_5_erreur();
void test_sin_3_fois_cos_2_valide();
void test_cos_3_plus_6X_fois_cos_7_fois_parenthese_12_plus_3_valide();

void test_4_minus_53_valide();
void test_0_plus_0_valide();
void test_5_3_minus_4_2_valide();
void test_10_div_33_valide();
void test_3_valide();
void test_parenthese_erreur();
void test_X_valide();
void test_div_plus_moins_erreur();
void test_1_plus_2_plus_3_valide();
void test_cos_erreur();
void test_sin_53_valide();
void test_cos_sin_parenthese_ouverte_erreur();
void test_3_fois_cos_X_plus_X_fois_exp_2X_valide();
void test_cos_56_plus_sin_tan_12_valide();
void test_expression_longue_valide();
void test_cos_valeur_tres_grande_valide();
void test_valeur_precise_valide();
void test_sin_X_valide();
void test_cos_3_plus_sin_X_erreur();
void test_XEXP_erreur();
void test_val_neg_valide();
void test_sinc_3_div_0_valide();
void test_plus_3_erreur();
void test_sin_neg_x_erreur();
void test_parenthese_ouverte_7_fois_2_sans_fin_erreur();
void test_parenthese_ferme_5_plus_2_erreur();
void test_5_plus_parenthese_ferme_2_erreur();

#endif // TEST_SYNTAXE_H