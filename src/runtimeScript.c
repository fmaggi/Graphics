#include "game/world.h"
#include "engine.h"

typedef struct world
{
    EntityID player;
} World;

World world;

void initWorld()
{
    // World creation here
    EntityID player = newEntity();
    world.player = player;

    TransformComponent* t = ECSaddComponent(player, TransformComponent);
    t->position = (vec3s){{-300, 0, -1}};
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
    tf->position = (vec3s){{0, -200, -1}};
    tf->rotation = 0;
    tf->scale = (vec2s){{800, 100}};

    int texture = loadTexture("test.png");
    SpriteComponent* sf = ECSaddComponent(floor, SpriteComponent);
    sf->color = (vec3s){{0.3, 0.45, 0.96}};
    sf->texIndex = texture;

    Body* v1 = createBody(tf->position, Static, 0);
    addAABB(v1, 400, 50);

    PhysicsComponent* p1 = ECSaddComponent(floor, PhysicsComponent);
    p1->physicsBody = v1;

    EntityID roof = newEntity();
    TransformComponent* tr = ECSaddComponent(roof, TransformComponent);
    tr->position = (vec3s){{-200, 300, -1}};
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
        t->position = body->position;
    }
    closeView(r);

    PhysicsComponent* p = ECSgetComponent(world.player, PhysicsComponent);
    Body* b = p->physicsBody;
    if (isKeyPressed(KEY_W))
        b->impulse.y += 30 / ts;
    if (isKeyPressed(KEY_S))
        b->impulse.y -= 30 / ts;

    if (isKeyPressed(KEY_D))
        b->impulse.x += 30 / ts;
    if (isKeyPressed(KEY_A))
        b->impulse.x -= 30 / ts;

    update(ts);
}

void onRenderWorld()
{
    struct registryView r = ECSgroupView(SpriteComponent, TransformComponent); 

    for (int i = 0; i < r.count; i++)
    {   
        EntityID id = r.view[i];
        TransformComponent* t = ECSgetComponent(id, TransformComponent);
        SpriteComponent* s = ECSgetComponent(id, SpriteComponent);
          
        mat4s m = getTransform(t->position, t->rotation, t->scale);

        rendererSubmit(m, s->color, s->texIndex);
    }

    closeView(r);
}

int onEventWorld(EventHolder* event)
{
    return 0;
}

void destroyWorld()
{
    // World destruction here
}