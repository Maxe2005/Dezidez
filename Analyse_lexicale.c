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

void MajtoMin(char *str) {
    while (*str) {
        *str = tolower(*str);
        str++;
    }
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


int PlusieursVirgules (char * nombre){
    int compteur = 0;
    int res = 0;
    if (nombre[0]=='.'){ //cas des nombre ".2591" ou "."
        res = 1;
    }
    
    for (int i = 0; i < strlen(nombre); i++){
        if (nombre[i]== '.'){
            compteur++;
        }
    }
    if (compteur > 1){
        res = 1;
    }
    return res;
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
    buffer[j]='\0';
    //on remplace l'expression par le buffer
    strcpy(Expression, buffer);
    strcat(Expression,"\0");
    // on vide le buffer
    memset(buffer, '\0', sizeof(buffer));
}


typejeton TokenFonction (char *Element, int* erreur){
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
        
    }  
    else {
        *erreur =FONCTION_INCONNUE;
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
    }  
    return fonct;     
}


typejeton TokenVariable (char *Element, int *erreur, int dimension){
    char *var2[] = {"x","y"};
    char *var1[] = {"x"};
    typejeton fonct;
    if (dimension){
        if (IsInTab2(var2,2,Element)){
            fonct.lexem = VARIABLE;
            if (ComparaisonString(Element,"y")){
                fonct.valeur.variable = 'y';
            }
            else if (ComparaisonString(Element,"x")){
                fonct.valeur.variable = 'x';
            }
        }  
        else{
            *erreur = VARIABLE_INCONNUE; 
        }
    }else {
        if (IsInTab2(var1,2,Element)){
            fonct.lexem = VARIABLE;
            fonct.valeur.variable = 'x';
        } 
        else{
            *erreur = VARIABLE_INCONNUE; 
        }
    }
    
    return fonct;     
}


typejeton TokenReel (char *Element, int* erreur){
    typejeton fonct;
    if (PlusieursVirgules(Element)){
        *erreur = NOMBRE_INVALIDE;
    }else{
        fonct.lexem = REEL;
        fonct.valeur.reel = atof(Element);
    }
    return fonct;
}  


void DecompositionToken(char *str, int SizeExpression, typejeton TabToken[TailleMax],int* erreur,int dimension) {
    char buffer[TailleMax];
    char *chiffre[] = {"0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "."};
    int lenchiffre = 11;
    char *op[] = {"+","-","*","/","**"};
    int lenop = 5;
    char *fonction[] = {"abs", "sin", "sqrt", "log", "cos", "tan", "exp", "entier", "val_neg", "sinc"};
    int lenfonction = 10;
    char *alphabet[] = {"a","b","c","d","e","f","g","h","i","j","k","l","m","n","o","p","q","r","s","t","u","v","w","x","y","z"};
    int lenalphabet = 26;
    char *parenthese[] = {"(",")"};
    int lenparenthese = 2;
    int i;
    int indiceinjection = 0;
    typejeton TokenBuffer;
    


    for (i = 0; i <= SizeExpression;i++) {
        // Si on a atteint la fin de la chaîne, on sort
        
        if (str[i] == '\0') break;
        
        char strenchainedecarac[2] = {str[i], '\0'}; //on met le caractère étudier dans le buffer
        strcpy(buffer, strenchainedecarac);

        // Traitement des nombres
        if (IsInTab3(chiffre, lenchiffre, buffer) == 1) {
            int longueurdunombre = 0;
            char reschiffre[TailleMax] = "";

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
            TabToken[indiceinjection]=TokenReel(reschiffre,erreur);
            indiceinjection++;
            i = i + longueurdunombre - 1;  // Ajuste l'index 'i' pour reprendre l'analyse au bon endroit
        }
        // Traitement des élément négatif (-x...) 
        else if (str[i]== '(' && str[i+1]=='-' ) { // si on a (- on commence la recher jusqu'a la dernière parenthèse
            int longueurdunombre = 2;
            TabToken[indiceinjection].lexem = FONCTION;
            TabToken[indiceinjection].valeur.fonction = VAL_NEG;
            indiceinjection++;
            TabToken[indiceinjection].lexem = PAR_OUV;
            indiceinjection++;
            i = i + longueurdunombre -1;  // Ajuste l'index 'i' pour reprendre l'analyse au bon endroit (on fait +1 pour skip la dernière parenthèse déjà traité)
        }
        // Gestion des opérateur

        else if (IsInTab3(op, lenop, buffer) == 1) {
            if (str[i]=='*' && str[i+1]=='*'){ //cas de la puissance
                strcat(buffer,"*"); //comme ça on injecte directement la puissance
                i++;
            }
            
            TabToken[indiceinjection]=TokenOperateur(buffer);
            indiceinjection++;
            //y'a un problème d'indice dans l'expression de teste on a 2 fois '-'
        }

        // Gestion des parenthèses on fait cas par cas car plus simple pour les tokens

        else if (str[i]==')') {
            TabToken[indiceinjection].lexem = PAR_FERM;
            indiceinjection++;
        }else if (str[i]=='('){
            TabToken[indiceinjection].lexem = PAR_OUV;
            indiceinjection++;
        }
        //gestion des fonction et variable apparement
        else if (IsInTab3(alphabet, lenalphabet, buffer) == 1) {
            int longueurident = 0;
            char residentificateur[TailleMax] = "";

            while (i + longueurident < SizeExpression && str[i + longueurident] != '\0') {
                strenchainedecarac[0] = str[i + longueurident];
                strenchainedecarac[1] = '\0';
                strcpy(buffer, strenchainedecarac);
                
                if (IsInTab3(alphabet, lenalphabet, buffer) == 1) {
                    strcat(residentificateur, buffer);
                    longueurident++;
                }else {
                    break; 
                }
            }
            if (longueurident > 1){ //cas d'une fonction
                TabToken[indiceinjection]=TokenFonction(residentificateur, erreur);
                indiceinjection++;
                i = i + longueurident -1 ; 
            } else { //cas d'une variable
                TabToken[indiceinjection]=TokenVariable(residentificateur, erreur,dimension);
                indiceinjection++;
                i = i + longueurident -1; 
            }
            
            
        }
        else{ //cas où notre élément ne correspond à aucune des condition au dessue ==> erreur on ne connais pas ce caractère
            *erreur = CARACTERE_INCONNUE;         

        }
    }
    TabToken[indiceinjection].lexem = FIN;
}

void Analyse_Lexicale (typejeton TabToken[TailleMax],char Expression[TailleMax],int* erreur,int  Dimension){
    int tailleExpression = strlen(Expression);
    char buffert[TailleMax];
    ExpressionSansLesEspaces(Expression,tailleExpression,buffert);//on retire les potentiel espace
    MajtoMin(Expression); // On retire toute les majuscules
    MultiplicationImplicite(Expression,tailleExpression,buffert);//on rajoute les multiplications dans les cas 2x --> 2*x
    tailleExpression = strlen(Expression);
    DecompositionToken(Expression,tailleExpression,TabToken,erreur,Dimension);//transforme l'expression en un tableau de Token 
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

