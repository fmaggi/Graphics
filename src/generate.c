#include "world/world.h"
#include "log/log.h"

#include <stdlib.h>
#include "time.h"

void init(World* w)
{
    LOG_TRACE("Creating World\n");

    Entity player;
    player.pos = (vec3s){0, 0, 0};
    player.color = (vec3s){0.2, 0.3, 0.9};
    player.speed = (vec2s){1, 1};

    Entity player2;
    player2.pos = (vec3s){0, 1, 0};
    player2.color = (vec3s){0.9, 0.8, 0.3};

    w->player = player;
    //w->player2 = player2;

    srand(time(0));

    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            Entity e;
            e.pos = (vec3s){i, j, 0};
            e.color = (vec3s){(float)rand()/(float)RAND_MAX, (float)rand()/(float)RAND_MAX, (float)rand()/(float)RAND_MAX};
            w->entities[3 * i + j] = e;
        }
    }
    w->index = 9;
}


void destroy(World* w)
{
    free(w);
}

WorldFuncPtr initWorld = &init;
WorldFuncPtr destroyWorld = &destroy;