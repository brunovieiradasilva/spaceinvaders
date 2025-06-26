ALLEGRO_VERSION=5.0.10
MINGW_VERSION=4.7.0
FOLDER=C:

FOLDER_NAME=\allegro-$(ALLEGRO_VERSION)-mingw-$(MINGW_VERSION)
PATH_ALLEGRO=$(FOLDER)$(FOLDER_NAME)
LIB_ALLEGRO=\lib\liballegro-$(ALLEGRO_VERSION)-monolith-mt.a
INCLUDE_ALLEGRO=\include

all: main.exe	

main.exe: main.o
	gcc -o main.exe main.o $(PATH_ALLEGRO)$(LIB_ALLEGRO)

main.o: main.c
	gcc -I $(PATH_ALLEGRO)$(INCLUDE_ALLEGRO) libs\invaders.c -c main.c	

invaders.exe: invaders.o
	gcc -o invaders.exe invaders.o $(PATH_ALLEGRO)$(LIB_ALLEGRO)

invaders.o: invaders.c
	gcc -I $(PATH_ALLEGRO)$(INCLUDE_ALLEGRO) libs\invaders.c -c invaders.c	
clean:
	del main.o 
	del main.exe
	del invaders.o 
	del invaders.exe
