#include "game/world.h"
#include "engine.h"

typedef struct world
{
    EntityID player;
} World;

World world;

void playerCollided(Body* self, Body* other)
{
    if (other->type == Dynamic)
    {
        EntityID id = *(EntityID*) self->userData;
        SpriteComponent* s = ECSgetComponent(id, SpriteComponent);
        s->color = (vec3s){{0.2, 0.5, 0.87}};
    }
}

void initWorld()
{
    // World creation here
    initPhysics(-700);
    orthoCamera((vec3s){{0, 0, 0}}, 1200, 800);

    EntityID player = newEntity();
    world.player = player;

    TransformComponent* t = ECSaddComponent(player, TransformComponent);
    t->position = (vec3s){{-300, 0, -1}};
    t->rotation = 0;
    t->scale = (vec2s){{200, 200}};

    SpriteComponent* s = ECSaddComponent(player, SpriteComponent);
    s->color = (vec3s){{0.92, 0.45, 0.35}};
    s->texIndex = NO_TEXTURE;

    Body* v = createBody(t->position, Dynamic, playerCollided, &world.player, 0);
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

    Body* v1 = createBody(tf->position, Static, 0, 0, 0);
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

    Body* v2 = createBody(tr->position, Static, 0, 0, 0);
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

    Body* v22 = createBody(tr2->position, Dynamic, 0, 0, 0);
    addAABB(v22, 100, 100);

    PhysicsComponent* p22 = ECSaddComponent(roof2, PhysicsComponent);
    p22->physicsBody = v22;
}

void onUpdateWorld(double ts)
{
    // World update here
    stepPhysics(ts);

    struct View view = ECSview(PhysicsComponent);

    for (int i = 0; i < view.count; i++)
    {
        PhysicsComponent* p1 = ECSviewGetComponent(&view, i);
        if (!ECShasComponent(p1->id,TransformComponent))
            continue;
        TransformComponent* t = ECSgetComponent(p1->id, TransformComponent);
        Body* body = p1->physicsBody;
        t->position = body->position;
    }

    // SpriteComponent* s = ECSgetComponent(world.player, SpriteComponent);
    // int factor = -1;
    // if (s->color.z < 0)
    //     factor = 1;
    // else if (s->color.z > 1)
    //     factor = -1;

    // s->color = glms_vec3_rotate(s->color, 0.08*factor, (vec3s){{1, 1, 1}});

    PhysicsComponent* p = ECSgetComponent(world.player, PhysicsComponent);
    Body* b = p->physicsBody;
    if (isKeyPressed(KEY_W))
        // moveCamera(0, 30);
        b->impulse.y += 1800;
    if (isKeyPressed(KEY_S))
        // moveCamera(0, -30);
        b->impulse.y -= 1800;

    if (isKeyPressed(KEY_D))
        // moveCamera(30, 0);
        b->impulse.x += 1800;
    if (isKeyPressed(KEY_A))
        // moveCamera(-30, 0);
        b->impulse.x -= 1800;
}

void onRenderWorld()
{
    struct View v = ECSview(SpriteComponent);

    for (int i = 0; i < v.count; i++)
    {
        SpriteComponent* s = ECSviewGetComponent(&v, i);
        if (!ECShasComponent(s->id,TransformComponent))
            continue;
        TransformComponent* t = ECSgetComponent(s->id, TransformComponent);

        pushQuad(t->position, t->rotation, t->scale, s->color, s->texIndex);
    }
}

int onEventWorld(void* event, enum EventType type)
{
    if (type == KeyPressed)
    {
        KeyEvent e = *(KeyEvent*) event;
        if (e.key == KEY_C && e.mods == MOD_CONTROL)
            dispatchEvent(0, WindowClose);
    }
    else if (type == MouseMoved)
        return 0;
    return 0;
}

void destroyWorld()
{
    // World destruction here
}
