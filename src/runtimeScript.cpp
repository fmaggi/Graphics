#include "game/world.h"
#include "engine.h"

typedef struct world
{
    EntityID player;
    EntityID floor[3];
    float speed;
} World;

World world;

#define FLOOR 1
#define ON_FLOOR 1
void playerCollided(Body* self, Body* other)
{
    if (other->userFlags & FLOOR)
    {
        self->userFlags |= ON_FLOOR;
    }
    if (self->position.y < -80)
    {
        world.speed = 0;
    }
}

void initWorld()
{
    // World creation here
    initPhysics(-700);
    orthoCamera({0, 0, 0}, 1200, 800);

    world.speed = 3;

    EntityID player = newEntity();
    world.player = player;

    TransformComponent* t = (TransformComponent*)ECSaddComponent(player, TransformComponent);
    t->position = {0, 0, 0};
    t->rotation = 0;
    t->scale = {200, 200};

    SpriteComponent* s = (SpriteComponent*)ECSaddComponent(player, SpriteComponent);
    s->color = {0.92, 0.45, 0.35};
    s->texIndex = NO_TEXTURE;

    Body* v = createBody(t->position, Dynamic, playerCollided, &world.player, 0);
    addAABB(v, 100, 100);
    v->speed.x = 180;

    PhysicsComponent* p = (PhysicsComponent*)ECSaddComponent(player, PhysicsComponent);
    p->physicsBody = v;

    EntityID floor = newEntity();
    world.floor[0] = floor;
    TransformComponent* tf = (TransformComponent*)ECSaddComponent(floor, TransformComponent);
    tf->position = {0, -200, 0};
    tf->rotation = 0;
    tf->scale = {900, 100};

    int texture = loadTexture("test.png");
    SpriteComponent* sf =(SpriteComponent*) ECSaddComponent(floor, SpriteComponent);
    sf->color = {0.3, 0.45, 0.96};
    sf->texIndex = texture;

    Body* v1 = createBody(tf->position, Static, 0, 0, FLOOR);
    addAABB(v1, 450, 50);

    PhysicsComponent* p1 = (PhysicsComponent*)ECSaddComponent(floor, PhysicsComponent);
    p1->physicsBody = (Body*)v1;

    EntityID floor2 = newEntity();
    world.floor[1] = floor2;
    TransformComponent* tf2 = (TransformComponent*)ECSaddComponent(floor2, TransformComponent);
    tf2->position = {1200, -200, 0};
    tf2->rotation = 0;
    tf2->scale = {900, 100};

    SpriteComponent* sf2 =(SpriteComponent*) ECSaddComponent(floor2, SpriteComponent);
    sf2->color = {0.3, 0.8, 0.1};
    sf2->texIndex = texture;

    Body* v12 = createBody(tf2->position, Static, 0, 0, FLOOR);
    addAABB(v12, 450, 50);

    PhysicsComponent* p12 = (PhysicsComponent*)ECSaddComponent(floor2, PhysicsComponent);
    p12->physicsBody = (Body*)v12;

    EntityID floor3 = newEntity();
    world.floor[2] = floor3;
    TransformComponent* tf3 = (TransformComponent*)ECSaddComponent(floor3, TransformComponent);
    tf3->position = {-1200, -200, 0};
    tf3->rotation = 0;
    tf3->scale = {900, 100};

    SpriteComponent* sf3 = (SpriteComponent*)ECSaddComponent(floor3, SpriteComponent);
    sf3->color = {0.7, 0.8, 0.1};
    sf3->texIndex = texture;

    Body* v13 = createBody(tf3->position, Static, 0, 0, FLOOR);
    addAABB(v13, 450, 50);

    PhysicsComponent* p13 = (PhysicsComponent*)ECSaddComponent(floor3, PhysicsComponent);
    p13->physicsBody = (Body*)v13;
}

void onUpdateWorld(double ts)
{
    // World update here
    stepPhysics(ts);
    static int next = 2;
    if ((int)camera.pos.x % 600 == 0 && (int)camera.pos.x % 1200 != 0)
    {
        PhysicsComponent* p = (PhysicsComponent*)ECSgetComponent(world.floor[next], PhysicsComponent);
        Body* b = (Body*)p->physicsBody;
        b->position.x += 1200 * 3;
        next++;
        if (next == 3)
            next = 0;
    }

    struct View view = ECSview(PhysicsComponent);

    for (uint32_t i = 0; i < view.count; i++)
    {
        PhysicsComponent* p1 = (PhysicsComponent*)ECSviewGetComponent(&view, i);
        TransformComponent* t = (TransformComponent*)ECSgetComponent(p1->id, TransformComponent);
        Body* body = (Body*)p1->physicsBody;
        t->position = body->position;
    }

    moveCamera(world.speed, 0);
}

void onRenderWorld()
{
    struct View v = ECSview(SpriteComponent);
    for (uint32_t i = 0; i < v.count; i++)
    {
        SpriteComponent* s = (SpriteComponent*)ECSviewGetComponent(&v, i);
        TransformComponent* t = (TransformComponent*)ECSgetComponent(s->id, TransformComponent);
        Renderer::PushQuad(t->position, t->rotation, t->scale, s->color, s->texIndex);
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
            PhysicsComponent* p = (PhysicsComponent*)ECSgetComponent(world.player, PhysicsComponent);
            Body* b = (Body*)p->physicsBody;
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
