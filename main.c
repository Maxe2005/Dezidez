#include "Analyse_lexicale.h"
#include <string.h>

// exemple "3+cos(x*5)"



int main (){
    char str[TailleMax]  = "223.348x + 759 - 365..." ;
    int lenExpression = strlen(str);
    char buffer[TailleMax];
    char Strdecoupee[TailleMax][TailleNombreMax];
    ExpressionSansLesEspaces(str,lenExpression,buffer);
    MultiplicationImplicite(str,lenExpression,buffer);

    // On cut la STR

    CutStr(str,lenExpression,Strdecoupee);
    afficherchainecarac(Strdecoupee, 10);


    typejeton jetontest;
    jetontest = TokenOperateur("-");
    return 0;
}











