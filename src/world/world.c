#include "world.h"
#include "log/log.h"
#include "stdlib.h"

World* emptyWorld()
{
    World* self = malloc(sizeof(World));
    if (self == NULL)
    {
        LOG_ERROR("Failed to initialize world\n");
        exit(-1);
    }
    self->count = 0;
    return self;
}