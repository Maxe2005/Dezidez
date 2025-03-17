#include "evaluateur.h"


//creation  d'un EXEMPLE :
Node creation_arbre(typejeton racine, Node* precedent, Node* suivant){
    Node arbre;
    arbre.jeton=racine;
    arbre.pjeton_preced=precedent;
    arbre.pjeton_suiv=suivant;
    return arbre;
}


//FIN EXEMPLE

int main(){

//Deux
typejeton Deux;
Deux.lexem=REEL;
Deux.valeur.reel=2;
//Quatre
typejeton Quatre;
Quatre.lexem=REEL;
Quatre.valeur.reel=4;
//
typejeton x;
x.lexem=VARIABLE;
x.valeur.variable='x';
//Somme
typejeton Somme;
Somme.lexem=OPERATEUR;
Somme.valeur.operateur=PLUS;
// Sinus
typejeton Sinus;
Sinus.lexem=FONCTION;
Sinus.valeur.fonction=SIN;
Node DEUX_ARBRE = creation_arbre(Deux,NULL,NULL);
Node QUATRE_ARBRE = creation_arbre(Quatre,NULL,NULL);
Node X_ARBRE = creation_arbre(x,NULL,NULL);
Node SOMME_ARBRE = creation_arbre(Somme,&DEUX_ARBRE,&X_ARBRE);
Node SINUS_ARBRE = creation_arbre(Sinus,&SOMME_ARBRE,NULL);

remplacer_variable(&SOMME_ARBRE,4,0);

printf("%s","\n");
printf("%f",decodage_arbre(&SOMME_ARBRE));


return 0;
}