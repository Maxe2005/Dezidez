#include "Structures.h"
#include <stdio.h>
#include <string.h>

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
 * Prend l'expression en paramètre et rajoute tout les * implicite qui ne sont pas marque
 * @param Expression : chaine de caractère qui contient l'expression sans les espaces
 * @param SizeExpression : La taille de l'expression
 * @param buffer : chaine de caractère temporaire ayant les même caractèristique que Expression
 */
void MultiplicationImplicite (char *Expression,int SizeExpression,char *buffer);