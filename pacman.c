#include "pacman.h"
int INIT_ALL(SDL_Window *window,SDL_Renderer **renderer)
{
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
    {
        SDL_Log("%s", SDL_GetError());
        return -1;
    }
    if (IMG_Init(IMG_INIT_PNG)!=IMG_INIT_PNG)
    {
        SDL_Log("Fialed to init image \n");
        return -2 ;
    }
    if (TTF_Init()==-1)
    {
        SDL_Log("Fialed to init TTF \n");
        return -3 ;
    }
    window = SDL_CreateWindow(
                 "PacMan BDCC",
                 SDL_WINDOWPOS_UNDEFINED,
                 SDL_WINDOWPOS_UNDEFINED,
                 xBlocks*PACMAN_DEM,
                 yBlocks*PACMAN_DEM,
                 SDL_WINDOW_OPENGL
             );

    if (window == NULL)
    {
        printf("Could not create window: %s\n", SDL_GetError());
        return -1;
    }
    *renderer=SDL_CreateRenderer(window,-1,SDL_RENDERER_ACCELERATED);

    if (renderer == NULL)
    {
        printf("Could not create renderer: %s\n", SDL_GetError());
        return -2;
    }
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);
    return 1;
}
int play_window(SDL_Window *window,SDL_Renderer *renderer,int *lastTime)
{

    SDL_Rect *button=(SDL_Rect*)malloc(sizeof(SDL_Rect));
    button->x=((xBlocks-5)/2)*PACMAN_DEM;
    button->y=((yBlocks-8)/2)*PACMAN_DEM;
    button->h=2*PACMAN_DEM;
    button->w=4*PACMAN_DEM;

    int img_size=3;
    SDL_Rect *img_holder=(SDL_Rect*)malloc(sizeof(SDL_Rect));
    img_holder->x=((xBlocks/2)+6.5)*PACMAN_DEM;
    img_holder->y=5*PACMAN_DEM;
    img_holder->h=PACMAN_DEM;
    img_holder->w=PACMAN_DEM;


    ///Select the color for drawing. It is set to red here.
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    /// Clear the entire screen to our selected color.
    SDL_RenderClear(renderer);

    draw_image("res/img/coin.png",renderer,img_holder);

    img_holder->x=((xBlocks/2)+3)*PACMAN_DEM;
    img_holder->y=4*PACMAN_DEM;
    img_holder->h=img_size*PACMAN_DEM;
    img_holder->w=img_size*PACMAN_DEM;

    draw_image("res/img/pacman_right.png",renderer,img_holder);
    img_holder->x=((xBlocks/2))*PACMAN_DEM;
    draw_image("res/img/ghost_red_right.png",renderer,img_holder);
    img_holder->x=((xBlocks/2)-3)*PACMAN_DEM;
    draw_image("res/img/ghost_blue_right.png",renderer,img_holder);
    img_holder->x=((xBlocks/2)-6)*PACMAN_DEM;
    draw_image("res/img/ghost_pink_right.png",renderer,img_holder);
    img_holder->x=((xBlocks/2)-9)*PACMAN_DEM;
    draw_image("res/img/ghost_yellow_right.png",renderer,img_holder);

    SDL_Color color= {255,255,255};
    TextElement *pacman=createTextElement(((xBlocks/2)-5.5)*PACMAN_DEM,8*PACMAN_DEM,3*PACMAN_DEM,10*PACMAN_DEM,"BDCC GAME","res/fonts/arial.ttf",200,color);
    TextElement *Play=createTextElement(button->x,button->y,button->h,button->w,"Play","res/fonts/arial.ttf",200,color);
    TextElement *Score=createTextElement(button->x,button->y+1.5*button->h,button->h,button->w+PACMAN_DEM,"Score","res/fonts/arial.ttf",200,color);
    TextElement *Settings=createTextElement(button->x,button->y+(3*button->h),button->h,button->w+2*PACMAN_DEM,"Settings","res/fonts/arial.ttf",200,color);
    TextElement *Exit=createTextElement(button->x,button->y+(4.5*button->h),button->h,button->w,"Exit","res/fonts/arial.ttf",200,color);


    draw_text(pacman,renderer);
    draw_text(Play,renderer);
    draw_text(Score,renderer);
    draw_text(Settings,renderer);
    draw_text(Exit,renderer);
    ///Up until now everything was drawn behind the scenes.This will show the new, red contents of the window.
    SDL_RenderPresent(renderer);
    Mix_Music  *backgroundSound ;
    Mix_Chunk *moveEffect ;
    backgroundSound = Mix_LoadMUS("res/sounds/pacman_beginning.wav");
    moveEffect = Mix_LoadWAV("res/sounds/pacman_eatfruit.wav");
    Mix_PlayMusic(backgroundSound, 0);

    SDL_Event windowEvent;
    int is_played=0;
    while(2)
    {
        if(SDL_PollEvent(&windowEvent))
        {
            if(windowEvent.type==SDL_QUIT)
                exit(1);

            if(windowEvent.type == SDL_MOUSEBUTTONDOWN)
            {
                if((Play->text_holder->x < windowEvent.button.x)&& (windowEvent.button.x< Play->text_holder->x+Play->text_holder->w )
                        &&(Play->text_holder->y < windowEvent.button.y)&& (windowEvent.button.y< Play->text_holder->y +Play->text_holder->h))
                {
                    *lastTime=SDL_GetTicks();
                    break;
                }

            }
            if(windowEvent.type ==SDL_MOUSEMOTION)
            {
                if((Play->text_holder->x < windowEvent.button.x)&& (windowEvent.button.x< Play->text_holder->x+Play->text_holder->w )
                        &&(Play->text_holder->y < windowEvent.button.y)&& (windowEvent.button.y< Play->text_holder->y +Play->text_holder->h))
                {

                    if(!is_played)
                        Mix_PlayChannel(-1, moveEffect, 0);
                    is_played=1;
                    SDL_Color color= {33, 33, 222};
                    Play->color=color;
                    draw_text(Play,renderer);
                    SDL_RenderPresent(renderer);

                    continue;
                }
            }
            SDL_Color color= {255,255,255};
            Play->color=color;
            draw_text(Play,renderer);
            SDL_RenderPresent(renderer);
             is_played=0;

        }

    }
    return 1;
}
PacManElement *createPacManElement(int x,int y)
{
    SDL_Rect *pacMan=(SDL_Rect*)malloc(sizeof(SDL_Rect));
    pacMan->x=x;
    pacMan->y=y;
    pacMan->h=PACMAN_DEM;
    pacMan->w=PACMAN_DEM;

    SDL_Rect *pacMan_prev=(SDL_Rect*)malloc(sizeof(SDL_Rect));
    pacMan_prev->x=-PACMAN_DEM;
    pacMan_prev->y=-PACMAN_DEM;
    pacMan_prev->h=PACMAN_DEM;
    pacMan_prev->w=PACMAN_DEM;


    PacManElement *newElement;
    newElement=(PacManElement*)malloc(sizeof(PacManElement));

    newElement->pacMan=pacMan;
    newElement->pacMan_prev=pacMan_prev;

    newElement->lives=3;
    newElement->lvl=1;
    newElement->score=0;
    newElement->next=NULL;
    return newElement;
}

WallElement *createWallElement(int x,int y)
{

    SDL_Rect *wall=(SDL_Rect*)malloc(sizeof(SDL_Rect));
    wall->x=x;
    wall->y=y;
    wall->h=PACMAN_DEM;
    wall->w=PACMAN_DEM;

    WallElement *newElement;
    newElement=(WallElement*)malloc(sizeof(WallElement));
    newElement->wall=wall;
    newElement->next=NULL;
    return newElement;
}
CoinElement *createCoinElement(int x,int y)
{
    SDL_Rect *coin=(SDL_Rect*)malloc(sizeof(SDL_Rect));
    coin->x=x;
    coin->y=y;
    coin->h=COIN_DEM;
    coin->w=COIN_DEM;

    CoinElement *newElement;
    newElement=(CoinElement*)malloc(sizeof(CoinElement));
    newElement->coin=coin;
    newElement->next=NULL;
    return newElement;
}
GhostElement *createGhostElement(int x,int y,int ghost_color,int direction)
{
    SDL_Rect *ghost=(SDL_Rect*)malloc(sizeof(SDL_Rect));
    ghost->x=x;
    ghost->y=y;
    ghost->h=PACMAN_DEM;
    ghost->w=PACMAN_DEM;

    SDL_Rect *ghost_prev=(SDL_Rect*)malloc(sizeof(SDL_Rect));
    ghost_prev->x=-PACMAN_DEM;
    ghost_prev->y=-PACMAN_DEM;
    ghost_prev->h=PACMAN_DEM;
    ghost_prev->w=PACMAN_DEM;

    GhostElement *newElement;
    newElement=(GhostElement*)malloc(sizeof(GhostElement));
    newElement->ghost=ghost;
    newElement->ghost_prev=ghost_prev;
    newElement->ghost_color=ghost_color;
    newElement->direction=direction;
    newElement->next=NULL;
    return newElement;
}
TextElement *createTextElement(int x,int y,int h,int w,char *message,char *font_path,int font_size,SDL_Color color)
{
    SDL_Rect *text_holder=(SDL_Rect*)malloc(sizeof(SDL_Rect));
    text_holder->x=x;
    text_holder->y=y;
    text_holder->h=h;
    text_holder->w=w;

    TextElement *newElement;
    newElement=(TextElement*)malloc(sizeof(TextElement));
    strcpy(newElement->message,message);
    strcpy(newElement->font_path,font_path);
    newElement->font_size=font_size;
    newElement->color=color;
    newElement->text_holder=text_holder;
    newElement->next=NULL;
    return newElement;
}
void insertWallAtBegining (WallElement **wallElement, WallElement *wall)
{
    wall->next=*wallElement;
    *wallElement=wall;
}
void insertCoinAtBegining (CoinElement **coinElement, CoinElement *coin)
{
    coin->next=*coinElement;
    *coinElement=coin;
}
void insertGhostAtBegining (GhostElement **ghostElement, GhostElement *ghost)
{
    ghost->next=*ghostElement;
    *ghostElement=ghost;
}
void insertTextAtBegining (TextElement **textElement, TextElement *text)
{
    text->next=*textElement;
    *textElement=text;
}

void create_line(WallElement **wallE,int size,int x_pos,int y_pos)
{
    int i;
    for(i=0; i<size; i++)
    {
        insertWallAtBegining(wallE,createWallElement(x_pos+(i*PACMAN_DEM),y_pos));
    }
}
void create_col(WallElement **wallE,int size,int x_pos,int y_pos)
{
    int i;
    for(i=0; i<size; i++)
    {
        insertWallAtBegining(wallE,createWallElement(x_pos,y_pos+(i*PACMAN_DEM)));
    }
}
void create_border(WallElement **wallE)
{
    create_line(wallE,28,0,3*PACMAN_DEM);
    create_line(wallE,28,0,33*PACMAN_DEM);

    create_line(wallE,5,PACMAN_DEM,12*PACMAN_DEM);
    create_line(wallE,5,22*PACMAN_DEM,12*PACMAN_DEM);

    create_line(wallE,6,0*PACMAN_DEM,16*PACMAN_DEM);
    create_line(wallE,6,22*PACMAN_DEM,16*PACMAN_DEM);

    create_line(wallE,6,0*PACMAN_DEM,18*PACMAN_DEM);
    create_line(wallE,6,22*PACMAN_DEM,18*PACMAN_DEM);

    create_line(wallE,6,0*PACMAN_DEM,22*PACMAN_DEM);
    create_line(wallE,6,22*PACMAN_DEM,22*PACMAN_DEM);

    create_line(wallE,3,0*PACMAN_DEM,27*PACMAN_DEM);
    create_line(wallE,3,25*PACMAN_DEM,27*PACMAN_DEM);
    create_line(wallE,3,0*PACMAN_DEM,28*PACMAN_DEM);
    create_line(wallE,3,25*PACMAN_DEM,28*PACMAN_DEM);

    create_col(wallE,9,0,4*PACMAN_DEM);
    create_col(wallE,9,27*PACMAN_DEM,4*PACMAN_DEM);
    create_col(wallE,4,0,23*PACMAN_DEM);
    create_col(wallE,4,0,29*PACMAN_DEM);
    create_col(wallE,4,27*PACMAN_DEM,23*PACMAN_DEM);
    create_col(wallE,4,27*PACMAN_DEM,29*PACMAN_DEM);

    create_col(wallE,4,5*PACMAN_DEM,13*PACMAN_DEM);
    create_col(wallE,4,22*PACMAN_DEM,13*PACMAN_DEM);
    create_col(wallE,4,5*PACMAN_DEM,19*PACMAN_DEM);
    create_col(wallE,4,22*PACMAN_DEM,19*PACMAN_DEM);

}
void create_inside(WallElement **wallE)
{

    create_col(wallE,4,13*PACMAN_DEM,4*PACMAN_DEM);
    create_col(wallE,4,14*PACMAN_DEM,4*PACMAN_DEM);

    create_col(wallE,3,2*PACMAN_DEM,5*PACMAN_DEM);
    create_col(wallE,3,5*PACMAN_DEM,5*PACMAN_DEM);
    create_col(wallE,3,7*PACMAN_DEM,5*PACMAN_DEM);
    create_col(wallE,3,11*PACMAN_DEM,5*PACMAN_DEM);
    create_col(wallE,3,16*PACMAN_DEM,5*PACMAN_DEM);
    create_col(wallE,3,20*PACMAN_DEM,5*PACMAN_DEM);
    create_col(wallE,3,22*PACMAN_DEM,5*PACMAN_DEM);
    create_col(wallE,3,25*PACMAN_DEM,5*PACMAN_DEM);

    create_col(wallE,8,7*PACMAN_DEM,9*PACMAN_DEM);
    create_col(wallE,8,8*PACMAN_DEM,9*PACMAN_DEM);
    create_col(wallE,8,19*PACMAN_DEM,9*PACMAN_DEM);
    create_col(wallE,8,20*PACMAN_DEM,9*PACMAN_DEM);

    create_col(wallE,2,13*PACMAN_DEM,11*PACMAN_DEM);
    create_col(wallE,2,14*PACMAN_DEM,11*PACMAN_DEM);

    create_col(wallE,3,10*PACMAN_DEM,16*PACMAN_DEM);
    create_col(wallE,3,17*PACMAN_DEM,16*PACMAN_DEM);

    create_col(wallE,5,7*PACMAN_DEM,18*PACMAN_DEM);
    create_col(wallE,5,8*PACMAN_DEM,18*PACMAN_DEM);
    create_col(wallE,5,19*PACMAN_DEM,18*PACMAN_DEM);
    create_col(wallE,5,20*PACMAN_DEM,18*PACMAN_DEM);
    create_col(wallE,3,13*PACMAN_DEM,23*PACMAN_DEM);
    create_col(wallE,3,14*PACMAN_DEM,23*PACMAN_DEM);

    create_col(wallE,3,4*PACMAN_DEM,26*PACMAN_DEM);
    create_col(wallE,3,5*PACMAN_DEM,26*PACMAN_DEM);
    create_col(wallE,3,22*PACMAN_DEM,26*PACMAN_DEM);
    create_col(wallE,3,23*PACMAN_DEM,26*PACMAN_DEM);

    create_col(wallE,3,7*PACMAN_DEM,27*PACMAN_DEM);
    create_col(wallE,3,8*PACMAN_DEM,27*PACMAN_DEM);
    create_col(wallE,3,19*PACMAN_DEM,27*PACMAN_DEM);
    create_col(wallE,3,20*PACMAN_DEM,27*PACMAN_DEM);

    create_col(wallE,3,13*PACMAN_DEM,29*PACMAN_DEM);
    create_col(wallE,3,14*PACMAN_DEM,29*PACMAN_DEM);

    create_line(wallE,2,3*PACMAN_DEM,5*PACMAN_DEM);
    create_line(wallE,3,8*PACMAN_DEM,5*PACMAN_DEM);
    create_line(wallE,3,17*PACMAN_DEM,5*PACMAN_DEM);
    create_line(wallE,2,23*PACMAN_DEM,5*PACMAN_DEM);

    create_line(wallE,2,3*PACMAN_DEM,7*PACMAN_DEM);
    create_line(wallE,3,8*PACMAN_DEM,7*PACMAN_DEM);
    create_line(wallE,3,17*PACMAN_DEM,7*PACMAN_DEM);
    create_line(wallE,2,23*PACMAN_DEM,7*PACMAN_DEM);

    create_line(wallE,4,2*PACMAN_DEM,9*PACMAN_DEM);
    create_line(wallE,4,2*PACMAN_DEM,10*PACMAN_DEM);
    create_line(wallE,8,10*PACMAN_DEM,9*PACMAN_DEM);
    create_line(wallE,8,10*PACMAN_DEM,10*PACMAN_DEM);
    create_line(wallE,4,22*PACMAN_DEM,9*PACMAN_DEM);
    create_line(wallE,4,22*PACMAN_DEM,10*PACMAN_DEM);

    create_line(wallE,3,9*PACMAN_DEM,12*PACMAN_DEM);
    create_line(wallE,3,9*PACMAN_DEM,13*PACMAN_DEM);
    create_line(wallE,3,16*PACMAN_DEM,12*PACMAN_DEM);
    create_line(wallE,3,16*PACMAN_DEM,13*PACMAN_DEM);

    create_line(wallE,3,10*PACMAN_DEM,15*PACMAN_DEM);
    create_line(wallE,3,15*PACMAN_DEM,15*PACMAN_DEM);
    create_line(wallE,8,10*PACMAN_DEM,19*PACMAN_DEM);

    create_line(wallE,8,10*PACMAN_DEM,21*PACMAN_DEM);
    create_line(wallE,8,10*PACMAN_DEM,22*PACMAN_DEM);

    create_line(wallE,4,2*PACMAN_DEM,24*PACMAN_DEM);
    create_line(wallE,4,22*PACMAN_DEM,24*PACMAN_DEM);
    create_line(wallE,5,7*PACMAN_DEM,24*PACMAN_DEM);
    create_line(wallE,5,16*PACMAN_DEM,24*PACMAN_DEM);
    create_line(wallE,4,2*PACMAN_DEM,25*PACMAN_DEM);
    create_line(wallE,4,22*PACMAN_DEM,25*PACMAN_DEM);
    create_line(wallE,5,7*PACMAN_DEM,25*PACMAN_DEM);
    create_line(wallE,5,16*PACMAN_DEM,25*PACMAN_DEM);

    create_line(wallE,8,10*PACMAN_DEM,27*PACMAN_DEM);
    create_line(wallE,8,10*PACMAN_DEM,28*PACMAN_DEM);
    create_line(wallE,10,2*PACMAN_DEM,30*PACMAN_DEM);
    create_line(wallE,10,2*PACMAN_DEM,31*PACMAN_DEM);
    create_line(wallE,10,16*PACMAN_DEM,30*PACMAN_DEM);
    create_line(wallE,10,16*PACMAN_DEM,31*PACMAN_DEM);

}
SDL_Texture *initialize_font_from_file(TextElement *text,SDL_Renderer *renderer)
{
    TTF_Font *font = TTF_OpenFont(text->font_path,text->font_size);
    if(!font)
    {
        SDL_Log("Fialed to load font \n");
        exit(-5);
    }
    SDL_Surface * text_surface = TTF_RenderText_Solid(font,text->message,text->color);
    if(!text_surface)
    {
        SDL_Log("Fialed to ceate text_surface \n");
        exit(-6);
    }
    SDL_Texture * text_texture = SDL_CreateTextureFromSurface(renderer,text_surface);
    if(!text_texture)
    {
        SDL_Log("Fialed to ceate text_texture \n");
        exit(-5);
    }
    SDL_FreeSurface(text_surface);
    return text_texture;
}
void draw_text(TextElement *text,SDL_Renderer *renderer)
{
    SDL_Texture * text_texture = initialize_font_from_file(text,renderer);
    SDL_QueryTexture(text_texture, NULL, NULL,NULL,NULL);

    SDL_RenderCopy(renderer, text_texture, NULL,text->text_holder);
    SDL_DestroyTexture(text_texture);
}
void draw_texts(TextElement **text,SDL_Renderer *renderer)
{
    TextElement *pt;
    for(pt=(*text); pt!=NULL; pt=pt->next)
    {
        draw_text(pt,renderer);
    }
}
SDL_Texture *initialize_IMG_from_file( char* file_name, SDL_Renderer *renderer)
{
    SDL_Surface *image = IMG_Load(file_name);
    SDL_Texture * image_texture = SDL_CreateTextureFromSurface(renderer, image);
    SDL_FreeSurface(image);
    return image_texture;
}
void draw_image(char *path,SDL_Renderer *renderer,SDL_Rect *holder)
{

    SDL_Texture * image_texture = initialize_IMG_from_file(path, renderer);
    SDL_QueryTexture(image_texture, NULL, NULL,NULL,NULL);
    SDL_RenderCopy(renderer, image_texture, NULL,holder);
    SDL_DestroyTexture(image_texture);
}
void draw_map(WallElement **wallE,SDL_Renderer *renderer)
{
    WallElement *pt;
    for(pt=(*wallE); pt!=NULL; pt=pt->next)
    {
        SDL_RenderFillRect(renderer,pt->wall);
    }
}
void draw_coins(CoinElement **coinE,SDL_Renderer *renderer)
{
    CoinElement *pt;
    for(pt=(*coinE); pt!=NULL; pt=pt->next)
    {
        draw_image("res/img/coin.png",renderer,pt->coin);
    }
}
void draw_lives(SDL_Renderer *renderer,PacManElement *pacman)
{
    SDL_Rect holder= {0,(yBlocks-2)*PACMAN_DEM+7,1.5*PACMAN_DEM,1.5*PACMAN_DEM};
    int i=0;
    for(i=0; i<pacman->lives-1; i++)
    {
        holder.x=i*1.5*PACMAN_DEM;
        draw_image("res/img/pacman_right.png",renderer,&holder);
    }
}
void draw_ghosts(GhostElement **ghostE,SDL_Renderer *renderer)
{
    GhostElement *pt;
    char path[50];
    for(pt=(*ghostE); pt!=NULL; pt=pt->next)
    {
        switch(pt->ghost_color)
        {
        case 0:
            if(pt->direction)
                strcpy(path,"res/img/ghost_red_right.png");
            else
                strcpy(path,"res/img/ghost_red_left.png");
            break;
        case 1:
            if(pt->direction)
                strcpy(path,"res/img/ghost_blue_right.png");
            else
                strcpy(path,"res/img/ghost_blue_left.png");
            break;
        case 2:
            if(pt->direction)
                strcpy(path,"res/img/ghost_pink_right.png");
            else
                strcpy(path,"res/img/ghost_pink_left.png");
            break;
        case 3:
            if(pt->direction)
                strcpy(path,"res/img/ghost_yellow_right.png");
            else
                strcpy(path,"res/img/ghost_yellow_left.png");
            break;
        }
        draw_image(path,renderer,pt->ghost);
    }
}

/// Fonction that test if the object is allowed to move or not by priorite of Cordinate X
int CollisionByX(int x, int y, WallElement *walE)
{
    WallElement *pt;
    for(pt=walE; pt!=NULL; pt=pt->next)
    {
        if(x == pt->wall->x )
        {
            if(y == pt->wall->y)
                return 0;
        }
    }
    return 1;
}
/// Fonction that test if the object is allowed to move or not by priorite of Cordinate X
int CollisionByY(int x, int y, WallElement *walE)
{
    WallElement *pt;
    for(pt=walE; pt!=NULL; pt=pt->next)
    {
        if(y == pt->wall->y)
        {
            if(x == pt->wall->x )
                return 0;
        }
    }
    return 1;
}
void delete_coin(int x,int y,CoinElement **coins)
{
    if(*coins==NULL)
    {
        return;
    }

    else if((x==(*coins)->coin->x)&&(y==(*coins)->coin->y))
    {
        CoinElement *p=*coins;
        *coins=(*coins)->next;
        free(p);
        return;
    }
    else
    {
        CoinElement *ptr=*coins;
        CoinElement *suiv=ptr->next;
        while(suiv!=NULL)
        {
            if((x==suiv->coin->x)&&(y==suiv->coin->y))
            {
                ptr->next=suiv->next;
                free(suiv);
                return;
            }
            ptr=suiv;
            suiv=suiv->next;
        }
        return;

    }
}
void create_coins(CoinElement **coinElement,WallElement *wall)
{
    int x,y,marging=5;
    for(x=1; x<xBlocks-1; x++)
        for(y=4; y<yBlocks-3; y++)
        {
            insertCoinAtBegining(coinElement,createCoinElement((x*PACMAN_DEM)+marging,(y*PACMAN_DEM)+marging));
        }
    for(x=1; x<6; x++)
        for(y=13; y<16; y++)
        {
            delete_coin((x*PACMAN_DEM)+marging,(y*PACMAN_DEM)+marging,coinElement);
        }
    for(x=23; x<27; x++)
        for(y=13; y<16; y++)
        {
            delete_coin((x*PACMAN_DEM)+marging,(y*PACMAN_DEM)+marging,coinElement);
        }
    for(x=1; x<6; x++)
        for(y=19; y<22; y++)
        {
            delete_coin((x*PACMAN_DEM)+marging,(y*PACMAN_DEM)+marging,coinElement);
        }
    for(x=23; x<27; x++)
        for(y=19; y<22; y++)
        {
            delete_coin((x*PACMAN_DEM)+marging,(y*PACMAN_DEM)+marging,coinElement);
        }
    for(x=11; x<17; x++)
        for(y=16; y<19; y++)
        {
            delete_coin((x*PACMAN_DEM)+marging,(y*PACMAN_DEM)+marging,coinElement);
        }
    for(x=3; x<5; x++)
        delete_coin((x*PACMAN_DEM)+marging,(6*PACMAN_DEM)+marging,coinElement);
    for(x=8; x<11; x++)
        delete_coin((x*PACMAN_DEM)+marging,(6*PACMAN_DEM)+marging,coinElement);
    for(x=17; x<20; x++)
        delete_coin((x*PACMAN_DEM)+marging,(6*PACMAN_DEM)+marging,coinElement);
    for(x=23; x<25; x++)
        delete_coin((x*PACMAN_DEM)+marging,(6*PACMAN_DEM)+marging,coinElement);


}
void create_ghosts(GhostElement **ghostElement)
{
    insertGhostAtBegining(ghostElement,createGhostElement(11*PACMAN_DEM +13,17*PACMAN_DEM+10,0,0));
    insertGhostAtBegining(ghostElement,createGhostElement(13*PACMAN_DEM+13,17*PACMAN_DEM+10,1,1));
    insertGhostAtBegining(ghostElement,createGhostElement(13*PACMAN_DEM+13,15*PACMAN_DEM+18,2,0));
    insertGhostAtBegining(ghostElement,createGhostElement(15*PACMAN_DEM+13,17*PACMAN_DEM+10,3,1));
}
void create_texts(TextElement **textElement,PacManElement *pacman)
{

    SDL_Color color= {255,255,255};
    insertTextAtBegining(textElement,createTextElement(0,0,2*PACMAN_DEM,3*PACMAN_DEM,"Score :","res/fonts/arial.ttf",40,color));
    insertTextAtBegining(textElement,createTextElement(4*PACMAN_DEM,8,30,15,"0","res/fonts/arial.ttf",40,color));
    insertTextAtBegining(textElement,createTextElement((xBlocks-8)*PACMAN_DEM,0,2*PACMAN_DEM,3*PACMAN_DEM,"Timer :","res/fonts/arial.ttf",40,color));
    insertTextAtBegining(textElement,createTextElement((xBlocks-4)*PACMAN_DEM,0,2*PACMAN_DEM,3*PACMAN_DEM,"00:00","res/fonts/arial.ttf",40,color));

}
void update_score(PacManElement *pacman,SDL_Renderer* renderer)
{
    ///draw black screen on each time to remove older scores
    SDL_Rect rect= {4*PACMAN_DEM,0,3*PACMAN_DEM,3*PACMAN_DEM};
    SDL_SetRenderDrawColor( renderer, 0, 0, 0, 255 );
    SDL_RenderFillRect( renderer,&rect);
    SDL_RenderPresent(renderer);

    char score[10];
    sprintf(score, "%d", pacman->score);

    int score_scal=1;
    if((pacman->score/100)!=0)
        score_scal=2;
    else if((pacman->score/1000)!=0)
        score_scal=3;
    else if((pacman->score/10000)!=0)
        score_scal=4;
    SDL_Color color= {255,255,255};

    draw_text(createTextElement(4*PACMAN_DEM,0,2*PACMAN_DEM,score_scal*PACMAN_DEM,score,"res/fonts/arial.ttf",40,color),renderer);
}
void update_timer(SDL_Renderer *renderer,int *lastTime,int beg)
{
    int currentTime = SDL_GetTicks()-beg;
    int sec=currentTime/1000;
    int min=0;
    if (currentTime > *lastTime + 1000)
    {

        SDL_Rect rect= {(xBlocks-4)*PACMAN_DEM,0,3*PACMAN_DEM,3*PACMAN_DEM};
        SDL_SetRenderDrawColor( renderer, 0, 0, 0, 255 );
        SDL_RenderFillRect( renderer,&rect);
        SDL_RenderPresent(renderer);

        min=sec/60;
        sec=sec%60;
        char timer[10];
        sprintf(timer, "%s%d:%s%d",(min>9)?"":"0",min,(sec>9)?"":"0",sec);

        SDL_Color color= {255,255,255};

        draw_text(createTextElement((xBlocks-4)*PACMAN_DEM,0,2*PACMAN_DEM,3*PACMAN_DEM,timer,"res/fonts/arial.ttf",40,color),renderer);
        *lastTime = currentTime;
    }
}
void create_map(WallElement **wallE)
{
    create_border(wallE);
    create_inside(wallE);

}
void show_wall_cord(WallElement *wall)
{
    WallElement *pt;
    int i=0;
    for(pt=wall; pt!=NULL; pt=pt->next)
    {
        i++;
        printf("\n%d-%d",pt->wall->x,pt->wall->y);
    }
    printf("\n size : %d",i);
}
void show_coins_cord(CoinElement *coin)
{
    printf("\n coins_cord");
    CoinElement *pt;
    int i=0;
    for(pt=coin; pt!=NULL; pt=pt->next)
    {
        i++;
        printf("\n%d-%d",pt->coin->x,pt->coin->y);
    }
    printf("\n coins size : %d",i);
}
void show_ghosts_cord(GhostElement *ghost)
{
    printf("\n ghosts_cord");
    GhostElement *pt;
    int i=0;
    for(pt=ghost; pt!=NULL; pt=pt->next)
    {
        i++;
        printf("\n%d-%d",pt->ghost->x,pt->ghost->y);
    }
    printf("\n ghosts size : %d",i);
}

void eat_coin(PacManElement *pacMan,CoinElement **coins)
{
    int x=pacMan->pacMan->x,y=pacMan->pacMan->y;
    if(*coins==NULL)
    {
        printf("\nCoin list is empty");
        return;
    }

    else if( (x<=(*coins)->coin->x)&& ((x+PACMAN_DEM)>=(*coins)->coin->x)&&(y<=(*coins)->coin->y)&&((y+PACMAN_DEM)>=(*coins)->coin->y) )
    {
        CoinElement *p=*coins;
        *coins=(*coins)->next;
        free(p);
        pacMan->score+=10;

        return;
    }
    else
    {
        CoinElement *ptr=*coins;
        CoinElement *suiv=ptr->next;
        while(suiv!=NULL)
        {
            if( (x<suiv->coin->x)&& ((x+PACMAN_DEM)>suiv->coin->x)&&(y<=suiv->coin->y)&&((y+PACMAN_DEM)>=suiv->coin->y) )
            {
                ptr->next=suiv->next;
                free(suiv);
                pacMan->score+=10;
                return;
            }
            ptr=suiv;
            suiv=suiv->next;
        }
        return;

    }
}

void draw_pacman(int direction, SDL_Renderer *renderer,PacManElement *pacman)
{
    char path[50];
    switch(direction)
    {
    case 1 :
        strcpy(path,"res/img/pacman_left.png");
        break;
    case 2 :
        strcpy(path,"res/img/pacman_right.png");
        break;
    case 3 :
        strcpy(path,"res/img/pacman_up.png");
        break;
    case 4 :
        strcpy(path,"res/img/pacman_down.png");
        break;
    default :
        strcpy(path,"res/img/pacman_left.png");
    }
    draw_image(path,renderer,pacman->pacMan);
    SDL_SetRenderDrawColor(renderer,0, 0, 0,255);
    SDL_RenderFillRect(renderer,pacman->pacMan_prev);

}
void check_if_exit(PacManElement *pacMan)
{
    if(pacMan->pacMan->y==17*PACMAN_DEM)
    {
        if(pacMan->pacMan->x==(-PACMAN_DEM))
        {
            pacMan->pacMan->x=27*PACMAN_DEM;
            SDL_Delay(50);
        }
        else if (pacMan->pacMan->x==28*PACMAN_DEM)
        {
            pacMan->pacMan->x=0;
            SDL_Delay(50);
        }
    }

}
void dest_pacman(SDL_Window **window,SDL_Renderer **renderer)
{
    IMG_Quit();
    SDL_DestroyRenderer(*renderer);
    SDL_DestroyWindow(*window);
    SDL_QuitSubSystem(SDL_INIT_TIMER);
    SDL_Quit();
}
