#include "game/world.h"
#include "graphics/texture.h"
#include "input/input.h"

#include "log/log.h"

#include "stdlib.h"
#include "time.h"

void initWorld()
{
    // World creation here

    int texture = loadTexture("test.png");
    int texture2 = loadTexture("awesomeface.png");

    EntityID player = newEntity();
    world.player = player;

    TransformComponent* t = ECSaddComponent(player, Transform);
    t->position = (vec3s){0, 0, -1};
    t->rotation = 0.785;
    t->scale = (vec2s){200, 200};

    SpriteComponent* s = ECSaddComponent(player, Sprite);
    s->color = (vec3s){0.2, 0.4, 0.96};
    s->texIndex = texture2;

    PhysicsComponent* p = ECSaddComponent(player, Physics);
    p->bb = AABB;
    p->dynamic = 1;


    srand(time(0));

    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            EntityID e = newEntity();
            TransformComponent* t2 = ECSaddComponent(e, Transform);
            t2->position = (vec3s){i, j, -20};
            t2->rotation = 0;
            t2->scale = (vec2s){200, 200};

            SpriteComponent* s2 = ECSaddComponent(e, Sprite);
            s2->color = (vec3s){ (float) rand() / RAND_MAX, (float) rand() / RAND_MAX, (float) rand() / RAND_MAX};
            s2->texIndex = texture;

            PhysicsComponent* p1 = ECSaddComponent(e, Physics);
            p1->dynamic = 0;
        }
    }
}

void onUpdateWorld(double ts)
{
    // World update here

    TransformComponent* t = ECSgetComponent(world.player, Transform);
    t->rotation += 1 * ts;
    
    handleInput(world.player, ts);
}


void destroyWorld()
{
    // World destruction here
}