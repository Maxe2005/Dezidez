#include "priorite.h"

int operateur (int tailletab, typejeton *tab){
    int indmax = 0;
    typeoperateur operateurmax = PLUS;
    for (int i = 0; i < tailletab; i++){
        if (tab[i].lexem != OPERATEUR){
            continue;
        }
        if (tab[i].valeur.operateur > operateurmax){
            operateurmax = tab[i].valeur.operateur;
            indmax = i;
        }
     
    }
    return indmax;
}

bool parenthese(int tailletab, typejeton *tab){
    int nbouv = 0;
    int nbfer = 0;
    for (int i = 0; i < tailletab; i++){
        if (tab[i].lexem == BAR_OUV){
            nbouv = nbouv + 1;
        }
        if(tab[i].lexem == BAR_FERM){
            nbfer = nbfer + 1;
        }
    }
    return nbouv == nbfer;
}



