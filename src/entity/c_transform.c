#include "c_transform.h"
#include "register.h"

#include "log/log.h"

#include "string.h"

void Transform_init(unsigned int size)
{
    LOG_TRACE("Setting up transform component\n");
    void* temp = malloc(sizeof(TransformComponent) * size);
    if (temp == NULL)
    {
        LOG_ERROR("Memory allocation error\n");
        exit(-1);
    }
    memset(temp, 0, sizeof(temp));
    registers.transforms = temp;
}

void addTransformComponent(unsigned int id, TransformComponent c)
{
    registers.transforms[id] = c;
}

void* getTransformComponent(unsigned int id)
{
    return &registers.transforms[id];
}