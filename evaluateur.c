# include "evaluateur.h"

// arbre de texte format [racine[[arbre_gauche][arbre_droite]]]    [sin[[+[[3][4]][]]]  sin(3+4)









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

