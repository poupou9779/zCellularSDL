#include <SDL/SDL.h>
#include "def.h"
#include <time.h>

int main(int argc, char *argv[])
{
    int i;
    SDL_bool _loop = SDL_TRUE;
    context_t ctxt;
    SDL_Event e;
    Uint32 lasttime = SDL_GetTicks(),
            newtime;

    srand((unsigned int)time(NULL));
    if(SDL_Init(SDL_INIT_VIDEO) == -1)
    {
        fprintf(stderr, SDL_GetError());
        exit(-1);
    }
    atexit(SDL_Quit);
    SDL_WM_SetCaption(NAME, NULL);

    init_ct(&ctxt);

    BlitAll(&ctxt);
    SDL_Flip(ctxt.screen);
    while(_loop)
    {
        newtime = SDL_GetTicks();
        if(newtime-lasttime < ELAPSE)
        {
            SDL_Delay(ELAPSE);
        }
        else
        {
            while(SDL_PollEvent(&e))
                if(e.type == SDL_MOUSEBUTTONDOWN)
                    _loop = SDL_FALSE;
            NextGen(&ctxt);
            BlitAll(&ctxt);
            SDL_Flip(ctxt.screen);
            lasttime = newtime;
        }
        SDL_Delay(1);
    }
    for(i = 0; i < NB_STATES; ++i)
        SDL_FreeSurface(ctxt.colors[i]);
    for(i = 0; i < NB_CELLS_Y; ++i)
    {
        free(ctxt.map[i]);
        free(ctxt.tmp[i]);
    }
    free(ctxt.map);
    free(ctxt.tmp);
    return 0;
    (void)argc;
    (void)argv;
}
