#include "Analyse_lexicale.h"
#include <string.h>

// exemple "3+cos(x*5)"



int main (){
    typejeton TokenListe[TailleMax];
    char str[TailleMax]  = "(-sinc(2x + 1))+10x" ;
    Analyse_Lexicale(TokenListe,str);
    /*
    int lenExpression = strlen(str);
    
    char buffer[TailleMax];
    
    ExpressionSansLesEspaces(str,lenExpression,buffer);
    MultiplicationImplicite(str,lenExpression,buffer);

    // On cut la STR

    CutStr(str,strlen(str),TokenListe);


    typejeton jetontest;
    jetontest = TokenOperateur("-");
    */
    return 0;
}


//pour les erreur on a
//Fonction inconnue : 101
//plusieurs vigules dans un float : 102
//caractère inconnue : 103









