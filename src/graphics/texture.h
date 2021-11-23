#ifndef TEXTURE_H
#define TEXTURE_H

#include <string>

#define NO_TEXTURE -1

typedef uint32_t TextureID;

class Texture
{
public:
    static TextureID CreateTexture(const std::string& name = "");
    ~Texture();

    void Bind();

private:
    uint32_t m_ID, m_slot;
    Texture(){}
};

#endif
