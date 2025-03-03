#ifndef GRAPHIQUE_H
#define GRAPHIQUE_H

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>
#include "gestionGraphique.h"

#define NB_FONTS 3
extern TTF_Font* fonts[NB_FONTS];

/**
 * Initialise les différentes polices de caractère avec les tailles correspondantes
 * @param font Le tableau de police vide à remplire
 */
void init_font (TTF_Font* font[NB_FONTS]);

#endif
