#include "world/world.h"
#include "log/log.h"

void init(World* w)
{
    LOG_TRACE("Creating World\n");

    Entity player;
    player.pos = (vec3s){0, 0, 0};

    float vertices[] = {
        200.0f,  200.0f, 0.0f,  // top right
        200.0f, 0.0f, 0.0f,  // bottom right
        0.0f, 0.0f, 0.0f,  // bottom left
        0.0f,  200.0f, 0.0f   // top left 
    };
    unsigned int indices[] = {  // note that we start from 0!
        0, 1, 3,   // first triangle
        1, 2, 3    // second triangle
    }; 

    player.vao = createVao();
    player.vbo = createVbo();
    player.ibo = createIbo();

    bindVao(player.vao);

    bindBuffer(player.vbo);
    addDataToBuffer(&(player.vbo), sizeof(vertices), vertices);

    bindBuffer(player.ibo);
    addDataToBuffer(&(player.ibo), sizeof(indices), indices);

    addAttribute(&(player.vao), player.vbo, 3, 3*sizeof(float));

    w->entities[w->index++] = player;
}

WorldFuncPtr initWorld = &init;

void destroy(World* w)
{
    for (int i = 0; i < w->index; i++)
    {
        destroyVao(w->entities[i].vao);
        destroyBuffer(&(w->entities[i].vbo));
        destroyBuffer(&(w->entities[i].ibo));
    }

    free(w);
}

WorldFuncPtr destroyWorld = &destroy;