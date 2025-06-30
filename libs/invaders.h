#include <stdio.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>

#define SCREEN_W 1080
#define SCREEN_H 720
#define WORLD_H 60

#define SHIP_W 100
#define SHIP_H 50
#define SHIP_Y (SCREEN_H - 60 - SHIP_H)
#define SHIP_SPEED 70

typedef struct Ship{
float x;
}Ship;

void init_ship(Ship *ships);

void draw_ship(Ship *ships);

typedef struct Button{
    int x;
    int y;
    int w;
    int h;
    char *text;
}Button;

void ship_atack();

void ship_keyboard(Ship *s, int t);
