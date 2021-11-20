#include "texture.h"

#include "gfx.h"

#include "log/log.h"
#include "string.h"
#include <unistd.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

static uint32_t usedSlot = 0;

extern void onTextureLoad(Texture* texture);

Texture::Texture(const std::string& name)
{
    if (name.empty())
    {
        m_slot = -1;
        m_ID = -1;
        return;
    }
    m_slot = usedSlot++;

    char path[512];
    getcwd(path, sizeof(path));
    strcat(path, "/res/textures/");
    strcat(path, name.c_str());

    stbi_set_flip_vertically_on_load(1);

    // load and generate the texture
    int width, height, nrChannels;
    unsigned char *data = stbi_load(path, &width, &height, &nrChannels, STBI_rgb_alpha);
    if (data == NULL)
    {
        LOG_ERROR("Failed to load texture %s: %s", name, stbi_failure_reason());
        --usedSlot;
        m_slot = -1;
    }

    glActiveTexture(GL_TEXTURE0 + m_slot);
    glGenTextures(1, &m_ID);
    glBindTexture(GL_TEXTURE_2D, m_ID);
    // set the texture wrapping/filtering options (on the currently bound texture object)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(data);

    // onTextureLoad(this);
}

Texture::~Texture()
{
    glDeleteTextures(1, &m_ID);
}

void Texture::Bind()
{
    glActiveTexture(GL_TEXTURE0 + m_slot);
    glBindTexture(GL_TEXTURE_2D, m_ID);
}

