#include "Analyse_lexicale.h"
#include <string.h>

// exemple "3+cos(x*5)"



int main (){
    char str[TailleMax]  = "sinc( 2) + 223.348x + 759**2 - (-365) +" ;
    int lenExpression = strlen(str);
    char buffer[TailleMax];
    char Strdecoupee[TailleMax][TailleNombreMax];
    printf("%d",strlen(str));
    ExpressionSansLesEspaces(str,lenExpression,buffer);
    MultiplicationImplicite(str,lenExpression,buffer);

    // On cut la STR
    printf("%d",strlen(str));
    CutStr(str,strlen(str),Strdecoupee);
    afficherchainecarac(Strdecoupee, lenExpression);


    typejeton jetontest;
    jetontest = TokenOperateur("-");
    return 0;
}











