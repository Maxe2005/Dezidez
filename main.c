#include "Analyse_lexicale.h"
#include <string.h>

// exemple "3+cos(x*5)"



int main (){
    
    char str[TailleMax]  = "sin(223.348)*sinc(x*4) + 759 - 365..." ;

    // définition des variables utiles

    int lenExpression = strlen(str);
    char buffer[TailleMax];
    char Strdecoupee[TailleMax][TailleNombreMax];
    typejeton TokenList[TailleMax];

    // On nettoie la str

    ExpressionSansLesEspaces(str,lenExpression,buffer);
    MultiplicationImplicite(str,lenExpression,buffer);


    // On cut la str et on rempli en mm temps le tableau de jeton : TokenList
    
    CutStr(str,lenExpression,Strdecoupee,TokenList);

    // On affiche d'abord le contenu de Strdecoupee puis le contenu de TokenList

    afficherchainecarac(Strdecoupee, lenExpression);
    afficher_tableau_jetons(TokenList, TailleMax);

    return 0;
}











