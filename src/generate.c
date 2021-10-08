#include "world/world.h"
#include "log/log.h"

#include "stdlib.h"
#include "time.h"

void init(World* w)
{
    LOG_TRACE("Creating World\n");

    EntityID player = newEntity();
    w->player = player;
    w->count = 1;

    TransformComponent t;
    t.position = (vec2s){0, 0};
    t.rotation = 0.785;
    t.scale = (vec2s){200, 200};

    SpriteComponent s;
    s.color = (vec3s){0.2, 0.4, 0.96};
    s.texIndex = 0;

    addComponent(player, sprite, (void*)&s);
    addComponent(player, transform, (void*)&t);

    srand(time(0));


    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            EntityID e = newEntity();
            TransformComponent t2;
            t2.position = (vec2s){i, j};
            t2.rotation = 0;
            t2.scale = (vec2s){200, 200};

            SpriteComponent s2;
            s2.color = (vec3s){ (float) rand() / RAND_MAX, (float) rand() / RAND_MAX, (float) rand() / RAND_MAX};
            s2.texIndex = 0;

            addComponent(e, sprite, (void*)&s2);
            addComponent(e, transform, (void*)&t2);
            w->count++;
        }
    }
}


void destroy(World* w)
{
  free(w);
}

WorldFuncPtr initWorld = &init;
WorldFuncPtr destroyWorld = &destroy;