#ifndef SYNTAXE_H
#define SYNTAXE_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <float.h>
#include "Structures.h"
#include "affiche_syntaxe.h"

/**
 * Table de priorité des opérateurs
 * Plus la valeur est élevée, plus l'opérateur est prioritaire
 * PLUS et MOINS: priorité 0 (les plus faibles)
 * FOIS et DIV: priorité 1
 * PUIS (puissance): priorité 2 (la plus élevée)
 */
static const char opPriorite[] = {
    [PLUS] = 0, 
    [MOINS] = 0, 
    [FOIS] = 1, 
    [DIV] = 1, 
    [PUIS] = 2
};

/**
 * Crée et initialise un nœud vide avec le jeton FIN
 * @return Un pointeur vers un nouveau nœud initialisé comme nœud vide
 */
Node* createEmptyNode();

/**
 * Construit récursivement l'arbre d'expression à partir d'une séquence de jetons
 * en utilisant l'algorithme d'analyse par priorité d'opérateurs
 * 
 * @param tab Tableau de jetons à analyser
 * @param debut Indice du premier jeton à considérer
 * @param fin Indice du dernier jeton à considérer
 * @param erreur Pointeur pour stocker le code d'erreur éventuel
 * @return Un pointeur vers la racine de l'arbre d'expression construit
 */
Node* buildExpressionTree(typejeton *tab, int debut, int fin, typeerreur *erreur);

/**
 * Vérifie si les parenthèses sont correctement équilibrées
 * dans une plage donnée de jetons
 * 
 * @param debut Indice du premier jeton à considérer
 * @param fin Indice du dernier jeton à considérer
 * @param tab Tableau de jetons à analyser
 * @return true si les parenthèses sont équilibrées, false sinon
 */
bool checkParenthesesBalance(int debut, int fin, typejeton *tab);

/**
 * Recherche l'opérateur de plus faible priorité à la racine de l'expression
 * (en tenant compte des parenthèses qui augmentent la priorité)
 * 
 * @param tab Tableau de jetons à analyser
 * @param debut Indice du premier jeton à considérer
 * @param fin Indice du dernier jeton à considérer
 * @param erreur Pointeur pour stocker le code d'erreur éventuel
 * @return L'indice de l'opérateur trouvé ou -1 si aucun opérateur n'est trouvé
 */
int findLowestPriorityOperator(typejeton *tab, int debut, int fin, typeerreur *erreur);

/**
 * Calcule la longueur effective de l'expression en tokens
 * jusqu'au jeton FIN qui marque la fin de l'expression
 * 
 * @param tab Tableau de jetons à analyser
 * @return La position du dernier jeton avant FIN, ou -1 si FIN n'est pas trouvé
 */
int findExpressionLength(typejeton *tab);

/**
 * Point d'entrée principal pour l'analyse syntaxique
 * Construit l'arbre syntaxique complet à partir d'une séquence de jetons
 * 
 * @param tab Tableau de jetons à analyser
 * @param erreur Pointeur pour stocker le code d'erreur éventuel
 * @return Un pointeur vers la racine de l'arbre syntaxique construit
 */
Node* buildSyntaxTree(typejeton *tab, typeerreur *erreur);

/**
 * Libère la mémoire allouée pour un arbre syntaxique
 * @param racine Pointeur vers la racine de l'arbre à libérer
 */
void liberer_arbre(Node* racine);


#endif