#ifndef PRIORITE_H
#define PRIORITE_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <float.h>
#include "Structures.h"

#define TAILLE_MAX 100
#define MAX_DEPTH TAILLE_MAX

void afficher_liste_jetons(typejeton *liste, int debut, int fin);
Node* arbrevide();
Node* operateur( typejeton *tab, int debut, int fin, typeerreur *erreur);
bool parenthese(int debut, int fin, typejeton *tab);
int minIndice(typejeton *tab,  int debut, int fin, typeerreur *erreur);
int calculTaille(typejeton *tab);
Node* Syntaxique(typejeton *tab, typeerreur *erreur);

// Affichage
void jeton_to_string(typejeton jeton, char* buffer, size_t size);
void afficher_arbre_rec(Node* racine, char* prefixe, int est_dernier);
void afficher_arbre(Node* racine);
void afficher_arbre2(Node* racine, int niveau);
const char* choisir_couleur(typelexem lexem);
void afficher_rec_couleur(Node* noeud, char* prefixe, int est_dernier);
void afficher_arbre_couleur(Node* racine);
bool comparer_arbres(Node* arbre1, Node* arbre2);
void liberer_arbre(Node* racine);



#endif