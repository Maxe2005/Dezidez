#include "syntaxe.h"

Node* createEmptyNode() {
    // Allocation et initialisation d'un nœud vide
    Node* arbre = (Node*)malloc(sizeof(Node));
    *arbre = (Node){
        .pjeton_preced = NULL,
        .pjeton_suiv = NULL,
        .jeton = {.lexem = FIN, .valeur = {0}}
    };
    return arbre;
}

Node* buildExpressionTree(typejeton *tab, int debut, int fin, typeerreur *erreur) {
    // Affichage de débogage si activé
    if (syntaxeVerbose >= 2) afficher_liste_jetons(tab, debut, fin);
    
    // Trouve l'opérateur de plus faible priorité dans la plage donnée
    int indOpPrincipal = findLowestPriorityOperator(tab, debut, fin, erreur);
    if (syntaxeVerbose >= 10) printf("indOpPrincipal=%d\n", indOpPrincipal);
    
    // Si une erreur est survenue lors de la recherche de l'opérateur
    if (*erreur != 0) {
        return createEmptyNode();
    }
    
    // Si la plage est vide (début > fin), c'est une erreur
    if (debut > fin) {
        *erreur = MEMBRE_VIDE;
        return createEmptyNode();
    }
    
    // Cas où on a trouvé un opérateur principal
    if (indOpPrincipal != -1) {
        // Construction récursive des sous-arbres gauche et droit
        Node* fg = buildExpressionTree(tab, debut, indOpPrincipal - 1, erreur);
        Node* fd = buildExpressionTree(tab, indOpPrincipal + 1, fin, erreur);
        
        // Création du nœud pour l'opérateur avec ses sous-arbres
        Node* arbre = (Node*)malloc(sizeof(Node));
        *arbre = (Node){
            .pjeton_preced = fg,
            .pjeton_suiv = fd,
            .jeton = tab[indOpPrincipal]
        };
        return arbre;
    }
    // Cas où il n'y a pas d'opérateur principal (expression simple ou entre parenthèses)
    else {
        switch (tab[debut].lexem) {
            case FONCTION:
                // Vérification de la syntaxe pour les fonctions
                if (tab[debut + 1].lexem != PAR_OUV) {
                    if (syntaxeVerbose >= 10) printf("PROBLEME_PARENTHESES_FONCTIONS dans fonctions tab[debut + 1].lexem != PAR_OUV\n");
                    *erreur = PROBLEME_PARENTHESES_FONCTIONS;
                    return createEmptyNode();
                }
                if ((tab[debut+2].lexem!=FONCTION) && (tab[debut+2].lexem!=REEL) && (tab[debut+2].lexem !=VARIABLE)) {
                    if (syntaxeVerbose >= 10) printf("MEMBRE VIDE dans fonctions (tab[debut+2].lexem!=FONCTION) && (tab[debut+2].lexem!=REEL) && (tab[debut+2].lexem !=VARIABLE)\n");
                    *erreur = MEMBRE_VIDE;
                    return createEmptyNode();
                }

                // Création de l'arbre pour une fonction
                Node* fg = buildExpressionTree(tab, debut + 2, fin - 1, erreur);
                Node* arbre = (Node*)malloc(sizeof(Node));
                *arbre = (Node){
                    .pjeton_preced = fg,
                    .pjeton_suiv = NULL,
                    .jeton = tab[debut]   
                }; 
                return arbre;
                break;
                
            case PAR_OUV:
                // Vérification de la syntaxe pour les expressions entre parenthèses
                if (tab[fin].lexem != PAR_FERM) {
                    if (syntaxeVerbose >= 10) printf("MEMBRE_VIDE dans PAR_OUV tab[fin].lexem != PAR_FERM\n");
                    *erreur = MEMBRE_VIDE;
                    return createEmptyNode();
                }
                if ((tab[debut+1].lexem!=FONCTION) && (tab[debut+1].lexem!=REEL) && (tab[debut+1].lexem !=VARIABLE)) {
                    if (syntaxeVerbose >= 10) printf("MEMBRE_VIDE dans PAR_OUV (tab[debut+1].lexem!=FONCTION) && (tab[debut+1].lexem!=REEL) && (tab[debut+1].lexem !=VARIABLE)\n");
                    *erreur = MEMBRE_VIDE;
                    return createEmptyNode();
                }
                else {
                    // Pour une expression entre parenthèses, on analyse le contenu sans les parenthèses
                    return buildExpressionTree(tab, debut + 1, fin - 1, erreur);
                }
                break;
                
            case PAR_FERM:
                // Erreur: parenthèse fermante en premier jeton
                if (syntaxeVerbose >= 10) printf("PARENTHESE_FERMEE_1_ER_JETON dans PAR_FERM\n");
                *erreur = PARENTHESE_FERMEE_1_ER_JETON;
                return createEmptyNode(); 
                break;
                
            case REEL:
            case VARIABLE:
                // Cas des valeurs simples (réel ou variable)
                if (fin - debut == 0) {
                    // Si c'est le seul jeton, création d'un nœud feuille
                    Node* arbre = (Node*)malloc(sizeof(Node));
                    *arbre = (Node){
                        .pjeton_preced = NULL,
                        .pjeton_suiv = NULL,
                        .jeton = tab[debut]
                    };
                    return arbre;
                }
                else {
                    // Erreur: il y a quelque chose après un réel ou une variable
                    *erreur = PROBLEME_APRES_REEL_OU_VARIABLE;
                    return createEmptyNode();
                }
                break;
                
            default:
                break;
        } 
    }
    return createEmptyNode();
}

int findLowestPriorityOperator(typejeton *tab, int debut, int fin, typeerreur *erreur) {
    int profondeurParentheses = 0;
    int indiceOperateurMinimal = -1;
    int prioriteOperateurMinimal = TAILLE_MAX;

    // Parcours les jetons pour trouver l'opérateur de plus faible priorité
    for (int i = debut; i <= fin; i++) {
        switch (tab[i].lexem) {
            case OPERATEUR:
                // On ne considère que les opérateurs au niveau 0 des parenthèses
                if (profondeurParentheses == 0 && opPriorite[tab[i].valeur.operateur] < prioriteOperateurMinimal) {
                    indiceOperateurMinimal = i;
                    prioriteOperateurMinimal = opPriorite[tab[i].valeur.operateur];
                }
                break;
            case PAR_OUV:
                // Entrée dans un niveau supplémentaire de parenthèses
                profondeurParentheses++;
                break;
            case PAR_FERM:
                // Sortie d'un niveau de parenthèses
                profondeurParentheses--;
                break;
            default:
                break;
        }
    }

    // Vérification de l'équilibre des parenthèses
    if (profondeurParentheses != 0) {
        *erreur = PROBLEMES_NOMBRE_PARENTHESES;
        return -1;
    }

    return indiceOperateurMinimal;
}

int findExpressionLength(typejeton *tab) {
    // Recherche du jeton FIN dans le tableau
    for (int i = 0; i < TAILLE_MAX; i++) {
        if (tab[i].lexem == FIN) {
            return (i - 1);  // Retourne l'indice du dernier jeton avant FIN
        }
    }
    return -1;  // Retourne -1 si aucun jeton FIN n'est trouvé
}

bool checkParenthesesBalance(int debut, int fin, typejeton *tab) {
    int nombreParenthesesOuvrantes = 0;
    int nombreParenthesesFermantes = 0;
    
    // Compte les parenthèses ouvrantes et fermantes
    for (int i = debut; i <= fin; i++) {
        if (tab[i].lexem == PAR_OUV) {
            nombreParenthesesOuvrantes++;
        }
        if (tab[i].lexem == PAR_FERM) {
            nombreParenthesesFermantes++;
        }
    }
    
    // Les parenthèses sont équilibrées si le nombre est identique
    return nombreParenthesesOuvrantes == nombreParenthesesFermantes;
}

Node* buildSyntaxTree(typejeton *tab, typeerreur *erreur) {
    // Détermination de la fin effective de l'expression
    int fin = findExpressionLength(tab);
    if (syntaxeVerbose >= 10) printf("findExpressionLength fin=%d\n", fin);
    
    // Vérification de la présence du jeton FIN
    if (fin == -1) {
        *erreur = ABSENCE_FIN;
        return createEmptyNode();
    }
    
    // Construction de l'arbre d'expression
    Node* arbreSyntaxique = (Node*)malloc(sizeof(Node));
    arbreSyntaxique = buildExpressionTree(tab, 0, fin, erreur);
    return arbreSyntaxique;
}