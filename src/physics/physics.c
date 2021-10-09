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
            unsigned int div = transforms[i].position.x / widthPerDivision;
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

    float left, right;

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

    float top, bottom;
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
    LOG_INFO("Collision %i %i\n", e1, e2);
}

void narrowPhase(TransformComponent* transforms, PhysicsComponent* physics, int count)
{
    int heightPerDivision = screenHeight/ DIVISIONS;
    for (int div = 0; div < DIVISIONS; div++)
    {
        for (int i = 0; i < currentId[div]; i++)
        {   
            for (int j = i+1; j < currentId[div]; j++)
            {
                EntityID e1 = divisions_screen[div][i];
                EntityID e2 = divisions_screen[div][j];
                if (!physics[e1].dynamic && !physics[e2].dynamic)
                    continue;
                checkCollision(e1, e2);
            }
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