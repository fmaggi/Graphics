#include "collision.h"

#include "cglm/struct.h"
#include "log/log.h"

#include "math.h"

#define DIVISIONS 4

struct AABB
{
    vec2s min, max;
    vec3s center;
    EntityID e;
};

struct AABB createAABB(EntityID e)
{
    struct AABB aabb;
    TransformComponent* t = ECSgetComponent(e, Transform);
    aabb.min.x = (t->position.x - 0.5) * t->scale.x;
    aabb.min.y = (t->position.y - 0.5) * t->scale.y; 

    aabb.max.x = (t->position.x + 0.5) * t->scale.x;
    aabb.max.y = (t->position.y + 0.5) * t->scale.y;
    aabb.e = e; 

    return aabb;
}

int checkCollisionAABB(struct AABB a, struct AABB b)
{
    float leftAABB, rightAABB;
    if (a.min.x < b.min.x)
    {
        rightAABB= b.min.x;
        leftAABB =  a.max.x;
    }
    else
    {
        rightAABB = a.min.x;
        leftAABB =  b.max.x;
    }  

    float topAABB, bottomAABB;
    if (a.min.y < b.min.y)
    {
        topAABB = b.min.y;
        bottomAABB = a.max.y;
    }
    else
    {
        topAABB = a.min.y;
        bottomAABB = b.max.y;
    }

    return leftAABB > rightAABB && bottomAABB > topAABB;
}

vec2s getDepth(TransformComponent* ta, TransformComponent* tb)
{

    float minX = (ta->scale.x + tb->scale.x) / 2;
    float minY = (ta->scale.y + tb->scale.y) / 2;

    float dx = ta->position.x * ta->scale.x - tb->position.x * tb->scale.x;
    float dy = ta->position.y * ta->scale.y - tb->position.y * tb->scale.y;

    dx = fabs(dx);
    dy = fabs(dy);

    float depthx = minX-dx;
    float depthy = minY-dy;

    return (vec2s){depthx, depthy};
}

void handleCollision(struct AABB a, struct AABB b)
{   
    PhysicsComponent* pa = ECSgetComponent(a.e, Physics);
    PhysicsComponent* pb = ECSgetComponent(b.e, Physics);

    TransformComponent* ta = ECSgetComponent(a.e, Transform);
    TransformComponent* tb = ECSgetComponent(b.e, Transform);

    vec2s depth = getDepth(ta, tb);
    if (depth.x < depth.y)
    {
        if (pa->flags & DYNAMIC)
        {
            pa->speed = (vec2s){-pa->speed.x, pa->speed.y};
            if (ta->position.x < tb->position.x)
                ta->position.x -= depth.x / ta->scale.x;
            else
                ta->position.x += depth.x / ta->scale.x;
        }
        if (pb->flags & DYNAMIC)
        {
            pb->speed = (vec2s){-pb->speed.x, pb->speed.y};
            if (tb->position.x < ta->position.x)
                tb->position.x -= depth.x / tb->scale.x;
            else
                tb->position.x += depth.x / tb->scale.x;
        }
    }
    else
    {
       if (pa->flags & DYNAMIC)
        {
            pa->speed = (vec2s){pa->speed.x, -pa->speed.y};
            if (ta->position.y < tb->position.y)
                ta->position.y -= depth.y / ta->scale.y;
            else
                ta->position.y += depth.y / ta->scale.y;
        }
        if (pb->flags & DYNAMIC)
        {
            pb->speed = (vec2s){pb->speed.x, -pb->speed.y};
            if (tb->position.y < ta->position.y)
                tb->position.y -= depth.y / tb->scale.y;
            else
                tb->position.y += depth.y / tb->scale.y;
        } 
    }
}

void screenVerticalDivisionsCollision(TransformComponent* transforms, PhysicsComponent* physics, int count, int screenWidth, int screenHeight)
{

    EntityID* verticalDivisions[DIVISIONS];
    int currentDivisionIndex[DIVISIONS] = {0};
    for (int i = 0; i < DIVISIONS; i++)
    {
        verticalDivisions[i] = malloc(sizeof(EntityID) * count);
    }

    // ----------------------- Broad Phase ---------------------------

    int widthPerDivision = screenWidth / DIVISIONS;
    for (int i = 0; i < count; i++)
    {
        TransformComponent t = transforms[i];
        PhysicsComponent p = physics[i];
        float leftEdge = (t.position.x - 0.5) * t.scale.x;
        float rightEdge = (t.position.x + 0.5) * t.scale.x;

        if ((p.flags & ACTIVE) && (rightEdge < screenWidth/2 && leftEdge > -screenWidth/2))
        {
            int divX1 = leftEdge / widthPerDivision; // left edge
            divX1 = (divX1 + DIVISIONS/2)-1; // so that 0 < divX1 < DIVISONS-1
            int divX2 = rightEdge / widthPerDivision; // right edge
            divX2 = (divX2 + DIVISIONS/2)-1; 

            int index = currentDivisionIndex[divX1];
            verticalDivisions[divX1][index] = i;
            currentDivisionIndex[divX1]++;

            if (divX1 != divX2)
            {
                int index = currentDivisionIndex[divX2];
                verticalDivisions[divX2][index] = i;
                currentDivisionIndex[divX2]++;
            }
        }
    }

    // -----------------------------------------------------------------------

    // -------------------------- Narrow Phase -------------------------------

    for (int div = 0; div < DIVISIONS; div++)
    {
        for (int i = 0; i < currentDivisionIndex[div]; i++)
        {   
            for (int j = i+1; j < currentDivisionIndex[div]; j++)
            {
                EntityID e1 = verticalDivisions[div][i];
                EntityID e2 = verticalDivisions[div][j];

                if (!(physics[e1].flags & DYNAMIC) && !(physics[e2].flags & DYNAMIC))
                    continue;

                struct AABB a = createAABB(e1);
                struct AABB b = createAABB(e2);
                int c = checkCollisionAABB(a, b);
                if (c)
                {   
                    handleCollision(a, b);
                }
            }
        }
    }

    // -----------------------------------------------------------------------------

    for (int i = 0; i < DIVISIONS; i++)
    {
        free(verticalDivisions[i]);
    }     
}