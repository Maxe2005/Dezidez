# Dezidez
Le projet consiste en la réalisation d'un grapheur d'expressions fonctionnelles, c'est à dire d'un logiciel destiné à représenter graphiquement des expressions du type: sin(x*abs(x))+2 , exp(x+(6*log(x+1))) etc...


### Ligne de compilation Windows

gcc -o main *.c Gestion_3D/*.c SDL2_gfx/*.c cJSON/cJSON.C -Iinclude -Llib -lmingw32 -lSDL2main -lSDL2 -lSDL2_ttf -lSDL2_mixer -lSDL2_image


### Ligne de compilation Ubuntu
gcc *.c Gestion_3D/*.c cJSON/cJSON.c -o prog -lSDL2 -lSDL2main -lSDL2_ttf -lm -lSDL2_mixer -lSDL2_gfx -lSDL2_image
