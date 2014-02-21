#include "def.h"
#include <limits.h>

Uint32 colors[NB_STATES] = {0x000000,
                            0x002266,
                            0xFF0000,
                            0x0000FF,
                            0x00FF00};

void init_ct(context_t *ctxt)
{
    int i, j;
    ctxt->map = malloc(NB_CELLS_Y * sizeof(*ctxt->map));
    ctxt->tmp = malloc(NB_CELLS_Y * sizeof(*ctxt->tmp));
    if(ctxt->map == NULL || ctxt->tmp == NULL)
    {
        fprintf(stderr, "malloc error #1");
        exit(-1);
    }
    for(i = 0; i < NB_CELLS_Y; ++i)
    {
        ctxt->map[i] = malloc(NB_CELLS_X * sizeof(**ctxt->map));
        ctxt->tmp[i] = malloc(NB_CELLS_X * sizeof(**ctxt->tmp));
        if(ctxt->map[i] == NULL)
        {
            fprintf(stderr, "malloc error #2 (%d)", i);
            exit(-1);
        }
    }
    ctxt->screen = SDL_SetVideoMode(WIDTH_CELL*NB_CELLS_X, HEIGHT_CELL*NB_CELLS_Y, 32, SDL_DOUBLEBUF|SDL_HWSURFACE);
    if(ctxt->screen == NULL)
    {
        fprintf(stderr, SDL_GetError());
        exit(-1);
    }
    for(i = 0; i < NB_STATES; ++i)
    {
        ctxt->colors[i] = SDL_CreateRGBSurface(SDL_HWSURFACE, WIDTH_CELL, HEIGHT_CELL, 32, 0, 0, 0, 0);
        SDL_FillRect(ctxt->colors[i], NULL, colors[i]);
    }
    for(i = 0; i < NB_CELLS_Y; ++i)
        for(j = 0; j < NB_CELLS_X; ++j)
#if defined (QUADLIFE)
            ctxt->map[i][j] = rand()%NB_STATES;
#else
            ctxt->map[i][j] = rand()%2;
#endif
}

void BlitAll(context_t *ctxt)
{
    int i, j;
    SDL_Rect dst;
    dst.h = HEIGHT_CELL;
    dst.w = WIDTH_CELL;
    for(i = 0; i < NB_CELLS_Y; ++i)
    {
        dst.y = i*HEIGHT_CELL;
        for(j = 0; j < NB_CELLS_X; ++j)
        {
            dst.x = j*WIDTH_CELL;
            SDL_BlitSurface(ctxt->colors[ctxt->map[i][j]], NULL, ctxt->screen, &dst);
        }
    }
}

int moveX[8] = {-1, +0, +1, -1, +1, -1, -0, +1},
    moveY[8] = {-1, -1, -1, -0, +0, +1, +1, +1};

result_t getNeighbours(context_t *ctxt, int y, int x)
{
    result_t res;
    int i;
    res.neighbours = 0;
    for(i = 0; i < NB_STATES; ++i)
        res.count[i] = 0;
    for(i = 0; i < 8; ++i)
    {
        if(y+moveY[i] >= 0 && y+moveY[i] < NB_CELLS_Y && x+moveX[i] >= 0 && x+moveX[i] < NB_CELLS_X
           && ctxt->map[y+moveY[i]][x+moveX[i]] != DEAD)
        {
            ++res.neighbours;
            ++res.count[ctxt->map[y+moveY[i]][x+moveX[i]]];
        }
    }
    return res;
}

state_e value(result_t res)
{
    int i;
    int ret = -1;
    int max = INT_MIN;
    for(i = 0; i < NB_STATES; ++i)
    {
        if(res.count[i] > max)
        {
            max = res.count[i];
            ret = i;
        }
    }
    if(max > 1)
        return ALIVE+i;
    for(i = 0; i < NB_STATES; ++i)
        if(res.count[i] == 0)
            return ALIVE+i;
    return ALIVE3;
}

void NextGen(context_t *ctxt)
{
    result_t result;
    int i, j;

    for(i = 0; i < NB_CELLS_Y; ++i)
    {
        for(j = 0; j < NB_CELLS_X; ++j)
        {
            result = getNeighbours(ctxt, i, j);
#if defined (DAY_N_NIGHT)
            if(ctxt->map[i][j] == DEAD)
            {
                if(result.neighbours == 3 || result.neighbours == 6 || result.neighbours == 7 || result.neighbours == 8)
                    ctxt->tmp[i][j] = ALIVE;
                else
                    ctxt->tmp[i][j] = DEAD;
            }
            else
            {
                if(result.neighbours == 3 ||result.neighbours == 4 || result.neighbours == 6 || result.neighbours == 7 ||result.neighbours == 8)
                    ctxt->tmp[i][j] = ALIVE;
                else
                    ctxt->tmp[i][j] = DEAD;
            }
#elif defined (LIFE_3_4)
            if(ctxt->map[i][j] == DEAD)
            {
                if(result.neighbours == 3 || result.neighbours == 4)
                    ctxt->tmp[i][j] = ALIVE;
                else
                    ctxt->tmp[i][j] = DEAD;
            }
            else
            {
                if(result.neighbours == 2 ||result.neighbours == 3)
                    ctxt->tmp[i][j] = ALIVE;
                else
                    ctxt->tmp[i][j] = DEAD;
            }
#elif defined (CONWAY)
            if(ctxt->map[i][j] == DEAD)
            {
                if(result.neighbours == 3)
                    ctxt->tmp[i][j] = ALIVE;
                else
                    ctxt->tmp[i][j] = DEAD;
            }
            else
            {
                if(result.neighbours == 2 || result.neighbours == 3)
                    ctxt->tmp[i][j] = ALIVE;
                else
                    ctxt->tmp[i][j] = DEAD;
            }
#elif defined (HIGHLIFE)
            if(ctxt->map[i][j] == DEAD)
            {
                if(result.neighbours == 6 || result.neighbours == 3)
                    ctxt->tmp[i][j] = ALIVE;
                else
                    ctxt->tmp[i][j] = DEAD;
            }
            else
            {
                if(result.neighbours == 2 || result.neighbours == 3)
                    ctxt->tmp[i][j] = ALIVE;
                else
                    ctxt->tmp[i][j] = DEAD;
            }
#elif defined (QUADLIFE)
            if(ctxt->map[i][j] == DEAD)
            {
                if(result.neighbours == 3)
                    ctxt->tmp[i][j] = value(result);
                else
                    ctxt->tmp[i][j] = DEAD;
            }
            else
            {
                if(result.neighbours == 2 ||result.neighbours == 3)
                    ctxt->tmp[i][j] = ctxt->map[i][j];
                else
                    ctxt->tmp[i][j] = DEAD;
            }
#endif
        }
    }
    for(i = 0; i < NB_CELLS_Y; ++i)
        for(j = 0; j < NB_CELLS_X; ++j)
            ctxt->map[i][j] = ctxt->tmp[i][j];
}


