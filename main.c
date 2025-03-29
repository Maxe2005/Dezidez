#include "Analyse_lexicale.h"
#include <string.h>

// exemple "3+cos(x*5)"



int main (){
    typejeton TokenListe[TailleMax];
    char str[TailleMax]  = "sinc( 2) + 223.348x + 759**2 - (-365) +" ;
    int lenExpression = strlen(str);
    char buffer[TailleMax];

    ExpressionSansLesEspaces(str,lenExpression,buffer);
    MultiplicationImplicite(str,lenExpression,buffer);

    // On cut la STR

    CutStr(str,strlen(str),TokenListe);


    typejeton jetontest;
    jetontest = TokenOperateur("-");
    return 0;
}











