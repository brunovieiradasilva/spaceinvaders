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

	//	al_draw_filled_rectangle(0, 0, 50, 50, al_map_rgb(0, 255, 0));
}

bool collide(int ax1, int ay1, int ax2, int ay2, int bx1, int by1, int bx2, int by2)
{
    if(ax1 > bx2) return false;
    if(ax2 < bx1) return false;
    if(ay1 > by2) return false;
    if(ay2 < by1) return false;

    return true;
}

int main(int argc, char const *argv[])
{

	ALLEGRO_DISPLAY *display = NULL;
	ALLEGRO_EVENT_QUEUE *event_queue = NULL;
	ALLEGRO_TIMER *timer = NULL;
	ALLEGRO_FONT *font = NULL;

	if (!al_init())
	{
		fprintf(stderr, "failed to initialize allegro!\n");
		return -1;
	}

	must_init(al_init_image_addon(), "imagens");
	must_init(al_install_audio(), "audio");
	must_init(al_init_acodec_addon(), "audio codecs");
	must_init(al_reserve_samples(16), "reserve samples");

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
	}else{
		al_set_window_title(display, "Space Invaders");
		al_set_display_icon(display, al_load_bitmap("imgs/alienship.png"));
	}

	// instala o teclado
	if (!al_install_keyboard())
	{
		fprintf(stderr, "failed to install keyboard!\n");
		return -1;
	}

	// instala o mouse
	if (!al_install_mouse())
	{
		fprintf(stderr, "failed to initialize mouse!\n");
		return -1;
	}

	// inicializa o modulo allegro que carrega as fontes
	al_init_font_addon();

	// inicializa o modulo allegro que entende arquivos tff de fontes
	if (!al_init_ttf_addon())
	{
		fprintf(stderr, "failed to load tff font module!\n");
		return -1;
	}

	/*
		//carrega o arquivo arial.ttf da fonte Arial e define que sera usado o tamanho 32 (segundo parametro)
		ALLEGRO_FONT font = al_load_font("arial.ttf", 32, 1);
		if(size_32 == NULL) {
			fprintf(stderr, "font file does not exist or cannot be accessed!\n");
		}
	*/

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
	while (playing)
	{
		printf("entrou\n");
		ALLEGRO_EVENT ev;
		// espera por um evento e o armazena na variavel de evento ev
		al_wait_for_event(event_queue, &ev);

		// se o tipo de evento for um evento do temporizador, ou seja, se o tempo passou de t para t+1
		if (ev.type == ALLEGRO_EVENT_TIMER)
		{
			draw_menu();

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
			Beep(500, 250);
			printf("\nmouse clicado em: %d, %d", ev.mouse.x, ev.mouse.y);
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