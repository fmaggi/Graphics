#include "world/world.h"
#include "log/log.h"

void init(World* w)
{
    LOG_TRACE("Creating World\n");

    initECS();

    EntityID e = newEntity();
    LOG_INFO("id: %i\n", e);

    TransformComponent t;
    t.position = (vec3s){0, 1, 2};
    t.rotation = (vec3s){20, 4, 5};
    t.scale = (vec3s){2, 3, 7};

    SpriteComponent s;
    s.color = (vec3s){0.2, 0.4, 0.96};
    s.texIndex = 0;

    addComponent(e, sprite, (void*)&s);
    addComponent(e, transform, (void*)&t);
}


void destroy(World* w)
{
    LOG_INFO("%i\n", hasComponent(0, transform));

    TransformComponent* ptr = (TransformComponent*) getComponent(0, transform);
    LOG_INFO("%f %f %f\n", ptr->position.x, ptr->position.y, ptr->position.z);
    SpriteComponent* p = (SpriteComponent*) getComponent(0, sprite);
    LOG_INFO("%f\n", p->texIndex);

    TransformComponent* transforms = registerView(transform);
    LOG_INFO("%f %f %f\n", transforms[0].position.x, transforms[0].position.y, transforms[0].position.z);
}

WorldFuncPtr initWorld = &init;
WorldFuncPtr destroyWorld = &destroy;