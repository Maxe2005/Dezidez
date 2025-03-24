#ifndef LEXICALE_H
#define LEXICALE_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <float.h>
#include "Structures.h"

Node operateur(typejeton *tab, int debut, int fin);
bool parenthese(int tailletab, typejeton *tab);



#endif