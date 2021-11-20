#ifndef TEXTURE_H
#define TEXTURE_H

#include <string>

#define NO_TEXTURE -1

class Texture
{
public:
    Texture(const std::string& name);
    ~Texture();

    void Bind();

    inline uint32_t GetID() { return m_slot;}

private:
    uint32_t m_ID, m_slot;
};

#endif
