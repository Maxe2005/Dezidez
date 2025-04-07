#include "Analyse_lexicale.h"
#include <string.h>

// exemple qui fonctionne avec 1 variable: 
//"(-SiNc(2x*y + 1))+10x"
//"Exp(5x + con(2.5))"
//"2x+x**2 +Log(x)"

//Exemple d'erreur traité :
//"( - fonctioninconnue(2*x + 5 ) )"    (fonction inconnue)     erreur 101
//"cosc(2x+log(18.5))"                  (fonction inconnue)     erreur 101
// "x + y**2 "                          (variable inconnue)     erreur 104
//" log( 2) * z "                       (variable inconnue)     erreur 104
// " log(sinc(1.1.1.1 ))"               (Nombre invalide)       erreur 102
//"x + 18...9 "                         (Nombre invalide)       erreur 102
// "2x + $ sin(2**5)"                   (Caractère inconnue)    erreur 103



//exemple qui fonctionne avec 2 variable (changer le dernier paramètre de Analyse_Lexical par 1)
// "x+sin(2y)"
//"(-cos    (   y**2+  18x)    )"
// 1/(x**2 + y**2)

//Exemple d'erreur traité avec 2 variable :
//"( - fonctioninconnue(x**y) )"        (fonction inconnue)     erreur 101
//"exp(xy)"                             (fonction inconnue)     erreur 101 (besoin de mettre un '*' entre les variables)
// "x + y**2 +e "                       (variable inconnue)     erreur 104
// "3.1415*(r**2) "                     (variable inconnue)     erreur 104 (le rayon n'est pas une variable reconnue)
// "3.1.4.1.5x+y"                       (Nombre invalide)       erreur 102
//"xy+ .69 "                            (Nombre invalide)       erreur 102
// "2x + !! sin(2**5)"                  (Caractère inconnue)    erreur 103


int main (){
    typejeton TokenListe[TailleMax]; //tableau de sortie
    char str[TailleMax]  = "x + y**2 +e " ; //expression tester
    int erreur = 0; //on initialise l'erreur (0 par défault)
    Analyse_Lexicale(TokenListe,str,&erreur,0);
    // On affiche le contenu de TokenListe

    afficher_tableau_jetons(TokenListe, TailleMax);
    printf("ERREUR : %d",erreur); //print d'éventuelle erreur
    
    return 0;
}











