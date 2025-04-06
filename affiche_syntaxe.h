#ifndef AFFICHE_SYNTAXE_H
#define AFFICHE_SYNTAXE_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <float.h>
#include "Structures.h"

static const int syntaxeVerbose = 1;
/*
|---------------------------|-----------|
| Verbose                   |   level   |
|---------------------------|-----------|
| None                      |     0     |
| print tests results       |     1     |
| print tests with trees    |     2     |
| All                       |     10    |
|---------------------------|-----------|
*/

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

static const char* get_error_message[] = {
    [0] = "AUCUNE ERREUR",
    [PROBLEME_PARENTHESES_FONCTIONS] = "PROBLEME_PARENTHESES_FONCTIONS",
    [MEMBRE_VIDE] = "MEMBRE_VIDE",
    [PARENTHESE_FERMEE_1_ER_JETON] = "PARENTHESE_FERMEE_1_ER_JETON",
    [PROBLEMES_NOMBRE_PARENTHESES] = "PROBLEMES_NOMBRE_PARENTHESES",
    [PROBLEME_APRES_REEL_OU_VARIABLE] = "PROBLEME_APRES_REEL_OU_VARIABLE"
};

// Affichage
void afficher_liste_jetons(typejeton *liste, int debut, int fin);
void jeton_to_string(typejeton jeton, char* buffer, size_t size);
void afficher_arbre_rec(Node* racine, char* prefixe, int est_dernier);
void afficher_arbre(Node* racine);
void afficher_arbre2(Node* racine, int niveau);
const char* choisir_couleur(typelexem lexem);
void afficher_rec_couleur(Node* noeud, char* prefixe, int est_dernier);
void afficher_arbre_couleur(Node* racine);
bool comparer_arbres(Node* arbre1, Node* arbre2);
void liberer_arbre(Node* racine);
void afficher_arbre_complet(Node* racine);

#endif