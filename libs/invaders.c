#include "invaders.h"
#include <stdio.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>

ALLEGRO_BITMAP *ship_png = NULL;

void init_ship(Ship *ships) {
    ship_png = al_load_bitmap("imgs/spaceship.png");
    if (!ship_png) {
        printf("Nao foi possivel carregar a imagem da nave\n");
        return;
    }

    ships->x = SCREEN_W / 2;
}

void destroy_ship(Ship *ships) {
    if (ship_png) {
        al_destroy_bitmap(ship_png);
        ship_png = NULL;
    }
}
