#ifndef PACMAN_H_INCLUDED
#define PACMAN_H_INCLUDED
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include<SDL2/SDL.h>
#include<SDL2/SDL_image.h>
#include<SDL2/SDL_mixer.h>
#include<SDL2/SDL_ttf.h>

#define PACMAN_DEM 20
#define SPEED 20
#define COIN_DEM 10
#define yBlocks 36
#define xBlocks 28

typedef struct PacManElement
{
    SDL_Rect *pacMan;
    SDL_Rect *pacMan_prev;
    int score;
    int lives;
    int lvl;
    struct PacManElement *next;
} PacManElement;

typedef struct WallElement
{
    SDL_Rect *wall;
    struct WallElement *next;
} WallElement;

typedef struct CoinElement
{
    SDL_Rect *coin;
    struct CoinElement *next;
} CoinElement;

typedef struct GhostElement
{
    SDL_Rect *ghost;
    SDL_Rect *ghost_prev;
    int ghost_color;
    int direction;
    struct GhostElement *next;
} GhostElement;

typedef struct TextElement
{
    char message[50];
    char font_path[50];
    int font_size;
    SDL_Color color;
    SDL_Rect *text_holder;
    struct TextElement *next;
} TextElement;

int INIT_ALL(SDL_Window *window,SDL_Renderer **renderer);
int play_window(SDL_Window *window,SDL_Renderer *renderer,int *);
///Creating all sturcts
PacManElement *createPacManElement(int x,int y);
WallElement *createWallElement(int x,int y);
CoinElement *createCoinElement(int x,int y);
GhostElement *createGhostElement(int x,int y,int ghost_color,int direction);
TextElement *createTextElement(int x,int y,int h,int w,char *message,char *font_path,int font_size,SDL_Color color);
///Inserting Elements
void insertWallAtBegining (WallElement **wallElement, WallElement *wall);
void insertCoinAtBegining (CoinElement **coinElement, CoinElement *coin);
void insertGhostAtBegining (GhostElement **ghostElement, GhostElement *ghost);
void insertTextAtBegining (TextElement **textElement, TextElement *text);

/// Function used to create a map line that used to draw the map then insert it in the map linked list
void create_line(WallElement **wallE,int size,int x_pos,int y_pos);
/// Functions used to create a map col that used to draw the map then insert it in the map linked list
void create_col(WallElement **wallE,int size,int x_pos,int y_pos);

/// Function used to create map border using create_line && create_col then insert it in the map linked list
void create_border(WallElement **wallE);
/// Function used to create map content using create_line && create_col then insert it in the map linked list
void create_inside(WallElement **wallE);

///Functions to init linke list of all element
void create_coins(CoinElement **coinElement,WallElement *wall);
void create_ghosts(GhostElement **ghostElement);
void create_texts(TextElement **textElement,PacManElement *pacman);
void create_map(WallElement **wallE);

/// Functions used to draw element of the app
SDL_Texture *initialize_font_from_file(TextElement *text,SDL_Renderer *renderer);
SDL_Texture *initialize_IMG_from_file(char* file_name, SDL_Renderer *renderer);

/// Functions used to draw element of the app
void draw_text(TextElement *text,SDL_Renderer *renderer);;
void draw_texts(TextElement **text,SDL_Renderer *renderer);
void draw_image(char *path,SDL_Renderer *renderer,SDL_Rect *holder);
void draw_map(WallElement **wallE,SDL_Renderer *renderer);
void draw_coins(CoinElement **coinE,SDL_Renderer *renderer);
void draw_ghosts(GhostElement **ghostE,SDL_Renderer *renderer);
void draw_pacman(int direction, SDL_Renderer *renderer,PacManElement *pacman);
void draw_lives(SDL_Renderer *renderer,PacManElement *pacman);

/// Fonction that test if the object is allowed to move or not by priorite of Cordinate X
int CollisionByX(int x, int y, WallElement *walE);
/// Fonction that test if the object is allowed to move or not by priorite of Cordinate X
int CollisionByY(int x, int y, WallElement *walE);

///Functions to UPDATE score and time text
void update_score(PacManElement *pacman,SDL_Renderer* renderer);
void update_timer(SDL_Renderer *renderer,int *lastTime,int beg);

///Functions to degub and show linked list
void show_wall_cord(WallElement *wall);
void show_coins_cord(CoinElement *coin);
void show_ghosts_cord(GhostElement *ghost);

///function to delete a singel coin from linked list of cons
void delete_coin(int x,int y,CoinElement **coins);
///function to delete a coin from linked list of cons according to pacman movment
void eat_coin(PacManElement *pacMan,CoinElement **coins);

void check_if_exit(PacManElement *pacMan);
void dest_pacman();


#endif // PACMAN_H_INCLUDED
