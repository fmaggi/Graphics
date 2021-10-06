#include "renderer.h"

#include "vao.h"
#include "buffer.h"
#include "shader.h"
#include "gfx.h"

#include "stdlib.h"
#include "log/log.h"

#include "cglm/struct.h"


#define MAX_SHADER 1
enum ShaderType
{
    basicShader
};

#define MAX_QUADS 16
#define maxVertices MAX_QUADS * 4
#define maxIndices  MAX_QUADS * 6
struct QuadVertex
{
    vec3 pos;
    vec3 color;
};

typedef struct _renderer
{
    enum ShaderType type;
    Shader* shaders[MAX_SHADER];
    Shader* currentShader;

    struct Vao vao;
    Vbo vbo;
    Ibo ibo;

    vec3s vertexPositions[4];
    struct QuadVertex vertices[maxVertices];
    struct QuadVertex* vertexPtrBase;
    struct QuadVertex* vertexPtrCurrent;

    unsigned int indexCount;
    unsigned int quadCount;

    unsigned int renderCalls;
} Renderer;

static Renderer r;

void createRenderer()
{
    r.shaders[basicShader] = createShader("vertex.glsl", "fragment.glsl");
    r.currentShader = r.shaders[basicShader];
    r.type = basicShader;

    r.vertexPtrBase = r.vertexPtrCurrent = r.vertices;
    r.indexCount = 0;
    r.quadCount = 0;

    r.vao = createVao();
    bindVao(r.vao);

    r.vbo = createVbo(sizeof(struct QuadVertex) * 4 * MAX_QUADS);
    addAttribute(&(r.vao), 3, 6*sizeof(float)); // position
    addAttribute(&(r.vao), 3, 6*sizeof(float)); // color

    unsigned int* indices = malloc(sizeof(unsigned int) * maxIndices);

    unsigned int offset = 0;
    int i;
    for (i = 0; i < maxIndices; i += 6)
    {
        indices[i]   = 0 + offset;
        indices[i+1] = 1 + offset;
        indices[i+2] = 3 + offset;
        indices[i+3] = 1 + offset;
        indices[i+4] = 2 + offset;
        indices[i+5] = 3 + offset;
        offset += 4;
    }

    r.ibo = createIbo(maxIndices, indices);
    free(indices);

    r.vertexPositions[0] = (vec3s){1.0f, 1.0f, 0.0f,};
    r.vertexPositions[1] = (vec3s){1.0f, 0.0f, 0.0f,}; 
    r.vertexPositions[2] = (vec3s){0.0f, 0.0f, 0.0f,};
    r.vertexPositions[3] = (vec3s){0.0f, 1.0f, 0.0f,};
    r.renderCalls = 0;
}

void destroyRenderer()
{  
    destroyShader(r.shaders[basicShader]);
    destroyVao(r.vao);
    destroyBuffer(r.vbo);
    destroyBuffer(r.ibo);
}

void _renderBatch()
{
    //shaderSetUniformMat4(r.currentShader, r.camera->view, "view");
    bindVao(r.vao);
    glDrawElements(GL_TRIANGLES, r.indexCount, GL_UNSIGNED_INT, 0);
    r.renderCalls++;
}

void startBatch()
{
    r.vertexPtrCurrent = r.vertices;
    r.indexCount = 0;
    r.quadCount = 0;
}

void rendererSetProjectionMatrix(mat4s proj)
{
    for (int i = 0; i < MAX_SHADER; i++)
    { 
        useShader(r.shaders[i]);
        shaderSetUniformMat4(r.shaders[i], proj, "projview");
    }

    useShader(r.currentShader);
}

void startScene(Camera* c)
{
    r.renderCalls = 0;
    rendererSetProjectionMatrix(c->projview);
    startBatch();
}

void flush()
{
    unsigned int size = (unsigned int)((unsigned char*) r.vertexPtrCurrent - (unsigned char*) r.vertexPtrBase);
    pushBufferData(r.vbo, size, r.vertices);

    _renderBatch();
}

void endScene()
{   
    flush();
    LOG_INFO_DEBUG("Render calls: %i\n", r.renderCalls);
}

void _render(struct Vao vao, Ibo indexBuffer)
{
    bindVao(vao);
    glDrawElements(GL_TRIANGLES, indexBuffer.count, GL_UNSIGNED_INT, 0);
}

void _drawQuad(mat4s transform, vec3 color)
{
    if(r.quadCount >= MAX_QUADS)
    {
        flush();
        startBatch();
    }

    for (int i = 0; i < 4; i++)
    {
        vec3s vertex = glms_mat4_mulv3(transform, r.vertexPositions[i], 1);
        glm_vec3_copy(vertex.raw, r.vertexPtrCurrent->pos);
        glm_vec3_copy(color, r.vertexPtrCurrent->color);
        r.vertexPtrCurrent++;
    }
    r.indexCount += 6;
    r.quadCount  += 1;
}

void _pushEntity(Entity* e)
{
    mat4s m;
    m = glms_mat4_identity();
    vec3s scale = {200, 200, 200};
    m = glms_scale(m, scale);
    m = glms_translate(m, e->pos);

    _drawQuad(m, e->color.raw);
}

void render(World* w)
{
    _pushEntity(&w->player2);
    _pushEntity(&w->player);
    for (int i = 0; i < w->index; i++)
        _pushEntity(&(w->entities[i]));
}

static int mode;
void rendererChangeMode()
{
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL * mode + GL_LINE * (1-mode));
    mode = !mode;
}


unsigned int rendererUseShader(enum ShaderType type)
{
    r.currentShader = r.shaders[type];
    r.type = basicShader;

    useShader(r.currentShader);

}

void prepareRenderer()
{
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
}