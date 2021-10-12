#include "game/world.h"
#include "physics/dyanmics/rigidBody.h"
#include "graphics/texture.h"
#include "input/input.h"

#include "log/log.h"

#include "stdlib.h"
#include "time.h"

Body* v = NULL;

void initWorld()
{
    // World creation here

    int texture = loadTexture("test.png");
    int texture2 = loadTexture("awesomeface.png");

    EntityID player = newEntity();
    world.player = player;

    TransformComponent* t = ECSaddComponent(player, Transform);
    t->position = (vec3s){-200, 0, -1};
    t->rotation = 0;
    t->scale = (vec2s){200, 200};

    SpriteComponent* s = ECSaddComponent(player, Sprite);
    s->color = (vec3s){0.2, 0.4, 0.96};
    s->texIndex = texture2;

    PhysicsComponent* p = ECSaddComponent(player, Physics);
    p->flags = ACTIVE | DYNAMIC;

    v = createBody(t->position, Dynamic, 0);
    addAABB(v, 100, 100);

    // srand(time(0));

    // for (int i = 0; i < 3; i++)
    // {
    //     for (int j = 0; j < 3; j++)
    //     {
    //         EntityID e = newEntity();
    //         TransformComponent* t2 = ECSaddComponent(e, Transform);
    //         t2->position = (vec3s){i, j, -20};
    //         t2->rotation = 0;
    //         t2->scale = (vec2s){200, 200};

    //         SpriteComponent* s2 = ECSaddComponent(e, Sprite);
    //         s2->color = (vec3s){ (float) rand() / RAND_MAX, (float) rand() / RAND_MAX, (float) rand() / RAND_MAX};
    //         s2->texIndex = texture;

    //         PhysicsComponent* p1 = ECSaddComponent(e, Physics);
    //         p1->flags = ACTIVE | STATIC;
    //     }
    // }

    EntityID floor = newEntity();
    TransformComponent* tf = ECSaddComponent(floor, Transform);
    tf->position = (vec3s){0, -400, -1};
    tf->rotation = 0;
    tf->scale = (vec2s){800, 50};

    SpriteComponent* sf = ECSaddComponent(floor, Sprite);
    sf->color = (vec3s){0, 0.4, 0.96};
    sf->texIndex = texture;

    PhysicsComponent* pf = ECSaddComponent(floor, Physics);
    pf->flags = ACTIVE | STATIC;


    Body* v1 = createBody(tf->position, Dynamic, 0);
    addAABB(v1, 400, 25);

    EntityID roof = newEntity();
    TransformComponent* tr = ECSaddComponent(roof, Transform);
    tr->position = (vec3s){0, 400, -1};
    tr->rotation = 0;
    tr->scale = (vec2s){800, 50};

    SpriteComponent* sr = ECSaddComponent(roof, Sprite);
    sr->color = (vec3s){0.2, 0.4, 0.96};
    sr->texIndex = texture;

    PhysicsComponent* pr = ECSaddComponent(roof, Physics);
    pr->flags = ACTIVE | STATIC;
}

void onUpdateWorld(double ts)
{
    // World update here

    TransformComponent* t = ECSgetComponent(world.player, Transform);
    t->rotation += 1 * ts;
    // if (isKeyPressed(KEY_SPACE))
    //     p->force = glms_vec2_add(p->force, (vec2s){0, 15});

    // if (isKeyPressed(KEY_W))
    //     p->force = glms_vec2_add(p->force, (vec2s){0, 1});
    // if (isKeyPressed(KEY_S))
    //     p->force = glms_vec2_add(p->force, (vec2s){0, -1});

    // if (isKeyPressed(KEY_D))
    //     p->force = glms_vec2_add(p->force, (vec2s){1, 0});
    // if (isKeyPressed(KEY_A))
    //     p->force = glms_vec2_add(p->force, (vec2s){-1, 0});

    // if (isKeyPressed(KEY_P))
    //     p->speed = (vec2s){0, 0};

    TransformComponent* p = ECSgetComponent(world.player, Transform);

    if (isKeyPressed(KEY_W))
        p->position.y += 100 * ts;
    if (isKeyPressed(KEY_S))
        p->position.y -= 100 * ts;

    if (isKeyPressed(KEY_D))
        p->position.x += 100 * ts;
    if (isKeyPressed(KEY_A))
        p->position.x -= 100 * ts;

    v->position = p->position;
    update(ts);
    
   // handleInput(world.player, ts);
}


void destroyWorld()
{
    // World destruction here
}