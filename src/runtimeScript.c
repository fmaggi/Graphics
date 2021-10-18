#include "game/world.h"
#include "engine.h"

World world;

void initWorld()
{
    // World creation here


    EntityID player = newEntity();
    world.player = player;

    TransformComponent* t = ECSaddComponent(player, TransformComponent);
    t->position = (vec3s){{-200, -200, -1}};
    t->rotation = 0;
    t->scale = (vec2s){{200, 200}};

    SpriteComponent* s = ECSaddComponent(player, SpriteComponent);
    s->color = (vec3s){{0.92, 0.45, 0.35}};
    s->texIndex = NO_TEXTURE;

    Body* v = createBody(t->position, Dynamic, 0);
    addAABB(v, 100, 100);

    PhysicsComponent* p = ECSaddComponent(player, PhysicsComponent);
    p->physicsBody = v;

    EntityID floor = newEntity();
    TransformComponent* tf = ECSaddComponent(floor, TransformComponent);
    tf->position = (vec3s){{200, -200, -1}};
    tf->rotation = 0;
    tf->scale = (vec2s){{200, 200}};

    int texture = loadTexture("test.png");
    SpriteComponent* sf = ECSaddComponent(floor, SpriteComponent);
    sf->color = (vec3s){{0.3, 0.45, 0.96}};
    sf->texIndex = texture;

    Body* v1 = createBody(tf->position, Static, 0);
    addAABB(v1, 100, 100);

    PhysicsComponent* p1 = ECSaddComponent(floor, PhysicsComponent);
    p1->physicsBody = v1;

    EntityID roof = newEntity();
    TransformComponent* tr = ECSaddComponent(roof, TransformComponent);
    tr->position = (vec3s){{-200, 200, -1}};
    tr->rotation = 0;
    tr->scale = (vec2s){{200, 200}};

    SpriteComponent* sr = ECSaddComponent(roof, SpriteComponent);
    sr->color = (vec3s){{0.92, 0.75, 0.4}};
    sr->texIndex = texture;

    Body* v2 = createBody(tr->position, Static, 0);
    addAABB(v2, 100, 100);

    PhysicsComponent* p2 = ECSaddComponent(roof, PhysicsComponent);
    p2->physicsBody = v2;

    EntityID roof2 = newEntity();
    TransformComponent* tr2 = ECSaddComponent(roof2, TransformComponent);
    tr2->position = (vec3s){{300, 250, -1}};
    tr2->rotation = 0;
    tr2->scale = (vec2s){{200, 200}};

    SpriteComponent* sr2 = ECSaddComponent(roof2, SpriteComponent);
    sr2->color = (vec3s){{0.2, 0.92, 0.7}};
    sr2->texIndex = texture;

    Body* v22 = createBody(tr2->position, Dynamic, 0);
    addAABB(v22, 100, 100);

    PhysicsComponent* p22 = ECSaddComponent(roof2, PhysicsComponent);
    p22->physicsBody = v22;
}

void onUpdateWorld(double ts)
{
    // World update here

    struct registryView r = ECSgroupView(PhysicsComponent, TransformComponent);
    for (int i = 0; i < r.count; i++)
    {
        EntityID id = r.view[i];
        TransformComponent* t = ECSgetComponent(id, TransformComponent);
        PhysicsComponent* p1 = ECSgetComponent(id, PhysicsComponent);
        Body* body = p1->physicsBody;
        if (body->type == Dynamic) // probably dont need to this as non dynamic bodies dont get updated but is clearer this way
            t->position = body->position;
    }
    closeView(r);

    TransformComponent* p = ECSgetComponent(world.player, TransformComponent);
    if (isKeyPressed(KEY_W))
        p->position.y += 100 * ts;
    if (isKeyPressed(KEY_S))
        p->position.y -= 100 * ts;

    if (isKeyPressed(KEY_D))
        p->position.x += 100 * ts;
    if (isKeyPressed(KEY_A))
        p->position.x -= 100 * ts;

    PhysicsComponent* ph = ECSgetComponent(world.player, PhysicsComponent);
    Body* b = ph->physicsBody;
    b->position = p->position;
    update(ts);
}

bool onEventWorld(EventHolder* event)
{
    return false;
}

void destroyWorld()
{
    // World destruction here
}