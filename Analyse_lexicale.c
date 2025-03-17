#include "Analyse_lexicale.h"



//cherche si on a un y (2 dim)
//Renvoie le caractère unique sous forme de structure
//Renvoie la structure de la fonction correspondante
//Trouve une fonction dans la chaine de caractère
//Detecte si on a affaire à une fonction


//(pour caractère unique)
//fonction :  caractère --> Type lexem 
/*
char *op[5] = {"+","-","*","/","**"};
char *chiffre[10] = {"0","1","2","3","4","5","6","7","8","9"};
char *fonction[10] = {"abs", "sin", "sqrt", "log", "cos", "tan", "exp", "entier", "val_neg", "sinc"};
char *alphabet[26] = {"a","b","c","d","e","f","g","h","i","j","k","l","m","n","o","p","q","r","s","t","u","v","w","x","y","z"};
char *parenthese[2] = {"(",")"};
*/

char ExpressionSansLesEspaces (char *Expression,int SizeExpression){
    static char NewExpression[100];
    int i;
    int j;
    for (i = 0; i < SizeExpression; i++){
        if (&Expression[i]!=" "){
            NewExpression[j++] = Expression[i];
            
        }
    }
    NewExpression[j] = '\o';
    return NewExpression;
}


// Fonction qui vérifie si un caractère est dans un tableau de chaînes
int IsInTab(char *tab[], int size, char element) {
    for (int i = 0; i < size; i++) {
        if (tab[i][0] == element) {  // Comparaison avec le premier caractère de chaque chaîne
            return 1;  // Trouvé
        }
    }
    return 0;  // Pas trouvé
}

char *MultiplicationImplicite(char *Expression, int SizeExpression) {
    // Définition des ensembles sous forme de tableaux de chaînes
    char chiffre[] = {"0", "1", "2", "3", "4", "5", "6", "7", "8", "9"};
    char variable[] = {"x", "y"};
    char fonction[] = {"a", "s", "s", "l", "c", "t", "e", "e", "v", "s"};
    
    // Tableau statique pour stocker la nouvelle expression
    static char res[200];  // Taille fixe suffisamment grande
    res[0] = '\0';  // Initialisation à une chaîne vide

    int ope = 0;
    int pos = 0;  // Index pour écrire dans res

    for (int i = 0; i < SizeExpression - 1; i++) {
        res[pos++] = Expression[i];  // Ajout du caractère actuel

        // Vérification des cas de multiplication implicite
        if ((IsInTab(chiffre, 10, Expression[i]) && IsInTab(variable, 2, Expression[i+1])) ||
            (IsInTab(chiffre, 10, Expression[i+1]) && IsInTab(variable, 2, Expression[i])) ||
            (IsInTab(variable, 2, Expression[i+1]) && Expression[i] == ')') ||
            (Expression[i+1] == '(' && IsInTab(variable, 2, Expression[i])) ||
            (IsInTab(chiffre, 10, Expression[i+1]) && Expression[i] == ')') ||
            (Expression[i+1] == '(' && IsInTab(chiffre, 10, Expression[i]))) {
            
            res[pos++] = '*';  // Ajout du signe de multiplication
            ope = 1;
        }
        
        if (ope == 1) {
            ope = 0;
        }
    }

    // Ajouter le dernier caractère de l'expression
    res[pos++] = Expression[SizeExpression - 1];
    res[pos] = '\0';  // Terminaison de la chaîne

    return res;
}


/*typelexem TypeString (char *Expression, int SizeExpression){
    typelexem Res =ERREUR;
    char *op[5] = {"+","-","*","/","**"};
    char *chiffre[10] = {"0","1","2","3","4","5","6","7","8","9"};
    char *fonction[10] = {"abs", "sin", "sqrt", "log", "cos", "tan", "exp", "entier", "val_neg", "sinc"};
    char alphabet[26] = {"a","b","c","d","e","f","g","h","i","j","k","l","m","n","o","p","q","r","s","t","u","v","w","x","y","z"};
    int y;

    for (y = 0; y < 5; y++){ //parcour par indice du tableau pour opération
        if (Expression==op[y]){
            Res =OPERATEUR;
        }
    }
    for (y = 0; y < 10; y++){ //parcour par indice du tableau pour chiffre/réel
        if (&Expression[0]==chiffre[y]){
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
    for (y = 0; y < 10 ; y++){//parcour par indice du tableau pour fonction
        if (Expression==fonction[y]){
            Res=FONCTION;
        }
    }
        
    return Res;
}*/

typelexem TypeString(char *Expression, int SizeExpression) {
    typelexem Res = ERREUR;
    char op[5] = {"+", "-", "*", "/", "**"};
    char chiffre[10] = {"0", "1", "2", "3", "4", "5", "6", "7", "8", "9"};
    char fonction[10] = {"abs", "sin", "sqrt", "log", "cos", "tan", "exp", "entier", "val_neg", "sinc"};
    
    int y;
    for (y = 0; y < 5; y++) {
        if (strcmp(Expression, op[y]) == 0) {
            Res = OPERATEUR;
        }
    }
    for (y = 0; y < 10; y++) {
        if (strcmp(Expression, chiffre[y]) == 0) {
            Res = REEL;
        }
    }
    if (strcmp(Expression, "x") == 0 || strcmp(Expression, "y") == 0) {
        Res = VARIABLE;
    }
    if (strcmp(Expression, "(") == 0) {
        Res = PAR_OUV;
    } else if (strcmp(Expression, ")") == 0) {
        Res = PAR_FERM;
    }
    for (y = 0; y < 10; y++) {
        if (strcmp(Expression, fonction[y]) == 0) {
            Res = FONCTION;
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
int CutStr (char *str, int SizeExpression, char *Strdecoupee){
    
    char *op[5] = {"+","-","*","/","**"};
    int lenop = 5;

    char *chiffre[11] = {"0","1","2","3","4","5","6","7","8","9","."};
    int lenchiffre = 11;
    
    char *fonction[10] = {"abs", "sin", "sqrt", "log", "cos", "tan", "exp", "entier", "val_neg", "sinc"};
    int lenfonction = 10;

    char *alphabet[26] = {"a","b","c","d","e","f","g","h","i","j","k","l","m","n","o","p","q","r","s","t","u","v","w","x","y","z"};
    int lenalphabet = 26;
    
    char *parenthese[2] = {"(",")"};
    int lenparenthese = 2;
    
    int i;

    for ( i = 0; i < SizeExpression ; i++){ // on regarde pour les nombres
        int j=i;
        if (IsInTab(chiffre,lenchiffre,str[j])==1){ // si ya un chiffre, on regarde pour la suite jusqu'à ce qu'il n'y en ai plus
            char res1 = "";
            int IndiceInjection = i;
            while(IsInTab(chiffre,lenchiffre,str[j])==1){
                int h = 0;
                for ( h = 0; h < lenop ; h++){
                    if (str[j]==chiffre[h])
                    {
                        strcat(res1,str[j]);
                    }
                }
                j++;
            }
            Strdecoupee[IndiceInjection]=res1;   // si i ya plus de chiffre, on ajoute le résultat s ou de virgule d'affillé, on ajoute le nombre entier dans le TabRes à l'indice du premier nombre détécté
        }
    }
    return 0;
}*/