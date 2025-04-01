#ifndef PRIORITE_H
#define PRIORITE_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <float.h>
#include "Structures.h"

#define TAILLE_MAX 100

Node arbrevide();
Node operateur( typejeton *tab, int debut, int fin, typeerreur *erreur);
bool parenthese(int debut, int fin, typejeton *tab);
int minIndice(typejeton *tab,  int debut, int fin, typeerreur *erreur);
int calculTaille(typejeton *tab);
Node Syntaxique(typejeton *tab, typeerreur *erreur);

// Affichage
void affiche_liste_jeton(typejeton *tab, int debut, int fin);
void jeton_to_string(typejeton jeton, char* buffer, size_t size);
void afficher_arbre_rec(Node* racine, char* prefixe, int est_dernier);
void afficher_arbre(Node* racine);

#endif