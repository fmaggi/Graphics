#include "world/world.h"
#include "log/log.h"

void init(World* w)
{
    LOG_TRACE("Creating World\n");

    Entity player;
    player.pos = (vec3s){0, 0, 0};

    float vertices[] = {
        200.0f,  200.0f, 0.0f, 0.2f, 0.4f, 0.8f, // top right
        200.0f, 0.0f, 0.0f,  0.7f, 0.2, 0.3f,// bottom right
        0.0f, 0.0f, 0.0f,  0.3f, 0.8f, 0.3f,// bottom left
        0.0f,  200.0f, 0.0f, 0.6f, 0.8f, 0.8f// top left 
    };
    unsigned int indices[] = {
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

    addAttribute(&(player.vao), 3, 6*sizeof(float));
    addAttribute(&(player.vao), 3, 6*sizeof(float));

    w->player = player;
}

WorldFuncPtr initWorld = &init;

void destroy(World* w)
{
}

WorldFuncPtr destroyWorld = &destroy;