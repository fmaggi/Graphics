#ifndef TEXTURE_H
#define TEXTURE_H

#define NO_TEXTURE -1

typedef struct texture Texture;

int loadTexture(const char* name);

void bindTexture(Texture* t);

void unloadTexture(Texture* t);

#endif