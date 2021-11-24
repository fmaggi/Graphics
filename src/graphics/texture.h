#ifndef TEXTURE_H
#define TEXTURE_H

#include <string>

typedef int32_t TextureID;
static const TextureID NoTexture = -1;

class Texture
{
public:
    static TextureID Create(const std::string& name = "");

    void Bind();
    void Destroy();
private:
    uint32_t m_ID, m_slot;
    Texture();
};


#endif
