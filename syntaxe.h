#ifndef PRIORITE_H
#define PRIORITE_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <float.h>
#include "Structures.h"
#include "affiche_syntaxe.h"

static const char opPriorite[] = {
    [PLUS] = 0, 
    [MOINS] = 0, 
    [FOIS] = 1, 
    [DIV] = 1, 
    [PUIS] = 2
};

Node* arbrevide();
Node* operateur( typejeton *tab, int debut, int fin, typeerreur *erreur);
bool parenthese(int debut, int fin, typejeton *tab);
int minIndice(typejeton *tab,  int debut, int fin, typeerreur *erreur);
int calculTaille(typejeton *tab);
Node* Syntaxique(typejeton *tab, typeerreur *erreur);


#endif