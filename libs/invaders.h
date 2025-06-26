#include <stdio.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>

const int SCREEN_W = 1080;
const int SCREEN_H = 720;
const int WORLD_H = 60;

const int SHIP_W = 60;
const int SHIP_H = 60;

typedef struct Ship{
float x;
}Ship;

void init_ship(Ship *);

void destroy_imgs();