#ifndef TEXTURE_H
#define TEXTURE_H

typedef struct _texture
{
    unsigned int id;
    unsigned int slot;
} Texture;

typedef void (*TextureLoadCallback)(Texture*);
extern TextureLoadCallback textureCallback;

int loadTexture(const char* name);

void bindTexture(Texture* t);

void unloadTexture(Texture* t);

#endif