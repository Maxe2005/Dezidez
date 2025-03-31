#ifndef EVALUATEUR
#define EVALUATEUR
#define _USE_MATH_DEFINES
#include <math.h>
#include "Structures.h"
#include <stdio.h>

/**
 *@brief Fonction qui évalue la valeur de demandé 
 * @param racine arbre
 * @param valeur_x variable x
 * @param valeur_y variable y
 * @param code_erreur Pointeur de la variable erreur globale
*/
float evaluateur(Node *racine,float valeur_x, float valeur_y,int *code_erreur);


/**
 * @brief Calcule la fonction avec sa valeur
 * @param fonction La fonction à étudier
 * @param variable La variable à fournir à la fonction donnée
 * @param code_erreur Pointeur de la variable erreur globale
 * @return return le résultat de la fonction
 */
float calculer_fonction(typefonction fonction, float variable,int *code_erreur);



#endif