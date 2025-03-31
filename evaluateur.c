# include "evaluateur.h"

Node creation_arbre(typejeton racine, Node* precedent, Node* suivant){
    Node arbre;
    arbre.jeton=racine;
    arbre.pjeton_preced=precedent;
    arbre.pjeton_suiv=suivant;
    return arbre;
}


/* décodage des arbre */
float evaluateur(Node *racine,float valeur_x, float valeur_y ,int *code_erreur){
    
    if (*code_erreur == 0){ //vérification si il y a une erreur
        switch (racine->jeton.lexem)
        {
        case REEL: //décodage réels
            return racine->jeton.valeur.reel;
            break;
        case VARIABLE:
            if (racine->jeton.valeur.variable=='x'){
                return valeur_x;
        

            }
            if (racine->jeton.valeur.variable=='y'){
                return valeur_y;
        

            }
            break;
        case OPERATEUR: //décodage opérateur
            if (racine->jeton.valeur.operateur==PLUS){ // appel récursif pour calculer la somme
                return evaluateur(racine->pjeton_preced, valeur_x,  valeur_y,  code_erreur)+evaluateur(racine->pjeton_suiv, valeur_x,  valeur_y,  code_erreur);
                
            }
            if (racine->jeton.valeur.operateur==MOINS){ // appel récursif pour calculer la soustraction
                return evaluateur(racine->pjeton_preced, valeur_x,  valeur_y,  code_erreur)-evaluateur(racine->pjeton_suiv, valeur_x,  valeur_y,  code_erreur);
                
            }
            if (racine->jeton.valeur.operateur==DIV){ // appel récursif pour calculer la division
                
                float nominateur=evaluateur(racine->pjeton_preced, valeur_x,  valeur_y,  code_erreur); 
                float denominateur=evaluateur(racine->pjeton_suiv, valeur_x,  valeur_y,  code_erreur);
                
                if(denominateur == 0){ 
                    *code_erreur=DIVISION_PAR_ZERO; //Potentielle erreur 311 
                    return 0;
                }
                else{
                    return (float)(nominateur/denominateur);
                }

                
            }
            if (racine->jeton.valeur.operateur==FOIS){ // appel récursif pour calculer la multiplication
                return evaluateur(racine->pjeton_preced, valeur_x,  valeur_y,  code_erreur)*evaluateur(racine->pjeton_suiv, valeur_x,  valeur_y,  code_erreur);
                
            }
            if (racine->jeton.valeur.operateur==PUIS){ // appel récursif pour calculer la multiplication
                float valeur=evaluateur(racine->pjeton_preced, valeur_x,  valeur_y,  code_erreur);
                float exposant=evaluateur(racine->pjeton_suiv, valeur_x,  valeur_y,  code_erreur);
                
                if (valeur == 0 && exposant == 0){ //Potentielle erreur 312
                    *code_erreur=ERREUR_ZERO_PUIS_ZERO;
                    return 0;
                }
                if (valeur == 0 && exposant < 0){ //Potentielle erreur 313
                    *code_erreur=ERREUR_ZERRO_PUIS_NEGATIVE; 
                    return 0;
                }
                else{
                return powf(valeur,exposant);
                }
            }

            break;
        case FONCTION: //décodage fonction
            return calculer_fonction(racine->jeton.valeur.fonction,evaluateur(racine->pjeton_preced, valeur_x,  valeur_y,  code_erreur),code_erreur);
            break;
        
        
        default:
            return 0;
            break;
        }

    } 
    
    else { //en cas d'erreur
        return 0;
       
    }
}

/* Calcul des fonctions*/
float calculer_fonction(typefonction fonction, float variable,int *code_erreur){
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
        if (variable < 0){  //potentielle erreur 302
            *code_erreur=LOG_NEGATIF;
            return 0;
        } else {
            if (variable == 0){ //potentielle erreur 303
                *code_erreur = LOG_ZERO;
                return 0;
            } else {    //si pas d'erreur
                return logf(variable);
            }
        }
        break;

    case TAN :
        if (fmodf(variable,M_PI/2)==0){ //potentielle erreur 304
            *code_erreur = TAN_PI_SUR_2;
            return 0;
        } else {    //si pas d'erreur
            return tanf(variable);
        }
        break;

    case EXP :
        return expf(variable);
        break;

    case ENTIER:
        return floorf(variable);
        break;

    case SQRT :
        if (variable < 0){ //potentielle erreur 301
            *code_erreur = RACINE_NEGATIVE;
            return 0;
        }
        else{   //si pas d'erreur
            return sqrtf(variable);
        }
        break;

    case SINC :
        if (variable == 0){ //potentielle erreur 305
            *code_erreur = SINC_DIVISIION_PAR_ZERO;
            return 0;
        } else {    //si pas d'erreur
            return (float)((sin(variable))/(variable)); // a préciser non normalisé dans la doc
        }
        break;

    case VAL_NEG :
        return -(variable);
        break;

    default:
        return 0;
        break;
    }
}
