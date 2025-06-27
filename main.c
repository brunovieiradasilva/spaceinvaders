#include <stdio.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>

#include <windows.h>
#include "libs/invaders.h"

const float FPS = 30;

ALLEGRO_BITMAP *background_png = NULL;
ALLEGRO_SAMPLE *sound = NULL;
ALLEGRO_DISPLAY *display = NULL;
ALLEGRO_EVENT_QUEUE *event_queue = NULL;
ALLEGRO_TIMER *timer = NULL;
ALLEGRO_FONT *font = NULL;

Button btn_normal = {0, 0, 0, 0, "dificuldade normal"};
Button btn_dificil = {0, 0, 0, 0, "dificuldade dificil"};
Button btn_sair = {0, 0, 0, 0, "sair"};
void must_init(bool test, const char *description)
{
	if (test)
		return;

	printf("couldn't initialize %s\n", description);
	exit(1);
}

void init_background()
{
	background_png = al_load_bitmap("imgs/background.png");
	if (!background_png)
	{
		printf("Nao foi possivel carregar a imagem de fundo\n");
		return;
	}

	al_draw_bitmap(background_png, 0, 0, 0);

	sound = al_load_sample("sounds/sound.wav");
	must_init(sound, "sound");
	al_play_sample(sound, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
}

void draw_menu()
{
	int btn_w = 300;
	int btn_h = 72;
	int spacing = 36;
	int center_x = SCREEN_W / 2;
	int center_y = SCREEN_H / 2;

	btn_normal.x = center_x - btn_w / 2;
	btn_normal.y = center_y - btn_h - spacing / 2;
	btn_normal.w = btn_w;
	btn_normal.h = btn_h;
	btn_dificil.x = center_x - btn_w / 2;
	btn_dificil.y = center_y + spacing / 2;
	btn_dificil.w = btn_w;
	btn_dificil.h = btn_h;
	btn_sair.x = SCREEN_W - 120 - 30;
	btn_sair.y = SCREEN_H - 50 - 30;
	btn_sair.w = 120;
	btn_sair.h = 50;

	// Desenha os botões
	al_draw_filled_rounded_rectangle(btn_normal.x, btn_normal.y, btn_normal.x + btn_normal.w, btn_normal.y + btn_normal.h, 15, 15, al_map_rgb(40, 180, 40));
	al_draw_filled_rounded_rectangle(btn_dificil.x, btn_dificil.y, btn_dificil.x + btn_dificil.w, btn_dificil.y + btn_dificil.h, 15, 15, al_map_rgb(180, 40, 40));
	al_draw_filled_rounded_rectangle(btn_sair.x, btn_sair.y, btn_sair.x + btn_sair.w, btn_sair.y + btn_sair.h, 12, 12, al_map_rgb(60, 60, 60));

	// Desenha as bordas dos botões
	al_draw_rounded_rectangle(btn_normal.x, btn_normal.y, btn_normal.x + btn_normal.w, btn_normal.y + btn_normal.h, 15, 15, al_map_rgb(0, 0, 0), 3);
	al_draw_rounded_rectangle(btn_dificil.x, btn_dificil.y, btn_dificil.x + btn_dificil.w, btn_dificil.y + btn_dificil.h, 15, 15, al_map_rgb(0, 0, 0), 3);
	al_draw_rounded_rectangle(btn_sair.x, btn_sair.y, btn_sair.x + btn_sair.w, btn_sair.y + btn_sair.h, 12, 12, al_map_rgb(0, 0, 0), 2);

	int text_h = al_get_font_line_height(font);
	al_draw_text(font, al_map_rgb(255, 255, 255), center_x, btn_normal.y + (btn_normal.h - text_h) / 2, ALLEGRO_ALIGN_CENTRE, btn_normal.text);
	al_draw_text(font, al_map_rgb(255, 255, 255), center_x, btn_dificil.y + (btn_dificil.h - text_h) / 2, ALLEGRO_ALIGN_CENTRE, btn_dificil.text);
	al_draw_text(font, al_map_rgb(255, 255, 255), btn_sair.x + btn_sair.w / 2, btn_sair.y + (btn_sair.h - text_h) / 2, ALLEGRO_ALIGN_CENTRE, btn_sair.text);
}

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

int main(int argc, char const *argv[])
{
	int in_menu = 1;
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

	int playing = 1;
	init_background();
	draw_menu();
	while (playing)
	{
		ALLEGRO_EVENT ev;
		// espera por um evento e o armazena na variavel de evento ev
		al_wait_for_event(event_queue, &ev);

		// se o tipo de evento for um evento do temporizador, ou seja, se o tempo passou de t para t+1
		if (ev.type == ALLEGRO_EVENT_TIMER)
		{
			// atualiza a tela (quando houver algo para mostrar)
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
			if (in_menu)
			{
				// Verifica se o clique foi no botão de dificuldade normal
				if (collide_btn(ev.mouse.x, ev.mouse.y, btn_normal.x, btn_normal.y, btn_normal.x + btn_normal.w, btn_normal.y + btn_normal.h))
				{
					// Exibe uma mensagem de clique no botão normal
					printf("\nDificuldade normal");
					in_menu = 0; // Sai do menu
				}

				// Verifica se o clique foi no botão de dificuldade difícil
				else if (collide_btn(ev.mouse.x, ev.mouse.y, btn_dificil.x, btn_dificil.y, btn_dificil.x + btn_dificil.w, btn_dificil.y + btn_dificil.h))
				{
					// Exibe uma mensagem de clique no botão difícil
					printf("\nDificuldade dificil");
					in_menu = 0; // Sai do menu
				}

				// Verifica se o clique foi no botão sair
				else if (collide_btn(ev.mouse.x, ev.mouse.y, btn_sair.x, btn_sair.y, btn_sair.x + btn_sair.w, btn_sair.y + btn_sair.h))
				{
					printf("\nSair do jogo");
					playing = 0; // Fecha o jogo
				}
			}
		}
		// evento pressionar de tecla
		else if (ev.type == ALLEGRO_EVENT_KEY_DOWN)
		{
			// imprime qual tecla foi
			printf("\ncodigo tecla: %d", ev.keyboard.keycode);
		}
	}

	// procedimentos de destruição
	al_destroy_bitmap(background_png);
	al_destroy_sample(sound);
	al_destroy_font(font);
	al_destroy_timer(timer);
	al_destroy_display(display);
	al_destroy_event_queue(event_queue);

	return 0;
}