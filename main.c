#include "Analyse_lexicale.h"
#include <string.h>

// exemple "3+cos(x*5)"



int main (){
    typejeton TokenListe[TailleMax];
    char str[TailleMax]  = "(-sinc(2x*1.2.2 + 1))+10x" ;
    int erreur = 0;
    Analyse_Lexicale(TokenListe,str,&erreur);
    printf("%d",erreur);
    return 0;
}











