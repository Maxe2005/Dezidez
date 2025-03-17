#include "Analyse_lexicale.h"
#include <stdio.h>


//cherche si on a un y (2 dim)
//Renvoie le caractère unique sous forme de structure
//Renvoie la structure de la fonction correspondante
//Trouve une fonction dans la chaine de caractère
//Detecte si on a affaire à une fonction


//(pour caractère unique)
//fonction :  caractère --> Type lexem 

char *op[5] = {"+","-","*","/","**"};
char *chiffre[10] = {"0","1","2","3","4","5","6","7","8","9"};
char *fonction[10] = {"abs", "sin", "sqrt", "log", "cos", "tan", "exp", "entier", "val_neg", "sinc"};
char *alphabet[26] = {"a","b","c","d","e","f","g","h","i","j","k","l","m","n","o","p","q","r","s","t","u","v","w","x","y","z"};
char *parenthese[2] = {"(",")"}


typelexem TypeString (char *Expression, int SizeExpression){
    typelexem Res =ERREUR;
    char *op[5] = {"+","-","*","/","**"};
    char *chiffre[10] = {"0","1","2","3","4","5","6","7","8","9"};
    char *fonction[10] = {"abs", "sin", "sqrt", "log", "cos", "tan", "exp", "entier", "val_neg", "sinc"};
    char *alphabet[26] = {"a","b","c","d","e","f","g","h","i","j","k","l","m","n","o","p","q","r","s","t","u","v","w","x","y","z"};
    int i;

    for (i = 0; i < 5; i++){ //parcour par indice du tableau pour opération
        if (Expression==op[y]){
            Res =OPERATEUR;
        }
    }
    for (i = 0; i < 10; i++){ //parcour par indice du tableau pour chiffre/réel
        if (Expression[0]==chiffre[y]){
            Res=REEL;
        }
    }
    //Variable variable
    if (Expression=="x" || Expression=="y" ){
        Res=VARIABLE;  
    }
    //parcour par indice du tableau pour parenthère
    if (Expression=="("  ){
        Res=PAR_OUV;
    }else if (Expression==")")
    {
        Res=PAR_FERM;
    }
    for (i = 0; i < NbOpe; i++){//parcour par indice du tableau pour fonction
        if (Expression==fonction[i]){
            Res=FONCTION;
        }
    }
        
    return Res;
}





int AnalyseLexicale (typejeton Token[], char *Expression, int SizeExpression){
    char caractere;
    int i;
    for ( i = 0; i < SizeExpression ; i++){ //on parcour par indice l'expression 
        caractere = Expression[i];
        printf("%c \n",caractere);
        //si caractère unique on remplie le tableau de Token
        //sinon on lance la reche de fonction
    }    
    return 0;
}

/*
int CutStr (char str, int SizeExpression){
    char *op[5] = {"+","-","*","/","**"};
    char *chiffre[10] = {"0","1","2","3","4","5","6","7","8","9"};
    char *fonction[10] = {"abs", "sin", "sqrt", "log", "cos", "tan", "exp", "entier", "val_neg", "sinc"};
    char *alphabet[26] = {"a","b","c","d","e","f","g","h","i","j","k","l","m","n","o","p","q","r","s","t","u","v","w","x","y","z"};
    char *parenthese[2] = {"(",")"}
    int i;
    int j;

    for ( i = 0; i < SizeExpression ; i++){ // on regarde pour les réel
        for ( j = 0; j < 5 ; j++)
    }

}*/