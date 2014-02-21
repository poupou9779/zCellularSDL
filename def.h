#ifndef DEF_H
#define DEF_H

#include <SDL/SDL.h>

#define NB_CELLS_Y  750
#define NB_CELLS_X  1200
#define WIDTH_CELL  1
#define HEIGHT_CELL 1

#define NB_STATES 3
#define ELAPSE 25

#define DAY_N_NIGHT

#if defined (DAY_N_NIGHT)
# define NAME "Day and night"
#elif defined (CONWAY)
# define NAME "Conway's game of life"
#elif defined (LIFE_3_4)
# define NAME "Life 3-4"
#elif defined (HIGHLIFE)
# define NAME "High Life"
#elif defined (QUADLIFE)
# define NAME "Quadlife"
#endif

#define ALIVE ALIVE1

typedef enum {DEAD = 0, ALIVE1, ALIVE2, ALIVE3, ALIVE4} state_e;

typedef struct
{
    int neighbours;
    int count[NB_STATES];
}
result_t;

typedef struct
{
    SDL_Surface *screen,
                *colors[NB_STATES];
    state_e **map,
            **tmp;
}
context_t;

#endif
