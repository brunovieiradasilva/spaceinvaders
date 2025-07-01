#include "invaders.h"
#include <stdio.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>

ALLEGRO_BITMAP *ship_png = NULL;
ALLEGRO_BITMAP *alien_png = NULL;
ALLEGRO_BITMAP *shot0_png = NULL;
ALLEGRO_BITMAP *shot1_png = NULL;
ALLEGRO_BITMAP *shot2_png = NULL;

bool collide(int ax1, int ay1, int ax2, int ay2, int bx1, int by1, int bx2, int by2)
{
    if (ax1 > bx2)
        return false;
    if (ax2 < bx1)
        return false;
    if (ay1 > by2)
        return false;
    if (ay2 < by1)
        return false;

    return true;
}

bool collide_btn(int ax, int ay, int bx1, int by1, int bx2, int by2)
{
    if (ax < bx1 || ax > bx2)
        return false;
    if (ay < by1 || ay > by2)
        return false;

    return true;
}

int randon(int lo, int hi)
{
    return lo + (rand() % (hi - lo + 1));
}

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

void ship_atack(Ship *ship, Shot *shot)
{
    if (shot->active)
    {
        printf("Ja existe um ataque disparado!\n");
        return;
    }
    else
    {
        shot->x = ship->x + SHIP_W / 2 - SHOT_W / 2; // Centraliza o tiro na nave
        shot->y = SHIP_Y - SHOT_H;                   // Posiciona o tiro acima da nave

        shot->active = 1; // Marca que o tiro foi disparado
        printf("Ataque disparado!\n");
    }
}

void ship_keyboard(Ship *ship, Shot *shot, int t, int type)
{
    if (type == 1)
    {
        ship->direction = 0; // parked
        return;
    }
    if (t == ALLEGRO_KEY_A || t == ALLEGRO_KEY_LEFT)
    {
        ship->direction = 1; // left
    }
    else if (t == ALLEGRO_KEY_D || t == ALLEGRO_KEY_RIGHT)
    {
        ship->direction = 2; // right
    }

    else if (t == ALLEGRO_KEY_W || t == ALLEGRO_KEY_UP)
    {
        ship_atack(ship, shot);
    }
}

void ship_move(Ship *s)
{
    if (s->direction == 1) // left
    {
        s->x -= SHIP_SPEED;
        if (s->x < 0)
            s->x = 0;
    }
    else if (s->direction == 2) // right
    {
        s->x += SHIP_SPEED;
        if (s->x > SCREEN_W - SHIP_W)
            s->x = SCREEN_W - SHIP_W;
    }
}

void init_alien(Alien *alien)
{
    alien_png = al_load_bitmap("imgs/alienship.png");
    if (!alien_png)
    {
        printf("Nao foi possivel carregar a imagem do alien\n");
        return;
    }

    alien->x = 0; // ou qualquer posição inicial desejada
    alien->y = 0;
    alien->alive = 1; // Marca o alien como ativo
}

void draw_alien(Alien *alien)
{
    if (alien->alive == 0) // Se o alien nao esta ativo, nao desenha
        return;
    if (alien_png)
    {
        al_draw_bitmap(alien_png, alien->x, alien->y, 0);
    }
    else
    {
        printf("Imagem do alien nao carregada\n");
    }
}

int alien_move(Alien *alien)
{
    if (alien->direction == 1) // left
    {
        alien->x -= ALIEN_SPEEDX;
        if (alien->x < 0)
        {
            alien->x = 0;
            alien->direction = 2;
            return 1; // Retorna 1 para indicar que o movimento vertical ocorreu
        }
    }
    else if (alien->direction == 2) // right
    {
        alien->x += ALIEN_SPEEDX;
        if (alien->x > SCREEN_W - ALIEN_W)
        {
            alien->x = SCREEN_W - ALIEN_W;
            alien->direction = 1;
            return 1; // Retorna 1 para indicar que o movimento vertical ocorreu
        }
    }
    return 0; // Retorna 0 para indicar que o movimento foi bem-sucedido
}

void destroy_alien(Alien *alien)
{
    if (alien_png)
    {
        al_destroy_bitmap(alien_png);
        alien_png = NULL;
    }
}

void init_alien_invasion(Alien *invasion[4][6])
{
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 6; j++)
        {
            invasion[i][j] = malloc(sizeof(Alien));      // Aloca memória para cada alien
            init_alien(invasion[i][j]);                  // Inicializa a imagem e as posições
            invasion[i][j]->x = j * (ALIEN_W + 60) + 50; // Espaçamento horizontal
            invasion[i][j]->y = i * (ALIEN_H + 30) + 2;  // Espaçamento vertical
            invasion[i][j]->direction = 2;               // Inicia todos os invasores estacionados
        }
    }
}

void draw_alien_invasion(Alien *invasion[4][6])
{
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 6; j++)
        {
            draw_alien(invasion[i][j]);
        }
    }
}
int move_alien_invasion(Alien *invasion[4][6], int dificulty)
{
    int need_move_down = 0;

    // 1. Verifica se algum alien vai bater na borda
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 6; j++)
        {
            Alien *a = invasion[i][j];

            if (dificulty == 1)
            {
                if (a->direction == 1 && a->x - (ALIEN_SPEEDX * 2) < 0 && a->alive == 1) // indo para a esquerda
                    need_move_down = 1;
                if (a->direction == 2 && a->x + (ALIEN_SPEEDX * 2) > SCREEN_W - ALIEN_W && a->alive == 1) // indo para a direita
                    need_move_down = 1;
            }
            else
            {
                if (a->direction == 1 && a->x - ALIEN_SPEEDX < 0 && a->alive == 1) // indo para a esquerda
                    need_move_down = 1;
                if (a->direction == 2 && a->x + ALIEN_SPEEDX > SCREEN_W - ALIEN_W && a->alive == 1) // indo para a direita
                    need_move_down = 1;
            }
        }
    }

    // 2. Se algum alien vai bater, move todos para baixo e inverte direção
    if (need_move_down)
    {
        for (int i = 0; i < 4; i++)
        {
            for (int j = 0; j < 6; j++)
            {
                Alien *a = invasion[i][j];
                // Inverte direção
                if (a->direction == 1)
                    a->direction = 2;
                else
                    a->direction = 1;
                // Move para baixo
                a->y += ALIEN_H;

                if (a->y > SCREEN_H - ALIEN_H && a->alive == 1)
                {
                    return 1; // Retorna 1 para indicar que os aliens chegaram ao fundo da tela
                }
            }
        }

        return 0;
    }
    else
    {
        // 3. Senão, move todos normalmente na horizontal
        for (int i = 0; i < 4; i++)
        {
            for (int j = 0; j < 6; j++)
            {
                alien_move(invasion[i][j]);
            }
        }
        return 0; // Retorna 0 para indicar que o movimento foi bem-sucedido
    }
}

void destroy_alien_invasion(Alien *invasion[4][6])
{
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 6; j++)
        {
            destroy_alien(invasion[i][j]);
            free(invasion[i][j]); // Libera a memória alocada para cada alien
        }
    }
}

void init_shots_images()
{
    shot0_png = al_load_bitmap("imgs/shot0.png");
    shot1_png = al_load_bitmap("imgs/shot1.png");
    shot2_png = al_load_bitmap("imgs/shot2.png");

    if (!shot0_png || !shot1_png || !shot2_png)
    {
        printf("Nao foi possivel carregar as imagens dos tiros\n");
    }
}

void init_shot(Shot *shot, Ship *ship, int type)
{
    shot->type = type;
    if (type == 0 || type == 1)
    {
        shot->x = -100;
        shot->y = -100;
    }
    else if (type == 2)
    {
        shot->x = -100;
        shot->y = -100;
    }

    shot->speed = SHOT_SPEED; // Define a velocidade do tiro
}

void draw_shot(Shot *shot)
{

    if (shot->type == 0 && shot0_png)
    {
        al_draw_scaled_bitmap(shot0_png, 0, 0, al_get_bitmap_width(shot0_png), al_get_bitmap_height(shot0_png), shot->x, shot->y, 50, 50, 0);
    }
    else if (shot->type == 1 && shot1_png)
    {
        al_draw_scaled_bitmap(shot1_png, 0, 0, al_get_bitmap_width(shot1_png), al_get_bitmap_height(shot1_png), shot->x, shot->y, 50, 50, 0);
    }
    else if (shot->type == 2 && shot2_png)
    {

        al_draw_scaled_bitmap(shot2_png, 0, 0, al_get_bitmap_width(shot2_png), al_get_bitmap_height(shot2_png), shot->x, shot->y, 50, 50, 0);
    }
    else
    {
        printf("Imagem do tiro do alien nao carregada\n");
    }
}
void move_shot(Shot *shot, int dificulty)
{
    if (shot->active)
    {
        if (dificulty == 1)
        {
            shot->y -= (shot->speed * 2);
        }
        else
        {
            shot->y -= shot->speed;
        }
    }
    else
    {
        shot->x = -100; // Marca como fora da tela
        shot->y = -100;
    }
    // Verifica se o tiro saiu da tela
    if (shot->y < 0 || shot->y > SCREEN_H)
    {
        shot->x = -100; // Marca como fora da tela
        shot->y = -100;
        shot->active = 0; // Reseta o estado do tiro
    }
}

void destroy_shot(Shot *shot)
{
    if (shot1_png)
    {
        al_destroy_bitmap(shot1_png);
        shot1_png = NULL;
    }
    if (shot2_png)
    {
        al_destroy_bitmap(shot2_png);
        shot2_png = NULL;
    }
}

void destroy_shot_images()
{
    if (shot0_png)
    {
        al_destroy_bitmap(shot0_png);
        shot0_png = NULL;
    }
    if (shot1_png)
    {
        al_destroy_bitmap(shot1_png);
        shot1_png = NULL;
    }
    if (shot2_png)
    {
        al_destroy_bitmap(shot2_png);
        shot2_png = NULL;
    }
}

void shot_colide(Shot *shot, Alien *alien)
{
    shot->active = 0; // Reseta o estado do tiro
    shot->x = -100;   // Marca o tiro como fora da tela
    shot->y = -100;
    alien->x = -100; // Marca o alien como destruído
    alien->y = -100;
    printf("Tiro colidiu com o alien!\n");
}

void init_alien_shot(Shot *shot, Alien *invasion[4][6])
{

    shot->x = -100;
    shot->y = -100;
    shot->speed = -SHOT_SPEED; // Define a velocidade do tiro
    shot->active = 1;          // Marca o tiro como ativo
    shot->type = 2;            // Marca o tiro como do alien
}

void alien_atack(Shot *shot, Alien *invasion[4][6])
{
    Alien *a;
    do
    {
        a = invasion[randon(0, 3)][randon(0, 5)];
    } while (a->alive == 0);

    shot->x = a->x + ALIEN_W / 2 - SHOT_W / 2; // Centraliza o tiro na nave
    shot->y = a->y - SHOT_H;                   // Posiciona o tiro acima da nave

    if (shot->active)
    {
        printf("Ja existe um ataque disparado!\n");
        return;
    }
    else
    {

        shot->active = 1; // Marca que o tiro foi disparado
        printf("Ataque disparado!\n");
    }
}

void destroy_space_invaders()
{
    destroy_shot_images();
    destroy_ship(NULL);
    destroy_alien_invasion(NULL);
    destroy_shot(NULL);
    al_shutdown_image_addon();
    al_shutdown_font_addon();
    al_shutdown_ttf_addon();
    al_shutdown_primitives_addon();
    al_uninstall_system();
}