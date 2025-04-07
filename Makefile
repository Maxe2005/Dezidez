# Makefile pour compilation cross-platform (Windows/Ubuntu/macOS)

# Compilateur
CC = gcc

# Sources
SRC = *.c Gestion_3D/*.c
SRC_JSON = cJSON/cJSON.c
SRC_SDL2_GFX = SDL2_gfx/*.c

# Vérification du système d'exploitation
ifeq ($(OS),Windows_NT)
	SYSTEM = Windows
else
	UNAME_S := $(shell uname -s)
	ifeq ($(UNAME_S),Linux)
		SYSTEM = Linux
	else ifeq ($(UNAME_S),Darwin)
		SYSTEM = macOS
	else
		SYSTEM = Unknown
	endif
endif

# Exécutable
ifeq ($(SYSTEM),Windows)
	TARGET = main.exe
	RUN_CMD = $(TARGET)
else
	TARGET = prog
	RUN_CMD = ./$(TARGET)
endif

# Flags de compilation
CFLAGS = -Wall -Wextra

# Flags de liaison et sources selon le système
ifeq ($(SYSTEM),Windows)
	# Windows
	INCLUDES = -Iinclude
	LIBS = -Llib -lmingw32 -lSDL2main -lSDL2 -lSDL2_ttf -lSDL2_mixer -lSDL2_image
	SRC_ALL = $(SRC) $(SRC_JSON:.c=.C) $(SRC_SDL2_GFX)
else ifeq ($(SYSTEM),Linux)
	# Linux (Ubuntu)
	INCLUDES = 
	LIBS = -lSDL2 -lSDL2main -lSDL2_ttf -lm -lSDL2_mixer -lSDL2_gfx -lSDL2_image
	SRC_ALL = $(SRC) $(SRC_JSON)
else ifeq ($(SYSTEM),macOS)
	# macOS
	INCLUDES = -I/usr/local/include -I/opt/homebrew/include
	LIBS = -L/usr/local/lib -L/opt/homebrew/lib -lSDL2 -lSDL2main -lSDL2_ttf -lm -lSDL2_mixer -lSDL2_gfx -lSDL2_image
	SRC_ALL = $(SRC) $(SRC_JSON)
else
	$(error Système non supporté)
endif

# Vérification des dépendances
DEPS_CHECK = deps_check
.PHONY: all clean run $(DEPS_CHECK) help

all: $(DEPS_CHECK) $(TARGET)

# Vérification des dépendances
$(DEPS_CHECK):
	@echo "Vérification des dépendances pour $(SYSTEM)..."
ifeq ($(SYSTEM),Windows)
	@where gcc > nul 2>&1 || (echo "Erreur: GCC non trouvé" && exit 1)
	@if not exist include (echo "Erreur: Dossier include non trouvé" && exit 1)
	@if not exist lib (echo "Erreur: Dossier lib non trouvé" && exit 1)
else ifeq ($(SYSTEM),Linux)
	@which gcc > /dev/null 2>&1 || (echo "Erreur: GCC non trouvé" && exit 1)
	@pkg-config --exists sdl2 || (echo "Erreur: SDL2 non trouvée" && exit 1)
	@pkg-config --exists SDL2_ttf || (echo "Erreur: SDL2_ttf non trouvée" && exit 1)
	@pkg-config --exists SDL2_mixer || (echo "Erreur: SDL2_mixer non trouvée" && exit 1)
	@pkg-config --exists SDL2_image || (echo "Erreur: SDL2_image non trouvée" && exit 1)
else ifeq ($(SYSTEM),macOS)
	@which gcc > /dev/null 2>&1 || (echo "Erreur: GCC non trouvé" && exit 1)
	@[ -d /usr/local/include/SDL2 -o -d /opt/homebrew/include/SDL2 ] || (echo "Erreur: SDL2 non trouvée" && exit 1)
endif
	@echo "Toutes les dépendances sont présentes."

# Compilation
$(TARGET): 
ifeq ($(SYSTEM),Windows)
	$(CC) -o $(TARGET) $(SRC_ALL) $(INCLUDES) $(CFLAGS) $(LIBS)
else
	$(CC) $(CFLAGS) $(SRC_ALL) -o $(TARGET) $(INCLUDES) $(LIBS)
endif

# Lancer le programme
run: $(TARGET)
	@echo "Lancement de $(TARGET)..."
	$(RUN_CMD)

# Compilation et exécution en une seule commande
build_run: all run

# Nettoyage
clean:
ifeq ($(SYSTEM),Windows)
	-del $(TARGET)
else
	rm -f $(TARGET)
endif

help:
	@echo "Makefile pour compilation cross-platform"
	@echo "Plateformes supportées: Windows, Linux (Ubuntu), macOS"
	@echo "Commandes disponibles:"
	@echo "  make          - Compile le programme"
	@echo "  make run      - Lance le programme (compile d'abord si nécessaire)"
	@echo "  make build_run - Compile et lance le programme"
	@echo "  make clean    - Supprime l'exécutable"
	@echo "  make help     - Affiche cette aide"
