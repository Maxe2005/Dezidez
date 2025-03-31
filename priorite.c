#include "priorite.h"

Node arbrevide(){
    Node arbre = (Node){
        .pjeton_preced = NULL,
        .pjeton_suiv = NULL,
        .jeton = FIN
    };
    return arbre;
}

Node operateur( typejeton *tab, int debut, int fin, typeerreur *erreur){
    printf("debut =%d fin %d\n", debut,fin);
    affiche_liste_jeton(tab, debut, fin);
    int  indoputile = minIndice(tab, debut, fin, erreur);
    printf("voir indoputile %d \n", indoputile);
    if (*erreur != 0){
        printf("erreur != 0 = %d\n", *erreur);
        return arbrevide();
    }
    // if (parenthese(debut, fin, tab)!= true){
    //     *erreur = PROBLEMES_NOMBRE_PARENTHESES;
    //     return arbrevide(); 
    // }
    if (debut>fin){
        printf("debut>fin\n");
        *erreur = MEMBRE_VIDE;
        return arbrevide();
    }
    if (indoputile != -1){
        printf("indoputile != -1\n");
        Node Fg= operateur(tab, debut, indoputile -1, erreur);
        Node Fd = operateur(tab,indoputile + 1, fin, erreur);
        Node arbre = (Node){
            .pjeton_preced = &Fg,
            .pjeton_suiv = &Fd,
            .jeton = tab[indoputile]
        };
        return arbre;
    }
    else{
        switch (tab[debut].lexem){
            case FONCTION:
                if (tab[debut + 1].lexem != PAR_OUV){
                        *erreur = PROBLEME_PARENTHESES_FONCTIONS;
                        return arbrevide();
                }
                Node Fg = operateur(tab, debut + 2, fin - 1, erreur);
                Node arbre = (Node){
                    .pjeton_preced = &Fg,
                    .jeton = tab[indoputile]
                }; 
                if (tab[debut + 2].lexem !=(FONCTION||REEL||VARIABLE)){
                    *erreur = PROBLEME_INTERIEUR_PARENTHESE;
                    return arbrevide();
                }
                break;
            case PAR_OUV:
                if (tab[fin].lexem != PAR_FERM){
                    *erreur = PARENTHESE_PAS_FERMEE;
                    return arbrevide();
                }
                if (tab[debut + 2].lexem !=(FONCTION||REEL||VARIABLE)){
                    *erreur = PROBLEME_INTERIEUR_PARENTHESE;
                    return arbrevide();
                }
                else{
                    Fg = operateur(tab, debut + 1, fin - 1, erreur);
                    Node arbre = (Node){
                        .pjeton_preced = &Fg,
                        .jeton = tab[indoputile]
                    };
                }
                break;
            case PAR_FERM:
                *erreur = PARENTHESE_FERMEE_1_ER_JETON;
                return arbrevide(); 
            case REEL:
                if (fin - debut == 0){
                    // Node Fg = arbrevide();
                    Node arbre = (Node){
                        .pjeton_preced = NULL,
                        .pjeton_suiv = NULL,
                        .jeton = tab[debut]
                       };
                }
                else{
                    *erreur = PROBLEME_APRES_REEL;
                    return arbrevide();
                }
        } 
    }
}

int minIndice(typejeton *tab,  int debut, int fin, typeerreur *erreur){
    int prof = 0;
    int operateurutile = 0;
    int profoperutile =0;
    int indoputile = -1;
    bool continuer = true;
    for (int i = debut; i< fin+1; i++){
        switch (tab[i].lexem){
            case OPERATEUR:
                if (prof == 0){
                    if (operateurutile < tab[i].lexem){
                        profoperutile = prof;
                        operateurutile = tab[i].lexem;
                        indoputile = i;
                    }
                }
                break;
                
            case PAR_OUV:
                prof = prof + 1;
                break;

            case PAR_FERM:
                prof = prof - 1;
                break;   

            default:
                break;
            }
        }
    if (&tab[0]){
        continuer = false;
    }
    if (prof!= 0){
        *erreur = PROBLEMES_NOMBRE_PARENTHESES;
        printf("test mindice. prof=%d\n", prof);
        return -1;
    }
    return (indoputile);
}


int calculTaille(typejeton *tab){
    for (int i = 0; i<TAILLE_MAX; i++){
        if (tab[i].lexem == FIN){
            return (i-1);
        }
    }
    return -1;
}

bool parenthese(int debut, int fin, typejeton *tab){
    int nbouv = 0;
    int nbfer = 0;
    for (int i = 0; i < fin - debut + 1; i++){
        if (tab[i].lexem == PAR_OUV){
            nbouv = nbouv + 1;
        }
        if(tab[i].lexem == PAR_FERM){
            nbfer = nbfer + 1;
        }
    }
    printf ("parentheses, nbouv=%d, nbfer=%d\n", nbouv, nbfer);
    // printf(nbouv);
    // printf(nbfer);
    return nbouv == nbfer;
}

Node Syntaxique(typejeton *tab, typeerreur *erreur){
    int fin = calculTaille(tab);
    Node arbrePlein = operateur( tab, 0, fin, erreur);
    return arbrePlein;
}

void affiche_liste_jeton(typejeton *tab, int debut, int fin){
    for(int i=debut;i<fin+1;i++){
        printf(", %d", tab[i].lexem);
    }
    printf("\n");
}