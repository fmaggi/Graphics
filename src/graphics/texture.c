#include "texture.h"

#include "gfx.h"

#include "log/log.h"
#include "string.h"
#include <unistd.h>


#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

static unsigned int usedSlot = 0;

Texture loadTexture(const char* name)
{
    Texture self;
    self.slot = usedSlot++;

    char path[512];
    getcwd(path, sizeof(path));
    strcat(path, "/res/textures/");
    strcat(path, name);

    stbi_set_flip_vertically_on_load(1);  

    glGenTextures(1, &self.id);
    glActiveTexture(GL_TEXTURE0 + self.slot);
    glBindTexture(GL_TEXTURE_2D, self.id);
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
        return self;
    }

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(data);
    return self;
}

void bindTexture(Texture t)
{
    glActiveTexture(GL_TEXTURE0 + t.slot);
    glBindTexture(GL_TEXTURE_2D, t.id);
}

void unloadTexture(Texture t)
{
    glDeleteTextures(1, &t.id);
}