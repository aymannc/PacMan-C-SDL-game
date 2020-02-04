#include "pacman.h"

int main(int argc, char* argv[])
{
    SDL_Window *window=NULL;
    SDL_Renderer *renderer=NULL;
    WallElement *wall=NULL;
    CoinElement *coins=NULL;
    GhostElement *ghosts=NULL;
    PacManElement *pacMan=NULL;
    TextElement *textlist=NULL;
    SDL_Event windowEvent;
    int pacman_direction=0;
    int is_moved;
    Mix_Music  *backgroundSound ;
    Mix_Chunk *moveEffect ;
    int lastTime =0;
    int beg =0;

    int init_resp=INIT_ALL(window,&renderer);

    if(init_resp<0)
    {
        SDL_Log("\nError in INIT : %d",init_resp);
        exit(init_resp);
    }

    if(play_window(window,renderer,&beg))
    {

        create_map(&wall);
        create_coins(&coins,wall);
        create_ghosts(&ghosts);
        create_texts(&textlist,pacMan);
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        pacMan=createPacManElement(0,17*PACMAN_DEM);
        draw_coins(&coins,renderer);

        SDL_SetRenderDrawColor(renderer,33, 33, 222,255);
        draw_map(&wall,renderer);

        draw_lives(renderer,pacMan);
        draw_texts(&textlist,renderer);
        draw_pacman(2,renderer,pacMan);
        draw_ghosts(&ghosts,renderer);
        moveEffect = Mix_LoadWAV("res/sounds/pacman_eatfruit.wav");
        while(2)
        {

            is_moved=0;
            if(SDL_PollEvent(&windowEvent))
            {
                if(windowEvent.type==SDL_QUIT)
                    break;

                if(SDL_KEYDOWN==windowEvent.type && windowEvent.key.repeat == 0)
                {
                    int x=pacMan->pacMan->x;
                    int y=pacMan->pacMan->y;
                    switch(windowEvent.key.keysym.sym)
                    {
                    case SDLK_LEFT:
                        if(CollisionByX(pacMan->pacMan->x-SPEED,pacMan->pacMan->y,wall))
                        {
                            pacMan->pacMan->x=pacMan->pacMan->x-SPEED;
                            pacman_direction=1;
                            is_moved=1;
                        };
                        break;
                    case SDLK_RIGHT:
                        if(CollisionByX(pacMan->pacMan->x+SPEED,pacMan->pacMan->y,wall))
                        {
                            pacman_direction=2;
                            pacMan->pacMan->x=pacMan->pacMan->x+SPEED;
                            is_moved=1;
                        };
                        break;
                    case SDLK_UP:
                        if(CollisionByY(pacMan->pacMan->x, pacMan->pacMan->y-SPEED, wall))
                        {
                            pacman_direction=3;
                            pacMan->pacMan->y=pacMan->pacMan->y-SPEED;
                            is_moved=1;
                        };
                        break;
                    case SDLK_DOWN:
                        if(CollisionByY(pacMan->pacMan->x, pacMan->pacMan->y+SPEED, wall))
                        {
                            pacman_direction=4;
                            pacMan->pacMan->y=pacMan->pacMan->y+SPEED;
                            is_moved=1;
                        };
                        break;
                    }

                    if(is_moved)
                    {
                        check_if_exit(pacMan);
                        pacMan->pacMan_prev->x=x;
                        pacMan->pacMan_prev->y=y;
                        eat_coin(pacMan,&coins);
                        Mix_PlayChannel(-1, moveEffect, 0);
                        draw_pacman(pacman_direction,renderer,pacMan);
                        update_score(pacMan,renderer);
                    }

                }
            }
            update_timer(renderer,&lastTime,beg);
            SDL_RenderPresent(renderer);

        };
    };
    dest_pacman(&window,&renderer);

    return 0;
}
