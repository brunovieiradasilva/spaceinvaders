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

#define SHIP_W 64
#define SHIP_H 64
#define SHIP_Y (SCREEN_H - 60 - SHIP_H)
#define SHIP_SPEED 30

#define ALIEN_W 72
#define ALIEN_H 72
#define ALIEN_Y NULL
#define ALIEN_SPEEDX 25
#define ALIEN_SPEEDY ALIEN_H

#define SHOT_W 8
#define SHOT_H 16
#define SHOT_SPEED 30

typedef struct Ship
{
    float x;
    int direction; // 0:parked , 1: left, 2: right
} Ship;

typedef struct Alien
{
    float x;
    float y;
    int direction; // 0:parked , 1: left, 2: right
    int alive; // 0: inactive, 1: active
} Alien;

typedef struct Shot
{
    float x;
    float y;
    float speed;
    int active; // 0: inactive, 1: active 
    int type;   // 0: alien shot, 1: ship shot
} Shot;

typedef struct Button
{
    int x;
    int y;
    int w;
    int h;
    char *text;
} Button;

bool collide(int ax1, int ay1, int ax2, int ay2, int bx1, int by1, int bx2, int by2);

bool collide_btn(int ax, int ay, int bx1, int by1, int bx2, int by2);

int randon(int lo, int hi);

void init_alien(Alien *alien);

void draw_alien(Alien *alien);

int alien_move(Alien *alien);

void alien_atack( Shot *shot, Alien *invasion[4][6]);

void init_alien_shot(Shot *shot, Alien *invasion[4][6]);

void destroy_alien(Alien *alien);

void init_ship(Ship *ships);

void draw_ship(Ship *ships);

void ship_atack(Ship *ship, Shot *shot);

void ship_keyboard(Ship *ship,Shot *shot, int t, int type);

void ship_move(Ship *s);

void destroy_ship(Ship *ships);

void init_alien_invasion(Alien *invasion[4][6]);

void draw_alien_invasion(Alien *invasion[4][6]);

int move_alien_invasion(Alien *invasion[4][6], int dificulty);

void destroy_alien_invasion(Alien *invasion[4][6]);

void init_shots_images();

void init_shot(Shot *shot,Ship *ship, int type);

void draw_shot(Shot *shot);

void move_shot(Shot *shot, int dificulty);

void destroy_shot(Shot *shot);

void destroy_space_invaders();