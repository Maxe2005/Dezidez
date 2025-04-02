#ifndef AFFICHE_SYNTAXE_H
#define AFFICHE_SYNTAXE_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <float.h>
#include "Structures.h"

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