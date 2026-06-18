# Dezidez

Le projet consiste en la réalisation d'un grapheur d'expressions fonctionnelles, c'est à dire d'un logiciel destiné à représenter graphiquement des expressions du type: sin(x*abs(x))+2 , exp(x+(6*log(x+1))) etc...

## Avec docker

### 1. Autoriser l'accès local au serveur X (à refaire à chaque session si besoin)

```bash
xhost +local:root
```

### 2. Build l'image Docker

```bash
docker compose build
```

### 3. Lancer l'application

```bash
docker compose up
```

## Ligne de compilation Windows

gcc -o main _.c Gestion_3D/_.c SDL2_gfx/\*.c cJSON/cJSON.C -Iinclude -Llib -lmingw32 -lSDL2main -lSDL2 -lSDL2_ttf -lSDL2_mixer -lSDL2_image

## Linux

### Ligne de compilation Ubuntu

```bash
gcc *.c Gestion_3D/*.c cJSON/cJSON.c -o prog -lSDL2 -lSDL2main -lSDL2_ttf -lm -lSDL2_mixer -lSDL2_gfx -lSDL2_image
```

### Pour installer toutes les dépendances

```bash
sudo apt update
sudo apt install $(grep -v '^#' requirements.txt)
```
