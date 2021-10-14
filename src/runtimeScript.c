#include "game/world.h"
#include "physics/dyanmics/rigidBody.h"
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
    t->position = (vec3s){-200, 0, -1};
    t->rotation = 0;
    t->scale = (vec2s){200, 200};

    SpriteComponent* s = ECSaddComponent(player, Sprite);
    s->color = (vec3s){0.2, 0.4, 0.96};
    s->texIndex = texture2;

    Body* v = createBody(t->position, Dynamic, 0);
    addAABB(v, 100, 100);

    PhysicsComponent* p = ECSaddComponent(player, Physics);
    p->physicsBody = v;

    // srand(time(0));

    // for (int i = 0; i < 3; i++)
    // {
    //     for (int j = 0; j < 3; j++)
    //     {
    //         EntityID e = newEntity();
    //         TransformComponent* t2 = ECSaddComponent(e, Transform);
    //         t2->position = (vec3s){i * 200, j * 200, -20};
    //         t2->rotation = 0;
    //         t2->scale = (vec2s){200, 200};

    //         SpriteComponent* s2 = ECSaddComponent(e, Sprite);
    //         s2->color = (vec3s){ (float) rand() / RAND_MAX, (float) rand() / RAND_MAX, (float) rand() / RAND_MAX};
    //         s2->texIndex = texture;
    //     }
    // }

    EntityID floor = newEntity();
    TransformComponent* tf = ECSaddComponent(floor, Transform);
    tf->position = (vec3s){200, -200, -1};
    tf->rotation = 0;
    tf->scale = (vec2s){200, 200};

    SpriteComponent* sf = ECSaddComponent(floor, Sprite);
    sf->color = (vec3s){0, 0.4, 0.96};
    sf->texIndex = texture;

    Body* v1 = createBody(tf->position, Static, 0);
    addAABB(v1, 100, 100);

    PhysicsComponent* p1 = ECSaddComponent(floor, Physics);
    p1->physicsBody = v1;

    EntityID roof = newEntity();
    TransformComponent* tr = ECSaddComponent(roof, Transform);
    tr->position = (vec3s){-200, 200, -1};
    tr->rotation = 0;
    tr->scale = (vec2s){200, 200};

    SpriteComponent* sr = ECSaddComponent(roof, Sprite);
    sr->color = (vec3s){0.2, 0.4, 0.96};
    sr->texIndex = texture;

    Body* v2 = createBody(tr->position, Static, 0);
    addAABB(v2, 100, 100);

    PhysicsComponent* p2 = ECSaddComponent(roof, Physics);
    p2->physicsBody = v2;

    EntityID roof2 = newEntity();
    TransformComponent* tr2 = ECSaddComponent(roof2, Transform);
    tr2->position = (vec3s){300, 250, -1};
    tr2->rotation = 0;
    tr2->scale = (vec2s){200, 200};

    SpriteComponent* sr2 = ECSaddComponent(roof2, Sprite);
    sr2->color = (vec3s){0.2, 0.4, 0.96};
    sr2->texIndex = texture;

    Body* v22 = createBody(tr2->position, Static, 0);
    addAABB(v22, 100, 100);

    PhysicsComponent* p22 = ECSaddComponent(roof2, Physics);
    p22->physicsBody = v22;
}

void onUpdateWorld(double ts)
{
    // World update here

    TransformComponent* p = ECSgetComponent(world.player, Transform);

    if (isKeyPressed(KEY_W))
        p->position.y += 100 * ts;
    if (isKeyPressed(KEY_S))
        p->position.y -= 100 * ts;

    if (isKeyPressed(KEY_D))
        p->position.x += 100 * ts;
    if (isKeyPressed(KEY_A))
        p->position.x -= 100 * ts;

    PhysicsComponent* ph = ECSgetComponent(world.player, Physics);

    Body* b = ph->physicsBody;
    b->position = p->position;
    update(ts);
    
   // handleInput(world.player, ts);
}


void destroyWorld()
{
    // World destruction here
}