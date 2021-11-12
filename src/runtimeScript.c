#include "game/world.h"
#include "engine.h"

typedef struct world
{
    EntityID player;
    EntityID floor[2];
    float speed;
} World;

World world;

#define FLOOR 1
#define ON_FLOOR 1
#define OTHER 2
void playerCollided(Body* self, Body* other)
{
    if (other->userFlags & FLOOR)
    {
        self->userFlags |= ON_FLOOR;
    }
    else if(other->userFlags & OTHER)
    {
        world.speed = 0;
    }
}

void initWorld()
{
    // World creation here
    initPhysics(-700);
    orthoCamera((vec3s){{0, 0, 0}}, 1200, 800);

    world.speed = 3;

    EntityID player = newEntity();
    world.player = player;

    TransformComponent* t = ECSaddComponent(player, TransformComponent);
    t->position = (vec3s){{0, 0, -1}};
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
    world.floor[0] = floor;
    TransformComponent* tf = ECSaddComponent(floor, TransformComponent);
    tf->position = (vec3s){{0, -200, -1}};
    tf->rotation = 0;
    tf->scale = (vec2s){{1200, 100}};

    int texture = loadTexture("test.png");
    SpriteComponent* sf = ECSaddComponent(floor, SpriteComponent);
    sf->color = (vec3s){{0.3, 0.45, 0.96}};
    sf->texIndex = texture;

    Body* v1 = createBody(tf->position, Static, 0, 0, FLOOR);
    addAABB(v1, 600, 50);

    PhysicsComponent* p1 = ECSaddComponent(floor, PhysicsComponent);
    p1->physicsBody = v1;

    EntityID floor2 = newEntity();
    world.floor[1] = floor2;
    TransformComponent* tf2 = ECSaddComponent(floor2, TransformComponent);
    tf2->position = (vec3s){{1200, -200, -1}};
    tf2->rotation = 0;
    tf2->scale = (vec2s){{1200, 100}};

    SpriteComponent* sf2 = ECSaddComponent(floor2, SpriteComponent);
    sf2->color = (vec3s){{0.3, 0.45, 0.96}};
    sf2->texIndex = texture;

    Body* v12 = createBody(tf2->position, Static, 0, 0, FLOOR);
    addAABB(v12, 600, 50);

    PhysicsComponent* p12 = ECSaddComponent(floor2, PhysicsComponent);
    p12->physicsBody = v12;

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
    tr2->position = (vec3s){{300, 0, -1}};
    tr2->rotation = 0;
    tr2->scale = (vec2s){{200, 200}};

    SpriteComponent* sr2 = ECSaddComponent(roof2, SpriteComponent);
    sr2->color = (vec3s){{0.2, 0.92, 0.7}};
    sr2->texIndex = texture;

    // Body* v22 = createBody(tr2->position, Static, 0, 0, OTHER);
    // addAABB(v22, 100, 100);

    // PhysicsComponent* p22 = ECSaddComponent(roof2, PhysicsComponent);
    // p22->physicsBody = v22;
}

void onUpdateWorld(double ts)
{
    // World update here
    stepPhysics(ts);
    static int next = 1;
    if ((int)camera.pos.x % 600 == 0 && (int)camera.pos.x % 1200 != 0)
    {
        next = !next;
        PhysicsComponent* p = ECSgetComponent(world.floor[next], PhysicsComponent);
        Body* b = p->physicsBody;
        b->position.x += 2400;
    }

    struct View view = ECSview(PhysicsComponent);

    for (int i = 0; i < view.count; i++)
    {
        PhysicsComponent* p1 = ECSviewGetComponent(&view, i);
        TransformComponent* t = ECSgetComponent(p1->id, TransformComponent);
        Body* body = p1->physicsBody;
        t->position = body->position;
    }

    PhysicsComponent* p = ECSgetComponent(world.player, PhysicsComponent);
    Body* b = p->physicsBody;
    b->position.x += world.speed;
    moveCamera(world.speed, 0);
}

void onRenderWorld()
{
    struct View v = ECSview(SpriteComponent);

    for (int i = 0; i < v.count; i++)
    {
        SpriteComponent* s = ECSviewGetComponent(&v, i);
        TransformComponent* t = ECSgetComponent(s->id, TransformComponent);

        pushQuad(t->position, t->rotation, t->scale, s->color, s->texIndex);
    }
}

int onEventWorld(struct Event e)
{
    if (e.type == KeyPressed)
    {
        if (e.key.key == KEY_C && e.key.mods == MOD_CONTROL)
            dispatchEvent((struct Event){ .type = WindowClose });
        if (e.key.key == KEY_SPACE)
        {
            PhysicsComponent* p = ECSgetComponent(world.player, PhysicsComponent);
            Body* b = p->physicsBody;
            if (b->userFlags & ON_FLOOR)
            {
                b->impulse.y += 30000;
                b->userFlags &= ~ON_FLOOR;
            }
        }
    }
    return 0;
}

void destroyWorld()
{
    // World destruction here
}
