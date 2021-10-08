#include "c_sprite.h"
#include "register.h"

#include "log/log.h"
#include "string.h"

void Sprite_init(unsigned int size)
{
    LOG_TRACE("Setting up sprite component\n");
    void* temp = malloc(sizeof(SpriteComponent) * size);
    if (temp == NULL)
    {
        LOG_ERROR("Memory allocation error\n");
        exit(-1);
    }
    memset(temp, 0, sizeof(temp));
    registers.sprites = temp;
}

void addSpriteComponent(unsigned int id, SpriteComponent c)
{
    c.render = 1;
    registers.sprites[id] = c;
}

void* getSpriteComponent(unsigned int id)
{
    return &registers.sprites[id];
}