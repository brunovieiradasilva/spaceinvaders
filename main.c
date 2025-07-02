// author: Bruno Vieira da Silva

#include <stdio.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>

#include <stdlib.h>
#include <time.h>
#include <windows.h>
#include "libs/invaders.h"

const float FPS = 10;

ALLEGRO_BITMAP *background_png = NULL;
ALLEGRO_BITMAP *background_png1 = NULL;
ALLEGRO_BITMAP *background_png2 = NULL;
ALLEGRO_SAMPLE *sound = NULL;
ALLEGRO_DISPLAY *display = NULL;
ALLEGRO_EVENT_QUEUE *event_queue = NULL;
ALLEGRO_TIMER *timer = NULL;
ALLEGRO_FONT *font = NULL;
ALLEGRO_FILE *record_file;

int record_normal = 0;
int record_dificil = 0;
int in_menu = 1;		 // 1: menu principal, 2: menu de dificuldade, 3: jogo, 4: menu de fim de jogo
int dificulty = 0;		 // 0: normal, 1: dificil
int players = 1;		 // 1: player, 2: multiplayer
int background_type = 0; // 0: background_png, 1: background_png1, 2: background_png2
int points = 0;			 // Pontos do jogador

void init_background()
{
	background_png = al_load_bitmap("imgs/background.png");
	background_png1 = al_load_bitmap("imgs/background1.png");
	background_png2 = al_load_bitmap("imgs/background2.png");
	if (!background_png)
	{
		printf("Nao foi possivel carregar a imagem de fundo 0\n");
		return;
	}

	if (!background_png1)
	{
		printf("Nao foi possivel carregar a imagem de fundo 1\n");
		return;
	}

	if (!background_png2)
	{
		printf("Nao foi possivel carregar a imagem de fundo 2\n");
		return;
	}
	sound = al_load_sample("sounds/sound.wav");
	must_init(sound, "sound");
}

void draw_background(int type)
{
	if (type == 0 && background_png)
	{
		al_draw_bitmap(background_png, 0, 0, 0);
	}
	else if (type == 1 && background_png1)
	{
		al_draw_bitmap(background_png1, 0, 0, 0);
	}
	else if (type == 2 && background_png2)
	{
		al_draw_bitmap(background_png2, 0, 0, 0);
	}
	else
	{
		printf("Imagem de fundo nao carregada\n");
	}

	if (in_menu == 3)
	{
		al_draw_filled_rectangle(0, SCREEN_H - 72, SCREEN_W, SCREEN_H, al_map_rgb(0, 200, 0));
	}
}

void draw_menu(Button *btn1, Button *btn2, Button *btn3)
{
	if (in_menu == 3)
	{
		int rec_x = 30;
		int rec_y = 20;
		char buf[16];
		sprintf(buf, "%d", points);
		al_draw_text(font, al_map_rgb(255, 255, 255), rec_x, rec_y, 0, "pontos:");
		al_draw_text(font, al_map_rgb(255, 255, 0), rec_x + 110, rec_y, 0, buf);
		return;
	}

	al_clear_to_color(al_map_rgb(0, 0, 0));

	btn3->text = "sair";
	btn3->x = SCREEN_W - 120 - 30;
	btn3->y = SCREEN_H - 50 - 30;
	btn3->w = 120;
	btn3->h = 50;

	if (in_menu == 1)
	{
		int btn_w = 300;
		int btn_h = 72;
		int spacing = 36;
		int center_x = SCREEN_W / 2;
		int center_y = SCREEN_H / 2;

		btn1->x = center_x - btn_w / 2;
		btn1->y = center_y - btn_h - spacing / 2;
		btn1->w = btn_w;
		btn1->h = btn_h;
		btn2->x = center_x - btn_w / 2;
		btn2->y = center_y + spacing / 2;
		btn2->w = btn_w;
		btn2->h = btn_h;

		al_clear_to_color(al_map_rgb(0, 0, 0));
		draw_background(0);

		// Desenha os botões
		al_draw_filled_rounded_rectangle(btn1->x, btn1->y, btn1->x + btn1->w, btn1->y + btn1->h, 15, 15, al_map_rgb(40, 180, 40));
		al_draw_filled_rounded_rectangle(btn2->x, btn2->y, btn2->x + btn2->w, btn2->y + btn2->h, 15, 15, al_map_rgb(180, 40, 40));
		al_draw_filled_rounded_rectangle(btn3->x, btn3->y, btn3->x + btn3->w, btn3->y + btn3->h, 12, 12, al_map_rgb(60, 60, 60));

		// Desenha as bordas dos botões
		al_draw_rounded_rectangle(btn1->x, btn1->y, btn1->x + btn1->w, btn1->y + btn1->h, 15, 15, al_map_rgb(0, 0, 0), 3);
		al_draw_rounded_rectangle(btn2->x, btn2->y, btn2->x + btn2->w, btn2->y + btn2->h, 15, 15, al_map_rgb(0, 0, 0), 3);
		al_draw_rounded_rectangle(btn3->x, btn3->y, btn3->x + btn3->w, btn3->y + btn3->h, 12, 12, al_map_rgb(0, 0, 0), 2);

		int text_h = al_get_font_line_height(font);
		al_draw_text(font, al_map_rgb(255, 255, 255), center_x, btn1->y + (btn1->h - text_h) / 2, ALLEGRO_ALIGN_CENTRE, btn1->text);
		al_draw_text(font, al_map_rgb(255, 255, 255), center_x, btn2->y + (btn2->h - text_h) / 2, ALLEGRO_ALIGN_CENTRE, btn2->text);
		al_draw_text(font, al_map_rgb(255, 255, 255), btn3->x + btn3->w / 2, btn3->y + (btn3->h - text_h) / 2, ALLEGRO_ALIGN_CENTRE, btn3->text);

		// --- Desenha recorde no canto superior esquerdo ---
		int rec_x = 30;
		int rec_y = 20;
		char buf_normal[16], buf_dificil[16];
		sprintf(buf_normal, "%d", record_normal);
		sprintf(buf_dificil, "%d", record_dificil);
		al_draw_text(font, al_map_rgb(255, 255, 255), rec_x, rec_y, 0, "recorde");
		al_draw_text(font, al_map_rgb(255, 255, 255), rec_x, rec_y + text_h + 5, 0, "normal:");
		al_draw_text(font, al_map_rgb(255, 255, 0), rec_x + 110, rec_y + text_h + 5, 0, buf_normal);
		al_draw_text(font, al_map_rgb(255, 255, 255), rec_x, rec_y + 2 * text_h + 10, 0, "dificil:");
		al_draw_text(font, al_map_rgb(255, 255, 0), rec_x + 110, rec_y + 2 * text_h + 10, 0, buf_dificil);
	}
	else if (in_menu == 2)
	{
		int btn_w = 220;
		int btn_h = 72;
		int spacing = 40;
		int center_x = SCREEN_W / 2;
		int center_y = SCREEN_H / 2;

		btn1->x = center_x - btn_w - spacing / 2;
		btn1->y = center_y - btn_h / 2;
		btn1->w = btn_w;
		btn1->h = btn_h;
		btn1->text = "singleplayer";
		btn2->x = center_x + spacing / 2;
		btn2->y = center_y - btn_h / 2;
		btn2->w = btn_w;
		btn2->h = btn_h;
		btn2->text = "multiplayer";

		// Desenha os botões
		al_draw_filled_rounded_rectangle(btn1->x, btn1->y, btn1->x + btn1->w, btn1->y + btn1->h, 15, 15, al_map_rgb(40, 120, 200));
		al_draw_filled_rounded_rectangle(btn2->x, btn2->y, btn2->x + btn2->w, btn2->y + btn2->h, 15, 15, al_map_rgb(200, 120, 40));
		al_draw_filled_rounded_rectangle(btn3->x, btn3->y, btn3->x + btn3->w, btn3->y + btn3->h, 12, 12, al_map_rgb(60, 60, 60));

		// Bordas
		al_draw_rounded_rectangle(btn1->x, btn1->y, btn1->x + btn1->w, btn1->y + btn1->h, 15, 15, al_map_rgb(0, 0, 0), 3);
		al_draw_rounded_rectangle(btn2->x, btn2->y, btn2->x + btn2->w, btn2->y + btn2->h, 15, 15, al_map_rgb(0, 0, 0), 3);
		al_draw_rounded_rectangle(btn3->x, btn3->y, btn3->x + btn3->w, btn3->y + btn3->h, 12, 12, al_map_rgb(0, 0, 0), 2);

		// Texto centralizado
		int text_h = al_get_font_line_height(font);
		al_draw_text(font, al_map_rgb(255, 255, 255), btn1->x + btn1->w / 2, btn1->y + (btn1->h - text_h) / 2, ALLEGRO_ALIGN_CENTRE, btn1->text);
		al_draw_text(font, al_map_rgb(255, 255, 255), btn2->x + btn2->w / 2, btn2->y + (btn2->h - text_h) / 2, ALLEGRO_ALIGN_CENTRE, btn2->text);
		al_draw_text(font, al_map_rgb(255, 255, 255), btn3->x + btn3->w / 2, btn3->y + (btn3->h - text_h) / 2, ALLEGRO_ALIGN_CENTRE, btn3->text);
	}
	else if (in_menu == 4)
	{
		int center_x = SCREEN_W / 2;
		int center_y = SCREEN_H / 2;
		int text_h = al_get_font_line_height(font);

		// Desenha o texto "FIM DE JOGO"
		al_draw_text(font, al_map_rgb(255, 0, 0), center_x, center_y - text_h * 2, ALLEGRO_ALIGN_CENTRE, "FIM DE JOGO");

		// Desenha o quadrado amarelo com os pontos
		char pontos_str[32];
		sprintf(pontos_str, "Pontos: %d", points);
		int rect_w = al_get_text_width(font, pontos_str) + 40;
		int rect_h = text_h + 20;
		int rect_x = center_x - rect_w / 2;
		int rect_y = center_y - rect_h / 2;
		al_draw_filled_rectangle(rect_x, rect_y, rect_x + rect_w, rect_y + rect_h, al_map_rgb(255, 255, 0));
		al_draw_rectangle(rect_x, rect_y, rect_x + rect_w, rect_y + rect_h, al_map_rgb(200, 200, 0), 3);
		al_draw_text(font, al_map_rgb(0, 0, 0), center_x, rect_y + 10, ALLEGRO_ALIGN_CENTRE, pontos_str);

		// Desenha o botão de sair embaixo do quadrado
		btn3->y = rect_y + rect_h + 40;
		btn3->x = center_x - btn3->w / 2;
		al_draw_filled_rounded_rectangle(btn3->x, btn3->y, btn3->x + btn3->w, btn3->y + btn3->h, 12, 12, al_map_rgb(60, 60, 60));
		al_draw_rounded_rectangle(btn3->x, btn3->y, btn3->x + btn3->w, btn3->y + btn3->h, 12, 12, al_map_rgb(0, 0, 0), 2);
		al_draw_text(font, al_map_rgb(255, 255, 255), btn3->x + btn3->w / 2, btn3->y + (btn3->h - text_h) / 2, ALLEGRO_ALIGN_CENTRE, btn3->text);
		if (points > record_normal && dificulty == 0)
		{
			record_normal = points;
			al_fseek(record_file, 0, ALLEGRO_SEEK_SET);
			char buf[32];
			sprintf(buf, "%d\n", record_normal);
			al_fputs(record_file, buf);
			al_fflush(record_file);
		}
		else if (points > record_dificil && dificulty == 1)
		{
			record_dificil = points;
			al_fseek(record_file, sizeof(int), ALLEGRO_SEEK_SET);
			char buf[32];
			sprintf(buf, "%d\n", record_dificil);
			al_fputs(record_file, buf);
			al_fflush(record_file);
		}
	}
}

int main(int argc, char const *argv[])
{

	Button btn_normal = {0, 0, 0, 0, "dificuldade normal"};
	Button btn_dificil = {0, 0, 0, 0, "dificuldade dificil"};
	Button btn_singleplayer = {0, 0, 0, 0, "singleplayer"};
	Button btn_multiplayer = {0, 0, 0, 0, "multiplayer"};
	Button btn_sair = {0, 0, 0, 0, "sair"};

	srand(time(NULL));
	if (!al_init())
	{
		fprintf(stderr, "failed to initialize allegro!\n");
		return -1;
	}
	must_init(al_install_keyboard(), "teclado");
	must_init(al_install_mouse(), "mouse");
	must_init(al_init_image_addon(), "imagens");
	must_init(al_install_audio(), "audio");
	must_init(al_init_acodec_addon(), "audio codecs");
	must_init(al_reserve_samples(16), "reserve samples");
	must_init(al_init_primitives_addon(), "primitives");

	// cria um temporizador que incrementa uma unidade a cada 1.0/FPS segundos
	timer = al_create_timer(1.0 / FPS);
	if (!timer)
	{
		fprintf(stderr, "failed to create timer!\n");
		return -1;
	}

	// cria uma tela com dimensoes de SCREEN_W, SCREEN_H pixels
	display = al_create_display(SCREEN_W, SCREEN_H);
	if (!display)
	{
		fprintf(stderr, "failed to create display!\n");
		al_destroy_timer(timer);
		return -1;
	}
	else
	{
		al_set_window_title(display, "Space Invaders");
		al_set_display_icon(display, al_load_bitmap("imgs/alienship.png"));
	}

	// inicializa o modulo allegro que carrega as fontes
	al_init_font_addon();
	must_init(al_init_ttf_addon(), "ttf");

	// carrega o arquivo arial.ttf da fonte Arial e define que sera usado o tamanho 32 (segundo parametro)
	font = al_load_font("PixelTwist.ttf", 32, 1);
	if (font == NULL)
	{
		fprintf(stderr, "font file does not exist or cannot be accessed!\n");
	}

	// cria a fila de eventos
	event_queue = al_create_event_queue();
	if (!event_queue)
	{
		fprintf(stderr, "failed to create event_queue!\n");
		al_destroy_display(display);
		return -1;
	}

	// eventos de tela
	al_register_event_source(event_queue, al_get_display_event_source(display));
	// eventos de tempo
	al_register_event_source(event_queue, al_get_timer_event_source(timer));
	// eventos de teclado
	al_register_event_source(event_queue, al_get_keyboard_event_source());
	// eventos de mouse
	al_register_event_source(event_queue, al_get_mouse_event_source());

	al_start_timer(timer);

	record_file = al_fopen("record.txt", "r+");
	if (!record_file)
	{
		record_file = al_fopen("record.txt", "w+");
		al_fputs(record_file, "0\n");
		al_fputs(record_file, "0\n");
		al_fflush(record_file);
		al_fseek(record_file, 0, ALLEGRO_SEEK_SET);
	}
	// Lê os recordes do arquivo
	char buf[32];
	if (al_fgets(record_file, buf, 32))
		sscanf(buf, "%d", &record_normal);
	if (al_fgets(record_file, buf, 32))
		sscanf(buf, "%d", &record_dificil);

	int playing = 1;
	init_background();
	play_music(sound, 0); // Toca a música de fundo

	Ship ship = {0};
	Ship ship1 = {0};
	init_ship(&ship, 0);  // Inicializa a nave do player
	init_ship(&ship1, 1); // Inicializa a nave do segundo player

	Alien *invasion[4][6] = {0};	  // Matriz de invasores
	init_alien_invasion(invasion, 0); // Inicializa os invasores

	Shot shot = {0};				   // tiro
	Shot shot1 = {0};				   // tiro do segundo player
	Shot shot2 = {0};				   // tiro do alien
	init_shot(&shot, &ship, 0);		   // Inicializa o tiro
	init_shot(&shot1, &ship1, 1);	   // Inicializa o tiro do segundo player
	init_alien_shot(&shot2, invasion); // Inicializa o tiro do alien
	init_shots_images();
	while (playing)
	{
		ALLEGRO_EVENT ev;
		// espera por um evento e o armazena na variavel de evento ev
		al_wait_for_event(event_queue, &ev);

		// se o tipo de evento for um evento do temporizador, ou seja, se o tempo passou de t para t+1
		if (ev.type == ALLEGRO_EVENT_TIMER)
		{
			// atualiza a tela (quando houver algo para mostrar)
			if (in_menu == 1)
			{
				draw_menu(&btn_normal, &btn_dificil, &btn_sair);
			}
			else if (in_menu == 2)
			{
				al_clear_to_color(al_map_rgb(0, 0, 0));
				draw_menu(&btn_singleplayer, &btn_multiplayer, &btn_sair);
			}
			else if (in_menu == 4)
			{
				draw_menu(NULL, NULL, &btn_sair);
			}
			else
			{
				al_clear_to_color(al_map_rgb(0, 0, 0));
				draw_background(background_type);
				// Move a nave a cada 1 segundo
				if (al_get_timer_count(timer) % (int)(FPS / 10) == 0)
				{
					ship_move(&ship);
					move_shot(&shot, dificulty); // Move o tiro do player
					if (players == 2)
					{
						ship_move(&ship1);
						move_shot(&shot1, dificulty); // Move o tiro do segundo player
					}
					if (dificulty == 1)
					{
						move_shot(&shot2, dificulty); // Move o tiro do alien
					}
				}

				if (al_get_timer_count(timer) % (int)FPS == 0)
				{
					// Move os invasores a cada segundo
					if (move_alien_invasion(invasion, dificulty))
					{
						printf("Invasores chegaram ao fundo da tela!\n");
						points -= 20;
						in_menu = 4; // Muda para o menu de fim de jogo
						continue;
					}
				}

				draw_ship(&ship, 0); // Desenha a nave do player
				draw_shot(&shot); // Desenha o tiro do player
				if (players == 2)
				{
					draw_ship(&ship1, 1); // Desenha a nave do segundo player
					draw_shot(&shot1); // Desenha o tiro do segundo player
				}
				if (dificulty == 1)
				{
					draw_shot(&shot2); // Move o tiro do alien
				}

				draw_alien_invasion(invasion);

				draw_menu(NULL, NULL, NULL); // Desenha os pontos na tela

				for (int i = 0; i < 4; i++)
				{
					for (int j = 0; j < 6; j++)
					{
						Alien *alien = invasion[i][j];
						if (alien && alien->alive)
						{
							int ax1 = alien->x;
							int ay1 = alien->y;
							int ax2 = alien->x + ALIEN_W;
							int ay2 = alien->y + ALIEN_H;
							int sx1 = shot.x;
							int sy1 = shot.y;
							int sx2 = shot.x + SHOT_W;
							int sy2 = shot.y + SHOT_H;
							if (collide(ax1, ay1, ax2, ay2, sx1, sy1, sx2, sy2) && shot.active)
							{
								alien->alive = 0;
								shot.active = 0;
								points += 10;				   // Adiciona pontos ao jogador
								alien_atack(&shot2, invasion); // Atira o alien
								play_music(sound, 2);
							}
							if (players == 2 && shot1.active)
							{
								int sx1b = shot1.x;
								int sy1b = shot1.y;
								int sx2b = shot1.x + SHOT_W;
								int sy2b = shot1.y + SHOT_H;
								if (collide(ax1, ay1, ax2, ay2, sx1b, sy1b, sx2b, sy2b))
								{
									alien->alive = 0;
									shot1.active = 0;
									points += 10;
									alien_atack(&shot2, invasion); // Atira o alien
									play_music(sound, 2);
								}
							}
							// --- Colisão do alien com a nave ---
							int ship_x1 = ship.x;
							int ship_y1 = SHIP_Y;
							int ship_x2 = ship.x + SHIP_W;
							int ship_y2 = SHIP_Y + SHIP_H;
							if (collide(ax1, ay1, ax2, ay2, ship_x1, ship_y1, ship_x2, ship_y2))
							{
								printf("Colisão entre alien e nave!\n");
								points -= 20; // Penaliza o jogador
								in_menu = 4;  // Muda para o menu de fim de jogo
							}
							if (players == 2)
							{
								int ship1_x1 = ship1.x;
								int ship1_y1 = SHIP_Y;
								int ship1_x2 = ship1.x + SHIP_W;
								int ship1_y2 = SHIP_Y + SHIP_H;
								if (collide(ax1, ay1, ax2, ay2, ship1_x1, ship1_y1, ship1_x2, ship1_y2))
								{
									printf("Colisão entre alien e nave!\n");
									points -= 20; // Penaliza o jogador
									in_menu = 4;  // Muda para o menu de fim de jogo
								}
							}
						}
						if (dificulty == 1 && shot2.active)
						{
							int shot2_x1 = shot2.x;
							int shot2_y1 = shot2.y;
							int shot2_x2 = shot2.x + SHOT_W;
							int shot2_y2 = shot2.y + SHOT_H;
							int ship_x1 = ship.x;
							int ship_y1 = SHIP_Y;
							int ship_x2 = ship.x + SHIP_W;
							int ship_y2 = SHIP_Y + SHIP_H;
							if (collide(shot2_x1, shot2_y1, shot2_x2, shot2_y2, ship_x1, ship_y1, ship_x2, ship_y2))
							{
								play_music(sound, 3);
								printf("Tiro do alien atingiu a nave!\n");
								points -= 20;
								in_menu = 4;
								shot2.active = 0;
							}
							if (players == 2)
							{
								int ship1_x1 = ship1.x;
								int ship1_y1 = SHIP_Y;
								int ship1_x2 = ship1.x + SHIP_W;
								int ship1_y2 = SHIP_Y + SHIP_H;
								if (collide(shot2_x1, shot2_y1, shot2_x2, shot2_y2, ship1_x1, ship1_y1, ship1_x2, ship1_y2))
								{
									play_music(sound, 3);
									printf("Tiro do alien atingiu a nave 2!\n");
									points -= 20;
									in_menu = 4;
									shot2.active = 0;
								}
							}
						}
					}
				}

				int all_dead = 1;
				for (int i = 0; i < 4; i++)
				{
					for (int j = 0; j < 6; j++)
					{
						if (invasion[i][j] && invasion[i][j]->alive)
						{
							all_dead = 0;
							break;
						}
					}
					if (!all_dead)
						break;
				}
				if (all_dead)
				{
					in_menu = 4; // Termina o jogo (vai para o menu de fim de jogo)
				}
			}

			al_flip_display();

			if (al_get_timer_count(timer) % (int)FPS == 0)
				printf("\n%d segundos se passaram...", (int)(al_get_timer_count(timer) / FPS));
		}
		// evento fechamento de tela
		else if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
		{
			playing = 0;
		}
		// evento clique de mouse
		else if (ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN)
		{
			Beep(500, 150);
			printf("\nmouse clicado em: %d, %d", ev.mouse.x, ev.mouse.y);
			if (in_menu == 1)
			{
				// Verifica se o clique foi no botão de dificuldade normal
				if (collide_btn(ev.mouse.x, ev.mouse.y, btn_normal.x, btn_normal.y, btn_normal.x + btn_normal.w, btn_normal.y + btn_normal.h))
				{
					// Exibe uma mensagem de clique no botão normal
					printf("\nDificuldade normal");
					in_menu = 2;
					dificulty = 0;							  // Define a dificuldade como normal
					init_alien_invasion(invasion, dificulty); // Inicializa os invasores
					background_type = randon(0, 2);			  // Escolhe um fundo aleatório
				}
				// Verifica se o clique foi no botão de dificuldade difícil
				else if (collide_btn(ev.mouse.x, ev.mouse.y, btn_dificil.x, btn_dificil.y, btn_dificil.x + btn_dificil.w, btn_dificil.y + btn_dificil.h))
				{
					printf("\nDificuldade dificil");
					in_menu = 2;
					dificulty = 1;							  // Define a dificuldade como difícil
					init_alien_invasion(invasion, dificulty); // Inicializa os invasores
					background_type = randon(0, 2);			  // Escolhe um fundo aleatório
				}
				// Verifica se o clique foi no botão sair
				else if (collide_btn(ev.mouse.x, ev.mouse.y, btn_sair.x, btn_sair.y, btn_sair.x + btn_sair.w, btn_sair.y + btn_sair.h))
				{
					printf("\nSair do jogo");
					playing = 0; // Fecha o jogo
				}
			}
			else if (in_menu == 2)
			{
				if (collide_btn(ev.mouse.x, ev.mouse.y, btn_singleplayer.x, btn_singleplayer.y, btn_singleplayer.x + btn_singleplayer.w, btn_singleplayer.y + btn_singleplayer.h))
				{
					printf("\nSingleplayer selecionado");
					players = 1;
					in_menu = 3; // inicia o jogo singleplayer
				}
				else if (collide_btn(ev.mouse.x, ev.mouse.y, btn_multiplayer.x, btn_multiplayer.y, btn_multiplayer.x + btn_multiplayer.w, btn_multiplayer.y + btn_multiplayer.h))
				{
					printf("\nMultiplayer selecionado");
					players = 2;
					in_menu = 3; // inicia o jogo multiplayer
				}
				else if (collide_btn(ev.mouse.x, ev.mouse.y, btn_sair.x, btn_sair.y, btn_sair.x + btn_sair.w, btn_sair.y + btn_sair.h))
				{
					printf("\nvoltar ao menu");
					in_menu = 1; // volta ao menu
				}
			}
			else if (in_menu == 4)
			{
				if (collide_btn(ev.mouse.x, ev.mouse.y, btn_sair.x, btn_sair.y, btn_sair.x + btn_sair.w, btn_sair.y + btn_sair.h))
				{
					printf("\nvoltar ao menu");
					// Reseta o jogo
					init_ship(&ship, 0);
					init_ship(&ship1, 1);

					destroy_alien_invasion(invasion); // Matriz de invasores

					init_shot(&shot, &ship, 0);		   // Inicializa o tiro
					init_shot(&shot1, &ship1, 1);	   // Inicializa o tiro do segundo player
					init_alien_shot(&shot2, invasion); // Inicializa o tiro do alien
					background_type = randon(0, 2);	   // Escolhe um fundo aleatório
					in_menu = 1;					   // volta ao menu
					points = 0;						   // Reseta os pontos
				}
			}
		}
		// evento pressionar de tecla
		else if (ev.type == ALLEGRO_EVENT_KEY_DOWN)
		{
			// imprime qual tecla foi
			printf("\ncodigo tecla: %d", ev.keyboard.keycode);
			if (ev.keyboard.keycode == ALLEGRO_KEY_ESCAPE)
			{
				printf("\nSair do jogo");
				if (in_menu == 3)
				{
					in_menu = 4;
					continue;
				}
				else if (in_menu == 4)
				{
					in_menu = 1; // Volta ao menu principal
					continue;
				}
				else if (in_menu == 2)
				{
					in_menu = 1; // Volta ao menu principal
					continue;
				}
				else
					playing = 0; // Fecha o jogo
			}
			if (in_menu == 3)
			{

				if (ev.keyboard.keycode == ALLEGRO_KEY_A || ev.keyboard.keycode == ALLEGRO_KEY_W || ev.keyboard.keycode == ALLEGRO_KEY_D)
					ship_keyboard(&ship, &shot, ev.keyboard.keycode, 0);

				if (players == 2 && (ev.keyboard.keycode == ALLEGRO_KEY_LEFT || ev.keyboard.keycode == ALLEGRO_KEY_RIGHT || ev.keyboard.keycode == ALLEGRO_KEY_UP))
				{
					ship_keyboard(&ship1, &shot1, ev.keyboard.keycode, 0);
				}
			}
		}

		else if (ev.type == ALLEGRO_EVENT_KEY_UP)
		{
			if (!in_menu)
			{
				ship_keyboard(&ship, NULL, ev.keyboard.keycode, 1);
			}
		}
	}

	// procedimentos de destruição
	destroy_space_invaders();
	al_fclose(record_file);
	al_destroy_bitmap(background_png);
	al_destroy_bitmap(background_png1);
	al_destroy_bitmap(background_png2);
	al_destroy_sample(sound);
	al_destroy_font(font);
	al_destroy_timer(timer);
	al_destroy_display(display);
	al_destroy_event_queue(event_queue);

	return 0;
}