#include "Analyse_lexicale.h"
#include <string.h>

// exemple "3+cos(x*5)"



int main (){
    char str [100] = "2x + 1" ;
    int lenExpression = strlen(str);
    //char [lenExpression] CutString;
    typejeton Token[6] ;
    AnalyseLexicale(Token,str,lenExpression);
    return 0;
}











