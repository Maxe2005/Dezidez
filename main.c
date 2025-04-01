#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include "syntaxe.h"
#include "priorite.h"
#include "test_syntaxe.h"
#include "Structures.h"

int main(int argc, char *argv[]){
    tester_syntaxe();
    printf("fin\n");
    return 0;
}

// TODO !!
// 1
// PENSER A ALLOC + FREE 
// `-> fix erreur segmentation

// 2
// modif les tests pour mettre a jour les "erreurs" parce qu'il faut changer le type d'erreur

// 3
// ajouter des tests