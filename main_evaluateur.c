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

int ERREUR_CODE=0;
//Deux
typejeton Deux;
Deux.lexem=REEL;
Deux.valeur.reel=2;
//Quatre
typejeton Quatre;
Quatre.lexem=REEL;
Quatre.valeur.reel=4;
//x
typejeton x;
x.lexem=VARIABLE;
x.valeur.variable='x';
//y
typejeton y;
y.lexem=VARIABLE;
y.valeur.variable='y';
//Somme
typejeton Somme;
Somme.lexem=OPERATEUR;
Somme.valeur.operateur=PLUS;
// Sinus
typejeton Sinus;
Sinus.lexem=FONCTION;
Sinus.valeur.fonction=SIN;

// racine carré
typejeton rcarre;
rcarre.lexem = FONCTION;
rcarre.valeur.fonction=SQRT;

//création des arbres
Node DEUX_ARBRE = creation_arbre(Deux,NULL,NULL);
Node QUATRE_ARBRE = creation_arbre(Quatre,NULL,NULL);
Node X_ARBRE = creation_arbre(x,NULL,NULL);
Node Y_ARBRE = creation_arbre(y,NULL,NULL);
Node RCARRE_ARBRE=creation_arbre(rcarre,&X_ARBRE,NULL);
Node SOMME_ARBRE = creation_arbre(Somme,&DEUX_ARBRE,&QUATRE_ARBRE);
Node SINUS_ARBRE = creation_arbre(Sinus,&SOMME_ARBRE,NULL);



// Création de l'arbre pour cos(x+y+5+6/0)

// Variables x et y
typejeton x1;
x1.lexem = VARIABLE;
x1.valeur.variable = 'x';

typejeton y1;
y1.lexem = VARIABLE;
y1.valeur.variable = 'y';

// Constantes 5, 6 et 0
typejeton cinq;
cinq.lexem = REEL;
cinq.valeur.reel = 5.0;

typejeton six;
six.lexem = REEL;
six.valeur.reel = 6.0;

typejeton zero1;
zero1.lexem = REEL;
zero1.valeur.reel = 0.0;

// Opérateurs
typejeton plus1;
plus1.lexem = OPERATEUR;
plus1.valeur.operateur = PLUS;

typejeton plus2;
plus2.lexem = OPERATEUR;
plus2.valeur.operateur = PLUS;

typejeton plus3;
plus3.lexem = OPERATEUR;
plus3.valeur.operateur = PLUS;

typejeton div1;
div1.lexem = OPERATEUR;
div1.valeur.operateur = DIV;

// Fonction cosinus
typejeton cosinus;
cosinus.lexem = FONCTION;
cosinus.valeur.fonction = COS;

// Construction de l'arbre pour cos(x+y+5+6/0)
Node X1_ARBRE = creation_arbre(x1, NULL, NULL);
Node Y1_ARBRE = creation_arbre(y1, NULL, NULL);
Node SOMME1_ARBRE = creation_arbre(plus1, &X1_ARBRE, &Y1_ARBRE);

Node CINQ_ARBRE = creation_arbre(cinq, NULL, NULL);
Node SOMME2_ARBRE = creation_arbre(plus2, &SOMME1_ARBRE, &CINQ_ARBRE);

Node SIX_ARBRE = creation_arbre(six, NULL, NULL);
Node ZERO1_ARBRE = creation_arbre(zero1, NULL, NULL);
Node DIV1_ARBRE = creation_arbre(div1, &SIX_ARBRE, &ZERO1_ARBRE);

Node SOMME3_ARBRE = creation_arbre(plus3, &SOMME2_ARBRE, &DIV1_ARBRE);
Node COSINUS_ARBRE = creation_arbre(cosinus, &SOMME3_ARBRE, NULL);


// Test 1: Division par zéro simple - a/0
typejeton a1;
a1.lexem = VARIABLE;
a1.valeur.variable = 'x';

typejeton zero2;
zero2.lexem = REEL;
zero2.valeur.reel = 0.0;

typejeton division1;
division1.lexem = OPERATEUR;
division1.valeur.operateur = DIV;

Node A1_ARBRE = creation_arbre(a1, NULL, NULL);
Node ZERO2_ARBRE = creation_arbre(zero2, NULL, NULL);
Node DIVISION1_ARBRE = creation_arbre(division1, &A1_ARBRE, &ZERO2_ARBRE);


// Test 2: Puissance négative pour 0 - 0^(-1)
typejeton zero3;
zero3.lexem = REEL;
zero3.valeur.reel = 0.0;

typejeton moins_un;
moins_un.lexem = REEL;
moins_un.valeur.reel = -1.0;

typejeton puissance1;
puissance1.lexem = OPERATEUR;
puissance1.valeur.operateur = PUIS;

Node ZERO3_ARBRE = creation_arbre(zero3, NULL, NULL);
Node MOINS_UN_ARBRE = creation_arbre(moins_un, NULL, NULL);
Node PUISSANCE1_ARBRE = creation_arbre(puissance1, &ZERO3_ARBRE, &MOINS_UN_ARBRE);


// Test 3: Puissance 0 pour 0 - 0^0
typejeton zero4;
zero4.lexem = REEL;
zero4.valeur.reel = 0.0;

typejeton zero5;
zero5.lexem = REEL;
zero5.valeur.reel = 0.0;

typejeton puissance2;
puissance2.lexem = OPERATEUR;
puissance2.valeur.operateur = PUIS;

Node ZERO4_ARBRE = creation_arbre(zero4, NULL, NULL);
Node ZERO5_ARBRE = creation_arbre(zero5, NULL, NULL);
Node PUISSANCE2_ARBRE = creation_arbre(puissance2, &ZERO4_ARBRE, &ZERO5_ARBRE);


// Test 4: Log(0)
typejeton zero6;
zero6.lexem = REEL;
zero6.valeur.reel = 0.0;

typejeton log1;
log1.lexem = FONCTION;
log1.valeur.fonction = LOG;

Node ZERO6_ARBRE = creation_arbre(zero6, NULL, NULL);
Node LOG1_ARBRE = creation_arbre(log1, &ZERO6_ARBRE, NULL);


// Test 5: Log(a) a<0 - log(-2)
typejeton moins_deux;
moins_deux.lexem = REEL;
moins_deux.valeur.reel = -2.0;

typejeton log2;
log2.lexem = FONCTION;
log2.valeur.fonction = LOG;

Node MOINS_DEUX_ARBRE = creation_arbre(moins_deux, NULL, NULL);
Node LOG2_ARBRE = creation_arbre(log2, &MOINS_DEUX_ARBRE, NULL);


// Test 6: tan((pi/2)*a) a=entier - tan(pi/2)
// Création de pi/2
typejeton pi_deux;
pi_deux.lexem = REEL;
pi_deux.valeur.reel = M_PI/2; // Valeur approximative de pi/2

typejeton tan1;
tan1.lexem = FONCTION;
tan1.valeur.fonction = TAN;

Node PI_DEUX_ARBRE = creation_arbre(pi_deux, NULL, NULL);
Node TAN1_ARBRE = creation_arbre(tan1, &PI_DEUX_ARBRE, NULL);


// Test 7: sinc(x) où x=0
typejeton zero7;
zero7.lexem = REEL;
zero7.valeur.reel = 0.0;

typejeton sinc1;
sinc1.lexem = FONCTION;
sinc1.valeur.fonction = SINC;

Node ZERO7_ARBRE = creation_arbre(zero7, NULL, NULL);
Node SINC1_ARBRE = creation_arbre(sinc1, &ZERO7_ARBRE, NULL);


// Test 8: Une expression plus complexe avec plusieurs erreurs potentielles
// sqrt(log(-1) + 1/0)
typejeton moins_un2;
moins_un2.lexem = REEL;
moins_un2.valeur.reel = -1.0;

typejeton log3;
log3.lexem = FONCTION;
log3.valeur.fonction = LOG;

typejeton un;
un.lexem = REEL;
un.valeur.reel = 1.0;

typejeton zero8;
zero8.lexem = REEL;
zero8.valeur.reel = 0.0;

typejeton div2;
div2.lexem = OPERATEUR;
div2.valeur.operateur = DIV;

typejeton plus4;
plus4.lexem = OPERATEUR;
plus4.valeur.operateur = PLUS;

typejeton sqrt1;
sqrt1.lexem = FONCTION;
sqrt1.valeur.fonction = SQRT;

Node MOINS_UN2_ARBRE = creation_arbre(moins_un2, NULL, NULL);
Node LOG3_ARBRE = creation_arbre(log3, &MOINS_UN2_ARBRE, NULL);

Node UN_ARBRE = creation_arbre(un, NULL, NULL);
Node ZERO8_ARBRE = creation_arbre(zero8, NULL, NULL);
Node DIV2_ARBRE = creation_arbre(div2, &UN_ARBRE, &ZERO8_ARBRE);

Node PLUS4_ARBRE = creation_arbre(plus4, &LOG3_ARBRE, &DIV2_ARBRE);
Node SQRT1_ARBRE = creation_arbre(sqrt1, &PLUS4_ARBRE, NULL);




printf("%s","\n");
printf("%f",evaluateur(&PUISSANCE2_ARBRE ,0,0,&ERREUR_CODE));
printf("%s","\n");
printf("%d", ERREUR_CODE);
}