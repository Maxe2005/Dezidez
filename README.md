# Dezidez
Le projet consiste en la réalisation d'un grapheur d'expressions fonctionnelles, c'est à dire d'un logiciel destiné à représenter graphiquement des expressions du type: sin(x*abs(x))+2 , exp(x+(6*log(x+1))) etc...

### Ligne de compilation Ubuntu
gcc *.c cJSON/cJSON.c -o prog -lSDL2 -lSDL2main -lSDL2_ttf -lm -lSDL2_mixer -lSDL2_gfx -lSDL2_image
