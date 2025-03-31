#ifndef STRUCTURES_H
#define STRUCTURES_H

//énumération des différents types de lexems existants
typedef enum
{
    REEL, OPERATEUR, FONCTION, ERREUR, FIN, PAR_OUV, PAR_FERM, VARIABLE, BAR_OUV, BAR_FERM, ABSOLU
} typelexem;

//énumération des diff types d'opérateurs existants
typedef enum
{
    PLUS, MOINS, FOIS, DIV, PUIS
} typeoperateur;

//énumération des diff types de fonctions existantes
typedef enum
{
    ABS, SIN, SQRT, LOG, COS, TAN, EXP, ENTIER, VAL_NEG, SINC
} typefonction;

//énumération des diff types de valeurs existantes
typedef union
{
    float reel;
    typefonction fonction;
    typeoperateur operateur;
    char variable;
    //typeerreur erreur;
} typevaleur;

//énumération des diff types de jetons existants
typedef struct
{
    typelexem lexem;
    typevaleur valeur;
} typejeton;

//déclaration de l'arbre
typedef struct Node
{
    typejeton jeton;
    struct Node *pjeton_preced;
    struct Node *pjeton_suiv;
} Node;

typedef Node *Arbre;

typedef enum {
//ERREUR ANALYSE LEXICAL
FONCTION_INCONNUE = 101 ,
NOMBRE_INVALIDE = 102,
CARACTERE_INCONNUE = 103,
VARIABLE_INCONNUE = 104,

//ERREUR FONCTION 30X
RACINE_NEGATIVE=301, 
LOG_NEGATIF=302,
LOG_ZERO=303, 
TAN_PI_SUR_2=304, 
SINC_DIVISIION_PAR_ZERO=305, // SIN(0)/0 

// ERREUR OPERATEUR 31X
DIVISION_PAR_ZERO=311, // 0/0
ERREUR_ZERO_PUIS_ZERO=312, // 0^0
ERREUR_ZERRO_PUIS_NEGATIVE=313, //0^(-x) 

} typeerreur;

#endif
