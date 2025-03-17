#include "Structures.h"
#include <stdio.h>
#include <string.h>


/**
 * Update le tableua Token en le remplissant de jetons de la str (pour l'instant les multiplié doivent être marqué par )
 * @param Token : tableau de la taille de la str
 * @param Expression : la fonction sous forme de str 
  * @param SizeExpression : La taille de l'expression
 */
int AnalyseLexicale (typejeton Token[], char *Expression, int SizeExpression);

/**
 * Renvoie de typelexem d'une expression passé en paramètre
 * @param Expression : Chaine de caractère
 * @param SizeExpression : La taille de l'expression
 */
typelexem TypeString (char *Expression, int SizeExpression);

/**
 * Renvoie 1 si l'element recherché est dans le tableau sinon renvoie 0
 * @param tab : tableaude chaine de caractère
 * @param size : La taille du tableau
  * @param element : Element recherché
 */
 int IsInTab(char *tab[], int size, char element)
 /**
 * Prend l'expression en paramètre et rajoute tout les * implicite qui ne sont pas marque
 * @param Expression : chaine de caractère qui contient l'expression sans les espaces
 * @param SizeExpression : La taille de l'expression
 */
char MultiplicationImplicite (char *Expression,int SizeExpression);
/**
 * Retire tout les espaces de l'expression
 * @param Expression : chaine de caractère qui contient l'expression 
 * @param SizeExpression : La taille de l'expression
 */
char ExpressionSansLesEspaces (char *Expression,int SizeExpression);


/**
 * Découpe la chaine de caractère
 * @param str : chaine de caractère qui contient l'expression 
 * @param SizeExpression : La taille de l'expression
 */
int CutStr (char *str, int SizeExpression, char *Strdecoupee);