#include "renderer.h"

#include "vao.h"
#include "buffer.h"
#include "shader.h"
#include "texture.h"
#include "gfx.h"
#include "camera.h"

#include "entity/entity.h"

#include "stdlib.h"
#include "log/log.h"

#include "cglm/struct.h"

void prepareRenderer();
void rendererSetProjectionMatrix(mat4s proj);

#define MAX_QUADS 16
static const unsigned int maxVertices = MAX_QUADS * 4;
static const unsigned int maxIndices  = MAX_QUADS * 6;
struct QuadVertex
{
    vec3s pos;
    vec3s color;
    vec2s uv;
    float texIndex;
};

typedef struct _renderer
{
    enum ShaderType type;
    Shader currentShader;
    Shader shaders[MAX_SHADER+1];

    Vao vao;
    Vbo vbo;
    Ibo ibo;

    vec3s vertexPositions[4];
    vec2s texturesCoords[4];
    struct QuadVertex* vertices;
    struct QuadVertex* vertexPtrCurrent;

    Texture* textures[16];
    unsigned int currentTexture;

    unsigned int indexCount;
    unsigned int quadCount;

    unsigned int renderCalls;
} Renderer;

static Renderer r;

void createRenderer()
{
    LOG_TRACE("Starting the renderer\n");
    r.shaders[basicShader] = createShader("vertex.glsl", "fragment.glsl");

    r.shaders[uvShader]    = createShader("texV.glsl", "texF.glsl");
    shaderSetTextureSlot(r.shaders[uvShader], "u_texture");

    r.currentShader = r.shaders[basicShader];
    r.type = basicShader;

    r.vertices = malloc(sizeof(struct QuadVertex) * maxVertices);
    if (r.vertices == NULL)
        LOG_ERROR("Failed memory allocation\n");

    r.vertexPtrCurrent = r.vertices;
    r.indexCount = 0;
    r.quadCount = 0;

    r.vao = createVao();
    bindVao(r.vao);

    r.vbo = createVbo(sizeof(struct QuadVertex) * maxVertices);
    addAttribute(&(r.vao), 3, sizeof(struct QuadVertex)); // position
    addAttribute(&(r.vao), 3, sizeof(struct QuadVertex)); // color
    addAttribute(&(r.vao), 2, sizeof(struct QuadVertex)); // uv coords;
    addAttribute(&(r.vao), 1, sizeof(struct QuadVertex)); // texIndex;

    unsigned int* indices = malloc(sizeof(unsigned int) * maxIndices);
    if (indices == NULL)
        LOG_ERROR("Failed memory allocation\n");

    unsigned int offset = 0;
    int i;
    for (i = 0; i < maxIndices; i += 6)
    {
        indices[i]   = 0 + offset;
        indices[i+1] = 1 + offset;
        indices[i+2] = 2 + offset;

        indices[i+3] = 2 + offset;
        indices[i+4] = 3 + offset;
        indices[i+5] = 0 + offset;
        offset += 4;
    }

    r.ibo = createIbo(maxIndices, indices);
    free(indices);

    r.vertexPositions[0] = (vec3s){-0.5f, -0.5f, 0.0f}; // bottom left
    r.vertexPositions[1] = (vec3s){ 0.5f, -0.5f, 0.0f}; // bottom right
    r.vertexPositions[2] = (vec3s){ 0.5f,  0.5f, 0.0f}; // top left
    r.vertexPositions[3] = (vec3s){-0.5f,  0.5f, 0.0f}; // top right

    r.texturesCoords[0] = (vec2s){0.0f, 0.0f};
    r.texturesCoords[1] = (vec2s){1.0f, 0.0f};
    r.texturesCoords[2] = (vec2s){1.0f, 1.0f};
    r.texturesCoords[3] = (vec2s){0.0f, 1.0f};

    r.renderCalls = 0;

    r.currentTexture = 0;
}

void destroyRenderer()
{  
    destroyShader(r.shaders[basicShader]);
    destroyShader(r.shaders[uvShader]);
    destroyVao(r.vao);
    destroyBuffer(r.vbo);
    destroyBuffer(r.ibo);
    free(r.vertices);

    for (int i = 0; i < r.currentTexture; i++)
    {
        if (r.textures[i])
            unloadTexture(r.textures[i]);
    }
}

void _renderBatch()
{
    if (r.indexCount == 0)
        return;

    for (int i = 0; i < r.currentTexture; i++)
        bindTexture(r.textures[i]);

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

void startFrame()
{
    prepareRenderer();
    r.renderCalls = 0;

    shaderSetUniformMat4(r.shaders[basicShader], camera.projview, "projview");
    shaderSetUniformMat4(r.shaders[uvShader],    camera.projview, "projview");

    useShader(r.currentShader);

    startBatch();
}

void flush()
{
    unsigned int size = (unsigned int)((unsigned char*) r.vertexPtrCurrent - (unsigned char*) r.vertices);
    pushBufferData(r.vbo, size, r.vertices);

    _renderBatch();
}

void endFrame()
{   
    flush();
    LOG_INFO_DEBUG("Render calls: %i\n", r.renderCalls);
}

// void _render(struct Vao vao, Ibo indexBuffer)
// {
//     bindVao(vao);
//     glDrawElements(GL_TRIANGLES, indexBuffer.count, GL_UNSIGNED_INT, 0);
// }

void _pushQuad(mat4s transform, vec3s color, float texIndex)
{
    if(r.quadCount >= MAX_QUADS)
    {
        flush();
        startBatch();
    }

    for (int i = 0; i < 4; i++)
    {
        vec3s vertex = glms_mat4_mulv3(transform, r.vertexPositions[i], 1);
        r.vertexPtrCurrent->pos = vertex;
        r.vertexPtrCurrent->color = color;
        r.vertexPtrCurrent->uv = r.texturesCoords[i];
        r.vertexPtrCurrent->texIndex = texIndex;
        r.vertexPtrCurrent++;
    }
    r.indexCount += 6;
    r.quadCount  += 1;
}

void render()
{
    SpriteComponent* sprites = registerView(sprite);
    TransformComponent* transforms = registerView(transform);
    unsigned int count = getEntityCount();
    for (int i = 0; i < count; i++)
    {   
        if (sprites[i].render)
        {
            mat4s m;
            vec3s scale = {transforms[i].scale.x, transforms[i].scale.y, 1};
            m = glms_mat4_identity();
            m = glms_scale(m, scale);
            m = glms_translate(m, transforms[i].position);
            m = glms_rotate(m, transforms[i].rotation, (vec3s){0, 0, 1});
            _pushQuad(m, sprites[i].color, sprites[i].texIndex);
        }
    }
}

/**
 *      ----------------------------------------------
 *      --------------- Renderer Utils ---------------
 *      ----------------------------------------------
*/

void onTextureLoad(Texture* texture)
{
    r.textures[r.currentTexture++] = texture;
}

void rendererChangeMode()
{
    static int mode = 0;
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL * mode + GL_LINE * (1-mode));
    mode = !mode;
}

void rendererSetShader(enum ShaderType type)
{
    r.currentShader = r.shaders[type];
    r.type = type;

    useShader(r.currentShader);
}

void prepareRenderer()
{
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}