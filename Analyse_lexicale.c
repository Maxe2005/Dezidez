#include "Analyse_lexicale.h"



//cherche si on a un y (2 dim)
//Renvoie le caractère unique sous forme de structure
//Renvoie la structure de la fonction correspondante
//Trouve une fonction dans la chaine de caractère
//Detecte si on a affaire à une fonction


//(pour caractère unique)
//fonction :  caractère --> Type lexem 


void ExpressionSansLesEspaces (char *Expression,int SizeExpression,char *buffer){
    int i;
    int j = 0;
    for (i = 0; i < SizeExpression; i++){
        if (Expression[i]!=' '){
            buffer[j] = Expression[i];
            j++;
        }
    }
    buffer[j]='\0';
    strcpy(Expression, buffer);
    memset(buffer, '\0', sizeof(buffer));
}


// Fonction qui vérifie si un caractère est dans un tableau de chaînes
int IsInTab(char *tab, int size, char element) {
    for (int i = 0; i < size; i++) {
        if (tab[i] == element) {
            return 1;
        }
    }
    return 0;
}

int ComparaisonString(char *element1,char *element2){
    int size1 =strlen(element1);
    int size2 =strlen(element2);
    int res = 1;
    int i;
    if (size1 != size2){
        res = 0;
    }
    for ( i = 0; i < size1; i++){
        if (element1[i]!= element2[i]){
            res = 0;
        }
        
    }
    return res;
}


int IsInTab2(char *tab[], int size, char *element) {
    for (int i = 0; i < size; i++) {
        if (ComparaisonString(tab[i],element)) {
            return 1;
        }
    }
    return 0;
}




void MultiplicationImplicite (char *Expression,int SizeExpression,char *buffer){
    char chiffre[] = "0123456789";
    char variable[] = "xy";
    int i;
    int j =0;
    //on parcour Expression
    for ( i = 0; i < SizeExpression-1; i++){
        if ((IsInTab(chiffre,10,Expression[i]))&& (IsInTab(variable,2,Expression[i+1]))){//si Expression[i] est un chiffre et si Expression[i+1] est une varible
            
            buffer[j]=Expression[i];
            j++; 
            buffer[j]='*';  
            j++;
        }else {  
            if ((IsInTab(chiffre,10,Expression[i+1]))&& (IsInTab(variable,2,Expression[i]))){//si Expression[i+1] est un chiffre et si Expression[i] est une varible
                
                buffer[j]=Expression[i];
                j++; 
                buffer[j]='*';  
                j++;
            }else{
                buffer[j]=Expression[i];
                j++; }
        }        
    }
    //on remplace l'expression par le buffer
    strcpy(Expression, buffer);
    // on vide le buffer
    memset(buffer, '\0', sizeof(buffer));
}


typejeton TokenFonction (char *Element){
    char *fonction[] = {"abs","sin","cos","sqrt","log","tan","exp","entier","sinc"};
    typejeton fonct;
    if (IsInTab2(fonction,strlen(fonction),Element)){// on regarde si c'est une fonction
        fonct.lexem = FONCTION;
        if (ComparaisonString(Element,"abs")){
            fonct.valeur.fonction = ABS;
        }
        else if (ComparaisonString(Element,"sin")){
            fonct.valeur.fonction = SIN;
        }
        else if (ComparaisonString(Element,"cos")){
            fonct.valeur.fonction = COS;
        }
        else if (ComparaisonString(Element,"sqrt")){
            fonct.valeur.fonction = SQRT;
        }
        else if (ComparaisonString(Element,"log")){
            fonct.valeur.fonction = LOG;
        }
        else if (ComparaisonString(Element,"tan")){
            fonct.valeur.fonction = TAN;
        }
        else if (ComparaisonString(Element,"exp")){
            fonct.valeur.fonction = EXP;
        }
        else if (ComparaisonString(Element,"entier")){
            fonct.valeur.fonction = ENTIER;
        }
        else if (ComparaisonString(Element,"sinc")){
            fonct.valeur.fonction = SINC;
        }
        else {
            //Erreur Unkown fonction
        }
    }  
    return fonct;     
}

typejeton TokenOperateur (char *Element){
    char *fonction[] = {"+","-","*","/","**"};
    typejeton fonct;
    if (IsInTab2(fonction,strlen(fonction),Element)){// on regarde si c'est une fonction
        fonct.lexem = OPERATEUR;
        if (ComparaisonString(Element,"+")){
            fonct.valeur.operateur = PLUS;
        }
        else if (ComparaisonString(Element,"-")){
            fonct.valeur.operateur = MOINS;
        }
        else if (ComparaisonString(Element,"*")){
            fonct.valeur.operateur = FOIS;
        }
        else if (ComparaisonString(Element,"**")){
            fonct.valeur.operateur = PUIS;
        }
        else if (ComparaisonString(Element,"/")){
            fonct.valeur.operateur = DIV;
        }
        else {
            //Erreur Unkown fonction
        }
    }  
    return fonct;     
}

