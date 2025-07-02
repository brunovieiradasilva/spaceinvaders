
# Space Invaders - Documentação

## Sumário

- [Descrição](#descrição)
- [Requisitos](#requisitos)
- [Estrutura de Pastas](#estrutura-de-pastas)
- [Compilação](#compilação)
- [Como Jogar](#como-jogar)
- [Controles](#controles)
- [Arquitetura do Código](#arquitetura-do-código)
- [Principais Funções](#principais-funções)
- [Personalização](#personalização)
- [Créditos](#créditos)

---

## Descrição

Este projeto é uma implementação do clássico jogo **Space Invaders** usando a biblioteca Allegro 5 em C. O jogo suporta modo singleplayer e multiplayer, diferentes níveis de dificuldade, recordes salvos em arquivo e gráficos customizados.

---

## Requisitos

- **Compilador C** (GCC recomendado)
- **Allegro 5** (com addons de imagem, áudio, fonte, ttf, primitives)
- **Windows** (pode ser adaptado para Linux)
- As imagens e sons necessários (ver pasta `imgs` e `sounds`)

---

## Estrutura de Pastas

```
spaceinvaders/
│
├── imgs/
│   ├── alienship.png
│   ├── background.png
│   ├── background1.png
│   ├── background2.png
│   ├── shot0.png
│   ├── shot1.png
│   ├── shot2.png
│   ├── spaceship.png
│   └── spaceship1.png
│
├── sounds/
│   ├── audio_explode1.flac
│   ├── audio_explode2.flac
│   ├── audio_shot.flac
│   └── sound.wav
│
├── libs/
│   ├── invaders.c
│   ├── invaders.h
│
├── main.c
├── Makefile
└── record.txt
```

---

## Compilação

1. **Configure o Allegro**: Certifique-se de que as bibliotecas e includes do Allegro estão corretamente configurados.
2. **Compile usando o Makefile**:
   ```sh
   make
   ```
   Isso irá gerar o executável main.exe.

---

## Como Jogar

1. Execute o jogo:
   ```
   ./main.exe
   ```
2. Escolha a dificuldade e o modo de jogo (singleplayer ou multiplayer).
3. Use os controles para mover a nave e atirar nos aliens.
4. O objetivo é destruir todos os aliens sem ser atingido ou deixar que eles alcancem o fundo da tela.

---

## Controles

- **Player 1**
  - Mover: `A` (esquerda), `D` (direita)
  - Atirar: `W`
- **Player 2 (multiplayer)**
  - Mover: `←` (esquerda), `→` (direita)
  - Atirar: `↑`
- **Menu**
  - Mouse para clicar nos botões
  - `ESC` para sair ou voltar ao menu

---

## Arquitetura do Código

- **main.c**: Loop principal do jogo, menus, controle de eventos, lógica de pontuação e recordes.
- **invaders.h**: Definições de structs, constantes, protótipos de funções e macros.
- **invaders.c**: Implementação das funções de nave, alien, tiros, colisão, animação e utilitários.
- **imgs/**: Imagens dos sprites, fundos e tiros.
- **sounds/**: Arquivos de áudio para música de fundo e efeitos.
- **record.txt**: Arquivo de texto que armazena os recordes de pontuação.

---

## Principais Funções

- **init_background()**: Carrega as imagens de fundo.
- **draw_background(int type)**: Desenha o fundo selecionado.
- **draw_menu(...)**: Desenha os menus e botões.
- **init_ship(Ship*, int)**: Inicializa a nave do jogador.
- **draw_ship(Ship*)**: Desenha a nave.
- **ship_move(Ship*)**: Move a nave conforme direção.
- **ship_atack(Ship*, Shot*)**: Dispara um tiro.
- **init_alien_invasion(Alien*[4][6])**: Inicializa a matriz de aliens.
- **draw_alien_invasion(Alien*[4][6])**: Desenha todos os aliens.
- **move_alien_invasion(Alien*[4][6], int)**: Move a matriz de aliens.
- **init_shots_images()**: Carrega as imagens dos tiros.
- **draw_shot(Shot*)**: Desenha um tiro.
- **move_shot(Shot*, int)**: Move um tiro.
- **collide(...)**: Detecta colisão entre retângulos.
- **alien_atack(Shot*, Alien*[4][6])**: Alien dispara um tiro.
- **init_alien_shot(Shot*, Alien*[4][6])**: Inicializa o tiro do alien.

---

## Personalização

- **Sprites**: Substitua as imagens em imgs para mudar o visual das naves, aliens, tiros e fundos.
- **Sons**: Troque os arquivos em sounds para novos efeitos ou músicas.
- **Dificuldade**: Ajuste as constantes em invaders.h para alterar velocidade, tamanho, etc.
- **Recordes**: O arquivo record.txt armazena os recordes de cada modo.

---

## Créditos

- Código: Bruno Vieira da Silva, com base no código de 
Pedro O.S Vaz de Melo.
- Biblioteca: [Allegro 5](https://liballeg.org/)
- Sprites e sons: domínio público

---

**Divirta-se jogando Space Invaders!**
```
