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

int IsInTab3(char *tab[], int size, char *element) {
    for (int i = 0; i < size; i++) {
        if (strcmp(tab[i], element) == 0) {
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
    if (IsInTab2(fonction,9,Element)){// on regarde si c'est une fonction
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
            printf("ERREUR");
        }
    }  
    return fonct;     
}

typejeton TokenOperateur (char *Element){
    char *operation[] = {"+","-","*","/","**"};
    typejeton fonct;
    if (IsInTab2(operation,5,Element)){
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
            printf("ERREUR");
            //Erreur Unkown fonction
        }
    }  
    return fonct;     
}

typejeton TokenParenthese (char *Element){
    char *operation[] = {"(",")"};
    typejeton fonct;
    if (IsInTab2(operation,5,Element)){
        if (ComparaisonString(Element,"(")){
            fonct.lexem = PAR_OUV;
        }
        else if (ComparaisonString(Element,")")){
            fonct.lexem = PAR_FERM;
        }
        else {
            printf("ERREUR");
            //Erreur Unkown character
        }
    }  
    return fonct;     
}




typejeton TokenVariable (char *Element){
    char *var[] = {"x","y"};
    typejeton fonct;
    if (IsInTab2(var,2,Element)){
        fonct.lexem = VARIABLE;
        if (ComparaisonString(Element,"y")){
            fonct.valeur.variable = 'y';
        }
        else if (ComparaisonString(Element,"x")){
            fonct.valeur.variable = 'x';
        }
        else{
            printf("ERREUR");
            //Erreur
        }
    }  
    return fonct;     
}


typejeton TokenReelPositif (char *Element){
    typejeton fonct;
    fonct.lexem = REEL;
    
    fonct.valeur.reel = atof(Element);
    return fonct;
}  

typejeton TokenReelNegatif (char *Element){
    typejeton fonct;
    int tailleElement = strlen(Element);
    char buffer[100];
    char debug;
    int i;
    int j=0;
    memset(buffer, '\0', sizeof(buffer));
    
    //on parcour l'Element pour garder que le nombre
    for ( i = 0; i < tailleElement; i++){
        if (Element[i] != '(' && Element[i] != ')' ){
            buffer[j++]=Element[i];
            //memset(buffer, '\0', sizeof(buffer));
        }
    }
    fonct.lexem = REEL;
    fonct.valeur.reel = atof(buffer);
    return fonct;
}  

void afficherchainecarac(char Strdecoupee[][100], int size) {
    for (int i = 0; i < size; i++) {
        if (Strdecoupee[i] != NULL) {  // Vérifie que l'élément n'est pas NULL
            printf("Strdecoupee[%d] = \"%s\"\n", i, Strdecoupee[i]);
        }
    }
}


void CutStr(char *str, int SizeExpression, char Strdecoupee[TailleMax][TailleNombreMax], typejeton TokenList[TailleMax]) {
    char buffer[TailleMax];
    char *chiffre[] = {"0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "."};
    int lenchiffre = 11;
    char *op[] = {"+","-","*","/","**"};
    int lenop = 5;
    char *fonction[] = {"abs", "sin", "sqrt", "log", "cos", "tan", "exp", "entier", "val_neg", "sinc"};
    int lenfonction = 10;
    char *alphabet[] = {"a","b","c","d","e","f","g","h","i","j","k","l","m","n","o","p","q","r","s","t","u","v","w","x","y","z"};
    int lenalphabet = 26;
    char *alphabetfonction[] = {"a","s","l","c","t","e","v"};
    int lenalphabetfonction = 7;
    char *parenthese[] = {"(",")"};
    int lenparenthese = 2;
    char *variables[] = {"x","y"};
    int lenvariables = 2;
    
    int i;
    int indiceinjection = 0;

    // Initialisation de Strdecoupee avec des chaînes vides
    for (int j = 0; j < TailleMax; j++) {
        Strdecoupee[j][0] = '\0';
    }

    for (i = 0; i < SizeExpression; i++) {
        // Si on a atteint la fin de la chaîne, on sort
        if (str[i] == '\0') break;
        
        char strenchainedecarac[2] = {str[i], '\0'};
        strcpy(buffer, strenchainedecarac);

        // Traitement des nombres
        if (IsInTab3(chiffre, lenchiffre, buffer) == 1) {
            int longueurdunombre = 0;
            char reschiffre[TailleNombreMax] = "";

            // Continue tant que tu trouves des chiffres
            while (i + longueurdunombre < SizeExpression && str[i + longueurdunombre] != '\0') {
                strenchainedecarac[0] = str[i + longueurdunombre];
                strenchainedecarac[1] = '\0';
                strcpy(buffer, strenchainedecarac);
                
                if (IsInTab3(chiffre, lenchiffre, buffer) == 1) {
                    strcat(reschiffre, buffer);
                    longueurdunombre++;
                } else {
                    break;
                }
            }
            
            strcpy(Strdecoupee[indiceinjection], reschiffre);
            TokenList[indiceinjection]=TokenReelPositif(reschiffre);
            indiceinjection++;
            i = i + longueurdunombre - 1;  // Ajuste l'index 'i' pour reprendre l'analyse au bon endroit
        }

        // Gestion des opérateurs

        else if (IsInTab3(op, lenop, buffer) == 1) {
            strcpy(Strdecoupee[indiceinjection], buffer);
            TokenList[indiceinjection]=TokenOperateur(buffer);
            indiceinjection++;
        }

        // Gestion des parenthèses

        else if (IsInTab3(parenthese, lenparenthese, buffer) == 1) {
            strcpy(Strdecoupee[indiceinjection], buffer);
            TokenList[indiceinjection]=TokenParenthese(buffer);                       // IL faut faire token parenthèse
            indiceinjection++;
        }
        
        // Gestion des fonctions

        else if (IsInTab3(alphabetfonction, lenalphabetfonction, buffer) == 1){
            int longueurfonction = 0;
            char resfonction[TailleNombreMax] = "";
            while (i + longueurfonction < SizeExpression && str[i + longueurfonction] != '\0' && str[i + longueurfonction] != '(' ){
                strenchainedecarac[0] = str[i + longueurfonction];
                strenchainedecarac[1] = '\0';

                strcpy(buffer, strenchainedecarac);
                strcat(resfonction, buffer);
                longueurfonction++;
            }
            if (IsInTab3(fonction, lenfonction, resfonction)==1){
                strcpy(Strdecoupee[indiceinjection], resfonction);
                TokenList[indiceinjection]=TokenFonction(resfonction);
                indiceinjection++;
                i = i + longueurfonction - 1;
            }
            else {
                strcpy(Strdecoupee[indiceinjection], "fonction inconnue");
                indiceinjection++;
                i = i + longueurfonction - 1;

            }
        }

        
        // Gestion des variables

        // faudra modifier les conditions pour quand c'est 2D et quand c'est 3D

        else if (IsInTab3(variables, lenvariables, buffer) == 1) {
            strcpy(Strdecoupee[indiceinjection], buffer);
            TokenList[indiceinjection]=TokenVariable(buffer);
            indiceinjection++;
        }
    }
}



int lenStrList (char Strdecoupee[TailleMax][TailleNombreMax]){
    int compteur = 0;
    for (int i=0; i<TailleMax ;i++){
        if (Strdecoupee[i][0]!='\0'){
            compteur++;
        }
    }
    return compteur;
}



// Fonction pour afficher un jeton unique
void afficher_jeton(typejeton jeton) {
    printf("Lexem: ");
    switch (jeton.lexem) {
        case REEL:
            printf("REEL, Valeur: %.2f\n", jeton.valeur.reel);
            break;
        case OPERATEUR:
            printf("OPERATEUR, Type: ");
            switch (jeton.valeur.operateur) {
                case PLUS: printf("+\n"); break;
                case MOINS: printf("-\n"); break;
                case FOIS: printf("*\n"); break;
                case DIV: printf("/\n"); break;
                case PUIS: printf("^\n"); break;
                default: printf("Inconnu\n"); break;
            }
            break;
        case FONCTION:
            printf("FONCTION, Type: ");
            switch (jeton.valeur.fonction) {
                case ABS: printf("ABS\n"); break;
                case SIN: printf("SIN\n"); break;
                case SQRT: printf("SQRT\n"); break;
                case LOG: printf("LOG\n"); break;
                case COS: printf("COS\n"); break;
                case TAN: printf("TAN\n"); break;
                case EXP: printf("EXP\n"); break;
                case ENTIER: printf("ENTIER\n"); break;
                case VAL_NEG: printf("VAL_NEG\n"); break;
                case SINC: printf("SINC\n"); break;
                default: printf("Inconnu\n"); break;
            }
            break;
        case VARIABLE:
            printf("VARIABLE, Nom: %c\n", jeton.valeur.variable);
            break;
        case ERREUR:
            printf("ERREUR rencontrée\n");
            break;
        case FIN:
            printf("FIN de l'analyse\n");
            break;
        case PAR_OUV:
            printf("Parenthese Ouvrante: (\n");
            break;
        case PAR_FERM:
            printf("Parenthese Fermante: )\n");
            break;
        case BAR_OUV:
            printf("Barre Ouvrante: |\n");
            break;
        case BAR_FERM:
            printf("Barre Fermante: |\n");
            break;
        case ABSOLU:
            printf("ABSOLU\n");
            break;
        default:
            printf("Type inconnu\n");
            break;
    }
}

// Fonction pour afficher un tableau de jetons
void afficher_tableau_jetons(typejeton tableau[], int taille) {
    printf("=== Affichage du tableau de jetons ===\n");
    for (int i = 0; i < taille; i++) {
        printf("Jeton %d: ", i + 1);
        afficher_jeton(tableau[i]);
    }
    printf("======================================\n");
}


