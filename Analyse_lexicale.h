#include "Structures.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TailleMax 100
/*à faire*/
/**
 * Retire tout les espaces de l'expression
 * @param Expression : chaine de caractère qui contient l'expression 
 * @param SizeExpression : La taille de l'expression
 * @param buffer : chaine de caractère temporaire ayant les même caractèristique que Expression
 */
void ExpressionSansLesEspaces (char *Expression,int SizeExpression,char *NewExpression);
/**
 * Renvoie 1 si l'element recherché est dans le tableau sinon renvoie 0
 * @param tab : tableaude chaine de caractère
 * @param size : La taille du tableau
  * @param element : Element recherché
 */
int IsInTab(char *tab, int size, char element);
/**
 * Renvoie 1 si l'element recherché est dans le tableau de chaine de caractère sinon renvoie 0
 * @param tab : tableau de chaine de caractère
 * @param size : La taille du tableau
  * @param element : Element recherché
 */
int IsInTab2(char *tab[], int size, char *element);
 /**
 * Prend l'expression en paramètre et rajoute tout les * implicite qui ne sont pas marque
 * @param Expression : chaine de caractère qui contient l'expression sans les espaces
 * @param SizeExpression : La taille de l'expression
 * @param buffer : chaine de caractère temporaire ayant les même caractèristique que Expression
 */
void MultiplicationImplicite (char *Expression,int SizeExpression,char *buffer);

/**
 * Permet de comparer 2 chaine de caractère
 * @param element1 : chaine de caractère
 * @param element2 : chaine de caractère
 */
int ComparaisonString(char *element1,char *element2);

/**
 * Renvoie le bon Token correrspond dans au string rentré en paramètre dans le cas de fonction
 * @param Element : chaine de caractère 
 */
typejeton TokenFonction (char *Element);

/**
 * Renvoie le bon Token correrspond dans au string rentré en paramètre dans le cas d'opérateur
 * @param Element : chaine de caractère 
 */
typejeton TokenOperateur (char *Element);

/**
 * Renvoie le bon Token correrspond dans au string rentré en paramètre dans le cas de variable
 * @param Element : chaine de caractère 
 */
typejeton TokenVariable (char *Element);
/**
 * Renvoie le bon Token correrspond dans au string rentré en paramètre dans le cas d'un reel POSITIF'
 * @param Element : chaine de caractère 
 */
typejeton TokenReelPositif (char *Element);
