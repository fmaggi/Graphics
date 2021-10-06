#include "world/world.h"
#include "log/log.h"

void init(World* w)
{
    LOG_TRACE("Creating World\n");

    Entity player;
    player.pos = (vec3s){0, 0, 0};
    player.color = (vec3s){0.2, 0.3, 0.9};

    Entity player2;
    player2.pos = (vec3s){0, 1, 0};
    player2.color = (vec3s){0.9, 0.8, 0.3};

    w->player = player;
    w->player2 = player2;

    for (int i = 0; i < 3; i++)
    {
        Entity e;
        e.pos = (vec3s){i, 2, 0};
        e.color = (vec3s){0.9, 0.3, 0.4};
        w->entities[i] = e;
    }
    w->index = 3;
}

WorldFuncPtr initWorld = &init;

void destroy(World* w)
{
    free(w);
}

WorldFuncPtr destroyWorld = &destroy;