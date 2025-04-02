#include "priorite.h"

Node* arbrevide(){
    Node* arbre = (Node*)malloc(sizeof(Node));
    *arbre = (Node){
        .pjeton_preced = NULL,
        .pjeton_suiv = NULL,
        .jeton = {.lexem = FIN, .valeur = {0}}
    };
    return arbre;
}

Node* operateur( typejeton *tab, int debut, int fin, typeerreur *erreur){

    afficher_liste_jetons(tab, debut, fin);
    int  indoputile = minIndice(tab, debut, fin, erreur);
    printf("indoputile=%d\n", indoputile);
    if (*erreur != 0){
        return arbrevide();
    }
    if (debut>fin){
        *erreur = MEMBRE_VIDE;
        return arbrevide();
    }
    if (indoputile != -1){
        Node* Fg = (Node*)malloc(sizeof(Node));
        Fg= operateur(tab, debut, indoputile - 1, erreur);
        Node* Fd = (Node*)malloc(sizeof(Node));
        Fd = operateur(tab,indoputile + 1, fin, erreur);
        Node* arbre = (Node*)malloc(sizeof(Node));
        *arbre = (Node){
            .pjeton_preced = Fg,
            .pjeton_suiv = Fd,
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
                if ((tab[debut+2].lexem!=FONCTION) && (tab[debut+2].lexem!=REEL) && (tab[debut+2].lexem !=VARIABLE)){
                    *erreur = MEMBRE_VIDE;
                    return arbrevide();
                }

                Node* Fg = operateur(tab, debut + 2, fin - 1, erreur);
                Node* arbre = (Node*)malloc(sizeof(Node));
                *arbre = (Node){
                    .pjeton_preced = Fg,
                    .pjeton_suiv = NULL,
                    .jeton = tab[debut]   
                }; 
                return arbre;
                break;
            case PAR_OUV:
                if (tab[fin].lexem != PAR_FERM){
                    *erreur = MEMBRE_VIDE;
                    return arbrevide();
                }
                if ((tab[debut+2].lexem!=FONCTION) && (tab[debut+2].lexem!=REEL) && (tab[debut+2].lexem !=VARIABLE)){
                    *erreur = MEMBRE_VIDE;
                    return arbrevide();
                }
                else{
                    Node* FG = operateur(tab, debut + 1, fin - 1, erreur);
                    Node* arbre = (Node*)malloc(sizeof(Node));
                    *arbre = (Node){
                        .pjeton_preced = FG,
                        .pjeton_suiv = NULL,
                        .jeton = tab[debut]
                    };
                    return arbre;
                }
                break;
            case PAR_FERM:
                *erreur = PARENTHESE_FERMEE_1_ER_JETON;
                return arbrevide(); 
                break;
            case REEL:
                if (fin - debut == 0){
                    Node* arbre = (Node*)malloc(sizeof(Node));
                    *arbre = (Node){
                        .pjeton_preced = NULL,
                        .pjeton_suiv = NULL,
                        .jeton = tab[debut]
                    };
                    return arbre;
                }
                else{
                    *erreur = PROBLEME_APRES_REEL;
                    return arbrevide();
                }
                break;
            default:
                break;
        } 
    }
    return arbrevide();
}

int minIndice(typejeton *tab,  int debut, int fin, typeerreur *erreur){
    int depth = 0;
    int minOperatorIndex = -1;
    int minOperatorPriority = TAILLE_MAX;

    for (int i = debut; i <= fin; i++) {
        switch (tab[i].lexem) {
            case OPERATEUR:
                if (depth == 0 && opPriorite[tab[i].valeur.operateur] < minOperatorPriority) {
                    minOperatorIndex = i;
                    minOperatorPriority = opPriorite[tab[i].valeur.operateur];
                }
                break;
            case PAR_OUV:
                depth++;
                break;
            case PAR_FERM:
                depth--;
                break;
            default:
                break;
        }
    }

    if (depth != 0) {
        *erreur = PROBLEMES_NOMBRE_PARENTHESES;
        return -1;
    }

    return minOperatorIndex;
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
    for (int i = debut; i <= fin; i++){
        if (tab[i].lexem == PAR_OUV){
            nbouv = nbouv + 1;
        }
        if(tab[i].lexem == PAR_FERM){
            nbfer = nbfer + 1;
        }
    }
    return nbouv == nbfer;
}

Node* Syntaxique(typejeton *tab, typeerreur *erreur){
    int fin = calculTaille(tab);
    Node* arbrePlein = (Node*)malloc(sizeof(Node));
    arbrePlein = operateur( tab, 0, fin, erreur);
    return arbrePlein;
}
