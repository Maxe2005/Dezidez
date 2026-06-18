FROM ubuntu:22.04

# Éviter les invites interactives lors de l'installation des paquets
ENV DEBIAN_FRONTEND=noninteractive

# Installation des outils de build de base
RUN apt-get update && apt-get install -y \
    build-essential \
    gcc \
    pkg-config \
    && rm -rf /var/lib/apt/lists/*

# Copie du projet dans le conteneur
WORKDIR /app
COPY . .

# Installation des dépendances à partir de ton requirements.txt
# (On nettoie les commentaires et les lignes vides au cas où)
RUN apt-get update && \
    apt-get install -y $(cat requirements.txt | grep -v '^#' | xargs) && \
    rm -rf /var/lib/apt/lists/*

# Compilation de l'application
RUN gcc *.c Gestion_3D/*.c cJSON/cJSON.c -o prog -lSDL2 -lSDL2main -lSDL2_ttf -lm -lSDL2_mixer -lSDL2_gfx -lSDL2_image

# Commande par défaut au lancement du conteneur
CMD ["./prog"]
