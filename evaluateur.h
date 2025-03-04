#ifndef EVALUATEUR
#define EVALUATEUR
#include <math.h>
#include "Structures.h"

float calculer_fonction(typefonction fonction, float variable);
/**
 * @brief Calcule la fonction avec sa valeur
 *
 * @param fonction La fonction à étudier
 * @param variable La variable à fournir à la fonction donnée
 * @return return le résultat de la fonction
 */

float decodage_arbre(Node *racine);
/**
 * @brief Calcule l'expression entière donner par l'arbre
 *
 * @param racine La racine d'où on commence le décodage de l'arbre
 * @return return le résultat totale de l'expression
 */
#endif