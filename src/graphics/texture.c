#include "texture.h"

#include "gfx.h"

#include "log/log.h"
#include "string.h"
#include <unistd.h>


#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

typedef struct texture
{
    unsigned int id;
    unsigned int slot;
} Texture;

extern void onTextureLoad(Texture* texture);

int loadTexture(const char* name)
{
    static unsigned int usedSlot = 0;
    Texture* self = malloc(sizeof(Texture));
    if (self == NULL)
    {
        LOG_WARN("Failed to allocate memory for texture\n");
        return -1;
    }
    self->slot = usedSlot++;

    char path[512];
    getcwd(path, sizeof(path));
    strcat(path, "/res/textures/");
    strcat(path, name);

    stbi_set_flip_vertically_on_load(1);  

    glActiveTexture(GL_TEXTURE0 + self->slot);
    glGenTextures(1, &self->id);
    glBindTexture(GL_TEXTURE_2D, self->id);
    // set the texture wrapping/filtering options (on the currently bound texture object)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // load and generate the texture
    int width, height, nrChannels;
    unsigned char *data = stbi_load(path, &width, &height, &nrChannels, 0);
    if (data == NULL)
    {   
        LOG_ERROR("Failed to load texture %s: %s\n", name, stbi_failure_reason());
        --usedSlot;
        return -1;
    }

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(data);

    onTextureLoad(self);
    return self->slot;
}

void bindTexture(Texture* t)
{
    glActiveTexture(GL_TEXTURE0 + t->slot);
    glBindTexture(GL_TEXTURE_2D, t->id);
}

void unloadTexture(Texture* t)
{
    glDeleteTextures(1, &t->id);
    free(t);
}