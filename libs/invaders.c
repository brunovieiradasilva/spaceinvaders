#include "invaders.h"
#include <stdio.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>

ALLEGRO_BITMAP *ship_png = NULL;

void init_ship(Ship *ships)
{
    ship_png = al_load_bitmap("imgs/spaceship.png");
    if (!ship_png)
    {
        printf("Nao foi possivel carregar a imagem da nave\n");
        return;
    }

    ships->x = SCREEN_W / 2;
}

void draw_ship(Ship *ships)
{
    if (ship_png)
    {
        al_draw_bitmap(ship_png, ships->x, SHIP_Y, 0);
    }
    else
    {
        printf("Imagem da nave nao carregada\n");
    }
}
void destroy_ship(Ship *ships)
{
    if (ship_png)
    {
        al_destroy_bitmap(ship_png);
        ship_png = NULL;
    }
}
void ship_atack()
{
    // Implementar a lógica de ataque da nave
    printf("Ataque disparado!\n");
}

void ship_keyboard(Ship *s, int t)
{
    if (t == ALLEGRO_KEY_A)
    {
        s->x -= SHIP_SPEED;
        if (s->x < 0)
            s->x = 0;
    }
    else if (t == ALLEGRO_KEY_D)
    {
        s->x += SHIP_SPEED;
        if (s->x > SCREEN_W - SHIP_W)
            s->x = SCREEN_W - SHIP_W;
    }
    else if (t == ALLEGRO_KEY_SPACE)
    {
        ship_atack();
    }
}