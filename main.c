#include "Analyse_lexicale.h"
#include <string.h>

// exemple "3+cos(x*5)"



int main (){
    char str[TailleMax]  = "2x + 1" ;
    int lenExpression = strlen(str);
    char buffer[TailleMax];
    ExpressionSansLesEspaces(str,lenExpression,buffer);
    MultiplicationImplicite(str,lenExpression,buffer);

    typejeton jetontest;
    jetontest = TokenFonction("sin");
    
    return 0;
}











