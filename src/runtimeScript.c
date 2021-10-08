#include "game/world.h"
#include "graphics/texture.h"
#include "input/input.h"

#include "stdlib.h"
#include "time.h"

void initWorld()
{
    // World creation here

    int texture = loadTexture("test.png");
    int t2 = loadTexture("awesomeface.png");

    EntityID player = newEntity();
    world.player = player;

    TransformComponent t;
    t.position = (vec2s){0, 0};
    t.rotation = 0.785;
    t.scale = (vec2s){200, 200};

    SpriteComponent s;
    s.color = (vec3s){0.2, 0.4, 0.96};
    s.texIndex = t2;

    addComponent(player, sprite, &s);
    addComponent(player, transform, &t);

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
            s2.texIndex = texture;

            addComponent(e, sprite, &s2);
            addComponent(e, transform, &t2);
        }
    }
}

void onUpdateWorld(double ts)
{
    // World update here

    TransformComponent* t = getComponent(world.player, transform);
    t->rotation += 1 * ts;
    
    handleInput(world.player, ts);
}


void destroyWorld()
{
    // World destruction here
}