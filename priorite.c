#include "priorite.h"

Node arbrevide(){
    Node arbre = (Node){
        .pjeton_preced = NULL,
        .pjeton_suiv = NULL,
        .jeton = FIN
    };
    return arbre;
}

Node operateur( typejeton *tab, int debut, int fin){
    int  indoputile = minIndice(tab, debut, fin); 
    if (debut==fin){
        return arbrevide();
    }
    if (indoputile != -1){
        Node Fg= operateur(tab, debut, indoputile -1);
        Node Fd = operateur(tab,indoputile + 1, fin);
        Node arbre = (Node){
            .pjeton_preced = &Fg,
            .pjeton_suiv = &Fd,
            .jeton = tab[indoputile]
        };
    }
    else{
        switch (tab[debut].lexem){
            case FONCTION:
                if (tab[debut + 1].lexem != PAR_OUV){
                        //ERREUR
                        return;
                }    
                Node Fg = operateur(tab, debut + 2, fin - 1);
                Node arbre = (Node){
                    .pjeton_preced = &Fg,
                    .jeton = tab[indoputile]
                }; 
                break;
            case PAR_OUV:
                if (tab[fin].lexem != PAR_FERM){
                    //ERREUR
                    return;
                }
                else{
                    Fg = operateur(tab, debut + 1, fin - 1);
                    Node arbre = (Node){
                        .pjeton_preced = &Fg,
                        .jeton = tab[indoputile]
                    };
                }
                break;
            case PAR_FERM:
                //ERREUR
                return;
            case REEL:
            if (tab[debut+1].lexem != FIN){
                Fg = arbrevide();
                Node arbre = (Node){
                    .pjeton_preced = &Fg,
                    .jeton = tab[indoputile]
                };
            }
            else{
                //ERREUR
            }
        } 
    }
}

int minIndice(typejeton *tab,  int debut, int fin){
    int prof = 0;
    int operateurutile = 0;
    int profoperutile =0;
    int indoputile = -1;
    bool continuer = true;
    for (int i = 0; i< fin; i++){
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
        //ERREUR #TODO
        return;
    }
    return (indoputile);
}





bool parenthese(int tailletab, typejeton *tab){
    int nbouv = 0;
    int nbfer = 0;
    for (int i = 0; i < tailletab; i++){
        if (tab[i].lexem == PAR_OUV){
            nbouv = nbouv + 1;
        }
        if(tab[i].lexem == PAR_FERM){
            nbfer = nbfer + 1;
        }
    }
    return nbouv == nbfer;
}



