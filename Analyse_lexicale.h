#include "Structures.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
 * Renvoie 1 si l'element recherché est dans le tableau de chaine de caractère sinon renvoie 0
 * @param tab : tableau de chaine de caractère
 * @param size : La taille du tableau
  * @param element : Element recherché
 */
int IsInTab3(char *tab[], int size, char *element);
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
typejeton TokenFonction (char *Element, int* erreur);

/**
 * Renvoie le bon Token correrspond dans au string rentré en paramètre dans le cas d'opérateur
 * @param Element : chaine de caractère 
 * @param erreur : pointeur vers une variable qui contiendra le numéro de l'erreur
 */
typejeton TokenOperateur (char *Element);

/**
 * Renvoie le bon Token correrspond dans au string rentré en paramètre dans le cas de variable
 * @param Element : chaine de caractère 
 */
typejeton TokenVariable (char *Element, int* erreur,int  Dimension);
/**
 * Renvoie le bon Token correrspond dans au string rentré en paramètre dans le cas d'un reel'
 * @param Element : chaine de caractère 
 * @param erreur : pointeur vers une variable qui contiendra le numéro de l'erreur
 * @param Dimension : 0 si on est en 2 dimension et 1 si on est en 3 dimentions
 */
typejeton TokenReel (char *Element , int* erreur);

/**
 * Permet de transformer une expression en un tableau de Token
 * @param Str : Expression mathématique sans espace et avec les multiplication implicite
 * @param SizeExpression : la taille de l'expression 
 * @param Strdecoupee : tableau de sortie qui sera rempli de Token 
 * @param erreur : pointeur vers une variable qui contiendra le numéro de l'erreur
 * @param Dimension : 0 si on est en 2 dimension et 1 si on est en 3 dimentions
 */ 
void DecompositionToken(char *str, int SizeExpression, typejeton TabToken[TAILLE_MAX],int* erreur,int  Dimension);


/**
 * retourne 1 si plus d'un point dans le str nombre, 0 sinon
 * @param nombre : str du nombre
 */
int PlusieursVirgules (char *nombre);

/**
 * Permet de transformer une chaine de caractère en un tableau de Token.
 * @param TabToken : Tableau de typejeton qui sera le tableau de sorti
 * @param Expression : chaine de caractère qui est une expression mathématique (supporte les espace et des multiplication implicite entre réel et variable)
 * @param erreur : pointeur vers une variable qui contiendra le numéro de l'erreur
 * @param Dimension : 0 si on est en 2 dimension et 1 si on est en 3 dimentions
 */
void Analyse_Lexicale (typejeton TabToken[TAILLE_MAX],char Expression[TAILLE_MAX],int* erreur,int  Dimension);

//*erreur = 101;
