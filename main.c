#include "Analyse_lexicale.h"
#include <string.h>

// exemple "3+cos(x*5)"



int main (){
    char str [100] = "2x + 1" ;
    int lenExpression = strlen(str);
    /*
    char TabCut[lenExpression];
    memset(TabCut, ' ', sizeof(TabCut)); // Tableau qui va servir à créer la Str Découpée
    */
    int i = 0;

    /*for ( i = 0; i < lenExpression ; i++){
        printf("%c",TabCut[i]);
    }*/

    char tab[2] = {'0', '1'};
        //char [lenExpression] CutString;
    printf("%d \n",IsInTab(tab,2,'1'));
    typejeton Token[6] ;
    //AnalyseLexicale(Token,str,lenExpression);
    return 0;
}











