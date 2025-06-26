#include "invaders.h"
#include <stdio.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>

ALLEGRO_BITMAP* ship_png = al_load_bitmap("spaceship.png");
if(!ship_png)
{
	printf("Nao foi possivel carregar a imagem da nave\n");
	return 1;
}


typedef struct Ship{
	float x;
} Ship;

void init_ship(Ship *ships){

	ships->x = SCREEN_W/2;
}


destroy_ship(Ship *ships){
	al_destroy_bitmap(ship_png);
}