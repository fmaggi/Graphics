#ifndef BODY_H
#define BODY_H

#include "glm/glm.hpp"

enum class BodyType
{
    Static = 0, Dynamic
};

class Physics;

class Body
{
public:
    glm::vec3 translation;
    glm::vec2 speed, impulse;

    uint32_t userFlags;
    void* userData;

    void AddAABB(float halfWidth, float halfHeight);
    void (*onCollision)(Body* self, Body* other);
private:
    Body(){}
    BodyType type;
    int32_t aabbID;

    friend Physics;
};

typedef void (*CollisionCallback)(Body* self, Body* other);

struct BodyDef
{
    BodyType type = BodyType::Static;
    glm::vec3 translation{0,0,0};

    uint32_t userFlags = 0;
    void* userData = nullptr;
    CollisionCallback onCollision = nullptr;
};

#endif
