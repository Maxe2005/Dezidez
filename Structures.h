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
    PLUS = 0, MOINS = 1, FOIS = 10, DIV = 11, PUIS = 20
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
    //typeerreur erreur;
    char variable;
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

//typedef Node *Arbre;

typedef enum {
    //ERREUR FONCTION 20X
    PROBLEME_PARENTHESES_FONCTIONS = 201, // cos(.... , cos....., cos.....)
    
    // ERREUR OPERATEUR 21X
    OPERATEURS_A_LA_SUITE = 211, // ++,**,/*
    MANQUE_OPERATEUR = 212, // 3+

    // ERREUR PARENTHESES 22X
    PARENTHESE_FERMEE_1_ER_JETON = 221 ,// ......
    PROBLEME_INTERIEUR_PARENTHESE = 222, // ()
    PROBLEMES_NOMBRE_PARENTHESES = 223, // ((.....), ((()))))))))))
    PARENTHESE_PAS_FERMEE = 224, // (......., .......)

    // ERREUR REEL 23X
    PROBLEME_APRES_REEL = 231, // 2,3,4

    //ERREUR TAILLE 24X
    ABSENCE_FIN = 241, //debut>fin, absence token END
    } typeerreur;

static const char* lexem_string[] = {"REEL", "OPERATEUR", "FONCTION", "ERREUR", "FIN", "PAR_OUV", "PAR_FERM", "VARIABLE", "BAR_OUV", "BAR_FERM", "ABSOLU"};
static const char* operateur_string[] = {"PLUS", "MOINS", "FOIS", "DIV", "PUIS"};
static const char* fonction_string[] = {"ABS", "SIN", "SQRT", "LOG", "COS", "TAN", "EXP", "ENTIER", "VAL_NEG", "SINC"};

#endif 