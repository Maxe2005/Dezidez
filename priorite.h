#ifndef PRIORITE_H
#define PRIORITE_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <float.h>
#include "Structures.h"



Node* arbrevide();
Node* operateur( typejeton *tab, int debut, int fin, typeerreur *erreur);
bool parenthese(int debut, int fin, typejeton *tab);
int minIndice(typejeton *tab,  int debut, int fin, typeerreur *erreur);
int calculTaille(typejeton *tab);
Node* Syntaxique(typejeton *tab, typeerreur *erreur);


#endif