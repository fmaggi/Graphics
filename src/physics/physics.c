#include "physics.h"

#include "cglm/struct.h"

#include "entity/entity.h"

#include "log/log.h"

static int screenWidth;
static int screenHeight;

#define DIVISIONS 4

static EntityID* divisions_screen[DIVISIONS];
int currentId[DIVISIONS];

void initPhysics(int width, int height)
{
    screenWidth = width;
    screenHeight = height;
}

void broadPhase(TransformComponent* transforms, unsigned int count)
{
    int widthPerDivision = screenWidth / DIVISIONS;

    for (int i = 0; i < count; i++)
    {
        if (hasComponent(i, Physics))
        {
            int div = transforms[i].position.x / widthPerDivision;
            if (div >= DIVISIONS)
                continue;
            int index = currentId[div];
            divisions_screen[div][index] = i;
            currentId[div]++;
        }
    }
}

void checkCollision(EntityID e1, EntityID e2)
{
    TransformComponent* t1 = ECSgetComponent(e1, Transform);
    TransformComponent* t2 = ECSgetComponent(e2, Transform);

    int left, right;

    if (t1->position.x >= t2->position.x)
    {
        right = t1->position.x * t1->scale.x - 0.5 * t1->scale.x;
        left =  t2->position.x * t2->scale.x + 0.5 * t2->scale.x;
    }
    else
    {
        right = t2->position.x * t2->scale.x - 0.5 * t2->scale.x;
        left = t1->position.x * t2->scale.x + 0.5 * t1->scale.x;
    }

    if (left < right)
    {
        LOG_INFO("No collision\n");
        return;
    }

    int top, bottom;
    if (t1->position.y >= t2->position.y)
    {
        top = t1->position.y * t1->scale.y - 0.5 * t1->scale.y;
        bottom =  t2->position.y * t2->scale.y + 0.5 * t2->scale.y;
    }
    else
    {
        top = t2->position.y * t2->scale.y - 0.5 * t2->scale.y;
        bottom = t1->position.y * t2->scale.y + 0.5 * t1->scale.y;
    }

    if (bottom < top)
    {
        LOG_INFO("No collision\n");
        return;
    }
    LOG_INFO("Collision\n");
}

void narrowPhase(TransformComponent* transforms, PhysicsComponent* physics, int count)
{
    int heightPerDivision = screenHeight/ DIVISIONS;
    for (int div = 0; div < DIVISIONS; div++)
    {
        int girdElements[DIVISIONS] = {-1};
        for (int i = 0; i < currentId[div]; i++)
        {   
            EntityID e = divisions_screen[div][i];
            int ygrid = transforms[e].position.y / heightPerDivision;
            if (girdElements[ygrid] != -1)
            {
                checkCollision(e, girdElements[ygrid]);
            }
            girdElements[ygrid] = e;
        }
    }
}

void updatePhysics()
{
    unsigned int count = getEntityCount();

    for (int i = 0; i < DIVISIONS; i++)
    {
        divisions_screen[i] = malloc(sizeof(EntityID) * count);
        currentId[i] = 0;
    }    

    PhysicsComponent* physics = registerView(Physics);
    TransformComponent* transforms = registerView(Transform); 

    broadPhase(transforms, count);
    narrowPhase(transforms, physics, count);

    for (int i = 0; i < DIVISIONS; i++)
    {
        free(divisions_screen[i]);
    }    
}