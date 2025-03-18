# include "evaluateur.h"

void remplacer_variable(Node *racine,float valeur_x, float valeur_y ){
    if(racine->jeton.lexem==VARIABLE){
        switch (racine->jeton.valeur.variable)
        {
        case 'x':
            racine->jeton.lexem=REEL;
            racine->jeton.valeur.reel=valeur_x;
            break;
        
        case 'y':
            racine->jeton.lexem=REEL;
            racine->jeton.valeur.reel=valeur_y;
            break;
        
        default:

            break;
        }
        
        
    }
    
}


float decodage_arbre(Node *racine){
    switch (racine->jeton.lexem)
    {
    case REEL:
        return racine->jeton.valeur.reel;
        break;
    case OPERATEUR:
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
        if (racine->jeton.valeur.operateur==PUIS){ // appel récursif pour calculer la multiplication
            return powf(decodage_arbre(racine->pjeton_preced),decodage_arbre(racine->pjeton_suiv));
            
        }

        break;
    case FONCTION:
        return calculer_fonction(racine->jeton.valeur.fonction,decodage_arbre(racine->pjeton_preced));
        break;
       
    
    default:
        return 7;
        break;
    }

    
}
















float calculer_fonction(typefonction fonction, float variable){
    switch (fonction){
    case ABS :
        return fabsf(variable);
        break;
    case SIN :
        return sinf(variable);
        break;
    case COS :
        return cosf(variable);
        break;
    case LOG :
        return logf(variable);
        break;
    case TAN :
        return tanf(variable);
        break;
    case EXP :
        return expf(variable);
        break;
    case ENTIER:
        return floorf(variable);
        break;
    case SQRT :
        return sqrtf(variable);
        break;
    case SINC :
        return (float)((sin(M_PI*variable))/(M_PI*variable));
        break;
    case VAL_NEG :
        return -(variable);
        break;
    


    default:
        return 0;
        break;
    }
    }

