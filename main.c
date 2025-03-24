#include "Analyse_lexicale.h"
#include <string.h>

// exemple "3+cos(x*5)"



int main (){
    char str[100]  = "2x + 1" ;
    int lenExpression = strlen(str);
    char buffer[100];
    ExpressionSansLesEspaces(str,lenExpression,buffer);
    
    printf("Sans les espaces : %s",str);

    MultiplicationImplicite(str,lenExpression,buffer);
    
    printf("Avec multiplication implicite : %s",str);
    return 0;
}











