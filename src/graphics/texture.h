#ifndef TEXTURE_H
#define TEXTURE_H

typedef struct _texture Texture;

int loadTexture(const char* name);

void bindTexture(Texture* t);

void unloadTexture(Texture* t);

#endif