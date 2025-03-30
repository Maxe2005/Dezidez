#ifndef EVALUATEUR
#define EVALUATEUR

#include <math.h>
#include "Structures.h"
#include <stdio.h>

#define _USE_MATH_DEFINES
#define M_PI 3.14159265358979323846

Node creation_arbre(typejeton racine, Node* precedent, Node* suivant);

/**
 *@brief Fonction qui évalue la valeur de demandé 
 * @param racine arbre
 * @param valeur_x variable x
 * @param valeur_y variable y
 * @param code_erreur Pointeur de la variable erreur globale
*/
float evaluateur(Node *racine,float valeur_x, float valeur_y,int *code_erreur);

/**
 * @brief remplacer x et y par les variables demandées
 * @param racine arbre
 * @param valeur_x variable x
 * @param valeur_y variable y
 * @param code_erreur Pointeur de la variable erreur globale 
 * @return retourne l'arbre avec x et y rem^placé par les vrariables demandées
 */
void remplacer_variable(Node *racine,float valeur_x, float valeur_y, int *code_erreur); //Je la fait plus tard, Esteban


/**
 * @brief Calcule la fonction avec sa valeur
 * @param fonction La fonction à étudier
 * @param variable La variable à fournir à la fonction donnée
 * @param code_erreur Pointeur de la variable erreur globale
 * @return return le résultat de la fonction
 */
float calculer_fonction(typefonction fonction, float variable,int *code_erreur);

/**
 * @brief Calcule l'expression entière donner par l'arbre
 * @param racine La racine où on commence le décodage de l'arbre
 * @param code_erreur Pointeur de la variable erreur globale
 * @return return le résultat totale de l'expression
 */
float decodage_arbre(Node *racine, int *code_erreur);



#endif
