#include <stdio.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>

const int SCREEN_W = 960;
const int SCREEN_H = 540;
const int WORLD_H = 60;

const int SHIP_W = 60;
const int SHIP_H = 60;

typedef struct Ship{
float x;
}

void init_ship(*Ship);

destroy_imgs();