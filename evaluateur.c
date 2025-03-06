# include "evaluateur.h"

// arbre de texte format [racine[[arbre_gauche][arbre_droite]]]    [sin[[+[[3][4]][]]]  sin(3+4)










float decodage_arbre(Node *racine){
    if (racine->jeton.lexem == REEL){ // retourne la valeur du reel si c'est un reel (cas final)
        return racine->jeton.valeur.reel; 

    }

    if (racine->jeton.lexem == OPERATEUR){ // si l'élément est un opérateur, on effectue l'opération demandée



        if (racine->jeton.valeur.operateur==PLUS){ // appel récursif pour calculer la somme
            return decodage_arbre(racine->pjeton_preced)+decodage_arbre(racine->pjeton_suiv);
        }

        if (racine->jeton.valeur.operateur==MOINS){ // appel récursif pour calculer la soustraction
            return decodage_arbre(racine->pjeton_preced)-decodage_arbre(racine->pjeton_suiv);
        }
        if (racine->jeton.valeur.operateur==DIV){ // appel récursif pour calculer la division
            return (float)(decodage_arbre(racine->pjeton_preced)/decodage_arbre(racine->pjeton_suiv));
        }

        if (racine->jeton.valeur.operateur==FOIS){ // appel récursif pour calculer la multiplication
            return decodage_arbre(racine->pjeton_preced)*decodage_arbre(racine->pjeton_suiv);
        }

    }


    if (racine->jeton.lexem == FONCTION){ // si l'élément est une fonction on prend sont élément suivant afin de le donner à la fonction de calcul de fonction
        calculer_fonction(racine->jeton.valeur.fonction,decodage_arbre(racine->pjeton_suiv));
    }

    else {
        erreur_evaluateur();
        return 0;
        

    }

}








erreur_evaluateur(){
    printf('erreur 3');
}





float calculer_fonction(typefonction fonction, float variable){
    float resultat=0;
    switch (fonction)
    {
    case ABS :
        resultat= fabsf(variable);
        break;
    case SIN :
        resultat= sinf(variable);
        break;
    case COS :
        resultat= cosf(variable);
        break;
    case LOG :
        resultat= logf(variable);
        break;
    case TAN :
        resultat= tanf(variable);
        break;
    case EXP :
        resultat= expf(variable);
        break;
    case ENTIER:
        resultat= floorf(variable);
        break;
    case SQRT :
        resultat= sqrtf(variable);
        break;
    case SINC :
        resultat= (float)((sin(M_PI*variable))/(M_PI*variable));
        break;
    case VAL_NEG :
        resultat= -(variable);
        break;
    


    default:
        return 0;
        erreur_evaluateur();
        break;
    }
    }

