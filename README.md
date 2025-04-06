# Dezidez
Le projet consiste en la réalisation d'un grapheur d'expressions fonctionnelles, c'est à dire d'un logiciel destiné à représenter graphiquement des expressions du type: sin(x*abs(x))+2 , exp(x+(6*log(x+1))) etc...


## Syntaxique

Prends en paramètre une liste de jeton et renvoit un arbre.

Renvoit une erreur dans les cas suivant :
- PROBLEME_PARENTHESES_FONCTIONS : manque une parenthèse après une fonction ou si la parenthèse ne se ferme pas
- MEMBRE_VIDE : si un opérateur n'est pas entouré d'objets traitables, si des parenthèses sont vides
- PARENTHESE_FERMEE_1_ER_JETON : si une parenthèse est fermée sans avoir été ouverte
- PROBLEMES_NOMBRE_PARENTHESES : si le nombre de parenthèses ouvrante est différent du nombre de parenthèses fermentes
- PROBLEME_APRES_REEL_OU_VARIABLE : Supposé impossible
- ABSENCE_FIN : si le jeton "FIN" est manquant


Plusieurs fonctions d'affichage sont disponible pour debugger