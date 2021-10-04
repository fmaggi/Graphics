#include "renderer.h"

#include "shader.h"
#include "gfx.h"

#include "stdlib.h"
#include "log/log.h"

typedef struct _renderer
{
    enum ShaderType type;
    Shader* shaders[MAX_SHADER];
    Shader* currentShader;
} Renderer;

Renderer* createRenderer()
{
    Renderer* self = malloc(sizeof(Renderer));
    if (self == NULL)
    {
        LOG_ERROR("Failed to create renderer");
        exit(-1);
    }
    self->shaders[basicShader] = createShader("vertex.glsl", "fragment.glsl");
    self->currentShader = self->shaders[basicShader];
    self->type = basicShader;
    return self;
}

void destroyRenderer(Renderer* r)
{  
    destroyShader(r->shaders[basicShader]);
    free(r);
}

void _render(struct Vao vao, Ibo indexBuffer)
{
    bindVao(vao);
    glDrawElements(GL_TRIANGLES, indexBuffer.count, GL_UNSIGNED_INT, 0);
}

void _renderEntity(Entity* e)
{
    _render(e->vao, e->ibo);
}

void render(World* w)
{
    int maxEntity = w->index;
    for (int i = 0; i < maxEntity; i++)
        _renderEntity(&(w->entities[i]));
}

static int mode;
void rendererChangeMode()
{
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL * mode + GL_LINE * (1-mode));
    mode = !mode;
}

void rendererUseShader(Renderer* r, enum ShaderType type)
{
    r->currentShader = r->shaders[type];
    r->type = basicShader;

    useShader(r->currentShader);
}

void prepareRenderer()
{
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
}