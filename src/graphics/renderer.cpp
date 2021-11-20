#include "renderer.h"

#include "vao.h"
#include "buffer.h"
#include "shader.h"
#include "texture.h"
#include "gfx.h"
#include "camera.h"

#include "entity/actions.h"

#include "log/log.h"

#include "stdlib.h"
#include "glm/glm.hpp"

void prepareRenderer();
void rendererSetProjectionMatrix(glm::mat4 proj);

#define MAX_QUADS 16
static const uint32_t maxVertices = MAX_QUADS * 4;
static const uint32_t maxIndices  = MAX_QUADS * 6;
struct QuadVertex
{
    glm::vec3 pos;
    glm::vec3 color;
    glm::vec2 uv;
    float texIndex;
};

struct rendererData
{
    ShaderType type;
    Shader* currentShader;
    Shader* shaders[MAX_SHADER];

    VertexArray* vao;
    VertexBuffer* vbo;
    IndexBuffer* ibo;

    glm::vec3 vertexPositions[4];
    glm::vec2 texturesCoords[4];
    QuadVertex* vertices;
    QuadVertex* vertexPtrCurrent;

    // Texture textures[16];
    uint32_t currentTexture;

    uint32_t indexCount;
    uint32_t quadCount;

    uint32_t renderCalls;
};

static rendererData r;

void Renderer::Init()
{
    LOG_TRACE("Starting the renderer");
    r.shaders[basicShader] = createShader("vertex.glsl", "fragment.glsl");

    r.shaders[uvShader]    = createShader("texV.glsl", "texF.glsl");
    shaderSetTextureSlot(r.shaders[uvShader], "u_texture");

    r.currentShader = r.shaders[basicShader];
    r.type = basicShader;

    r.vertices = (QuadVertex*) malloc(sizeof(QuadVertex) * maxVertices);
    if (r.vertices == NULL)
        LOG_ERROR("Failed memory allocation");

    r.vertexPtrCurrent = r.vertices;
    r.indexCount = 0;
    r.quadCount = 0;

    r.vao = new VertexArray();
    r.vao->SetStride(sizeof(QuadVertex));

    r.vbo = new VertexBuffer(sizeof(QuadVertex) * maxVertices, VertexBuffer::BufferType::Dynamic);
    r.vao->AddAttribute(3); // position
    r.vao->AddAttribute(3); // color
    r.vao->AddAttribute(2); // uv coords;
    r.vao->AddAttribute(1); // texIndex;

    uint32_t* indices = (uint32_t*) malloc(sizeof(uint32_t) * maxIndices);
    if (indices == NULL)
        LOG_ERROR("Failed memory allocation");

    uint32_t offset = 0;
    for (int32_t i = 0; i < maxIndices; i += 6)
    {
        indices[i]   = 0 + offset;
        indices[i+1] = 1 + offset;
        indices[i+2] = 2 + offset;

        indices[i+3] = 2 + offset;
        indices[i+4] = 3 + offset;
        indices[i+5] = 0 + offset;
        offset += 4;
    }

    r.ibo = new IndexBuffer(maxIndices, indices);
    free(indices);

    r.vertexPositions[0] = {-0.5f, -0.5f, 0.0f}; // bottom left
    r.vertexPositions[1] = { 0.5f, -0.5f, 0.0f}; // bottom right
    r.vertexPositions[2] = { 0.5f,  0.5f, 0.0f}; // top left
    r.vertexPositions[3] = {-0.5f,  0.5f, 0.0f}; // top right

    r.texturesCoords[0] = {0.0f, 0.0f};
    r.texturesCoords[1] = {1.0f, 0.0f};
    r.texturesCoords[2] = {1.0f, 1.0f};
    r.texturesCoords[3] = {0.0f, 1.0f};

    r.renderCalls = 0;

    // currentTexture = 0;
}

void Renderer::Destroy()
{
    destroyShader(r.shaders[basicShader]);
    destroyShader(r.shaders[uvShader]);
    delete r.vao;
    delete r.vbo;
    delete r.ibo;
    free(r.vertices);
}

void renderBatch()
{
    if (r.indexCount == 0)
        return;

    r.vao->Bind();
    glDrawElements(GL_TRIANGLES, r.indexCount, GL_UNSIGNED_INT, 0);
    r.renderCalls++;
}

void startBatch()
{
    r.vertexPtrCurrent = r.vertices;
    r.indexCount = 0;
    r.quadCount = 0;
}

void Renderer::StartFrame()
{
    prepareRenderer();
    r.renderCalls = 0;

    shaderSetUniformMat4(r.shaders[basicShader], camera.projview, "projview");
    shaderSetUniformMat4(r.shaders[uvShader],    camera.projview, "projview");

    // shaderSet bind the shade Restore the shader to the original one
    useShader(r.currentShader);

    startBatch();
}

void flush()
{
    uint32_t size = (uint32_t)((unsigned char*) r.vertexPtrCurrent - (unsigned char*) r.vertices);
    r.vbo->PushData(size, r.vertices);

    renderBatch();
}

void Renderer::EndFrame()
{
    flush();
    LOG_INFO_DEBUG("Render calls: %i", r.renderCalls);
}

void Renderer::PushQuad(glm::vec3 position, float rotation, glm::vec2 scale, glm::vec3 color, Texture& texture)
{
    float left = position.x + scale.x/2;
    float right = position.x - scale.x/2;
    float top = position.y + scale.y/2;
    float bottom = position.y - scale.y/2;

    if (!inFrustum(left, right, top, bottom))
    {
        return;
    }

    glm::mat4 transform = getTransform(position, rotation, scale);

    if(r.quadCount >= MAX_QUADS)
    {
        flush();
        startBatch();
    }

    for (int i = 0; i < 4; i++)
    {
        glm::vec3 vertex = transform * glm::vec4(r.vertexPositions[i], 1);
        r.vertexPtrCurrent->pos = {vertex.x, vertex.y, vertex.z};
        r.vertexPtrCurrent->color = color;
        r.vertexPtrCurrent->uv = r.texturesCoords[i];
        r.vertexPtrCurrent->texIndex = texture.GetID();
        r.vertexPtrCurrent++;
    }
    r.indexCount += 6;
    r.quadCount  += 1;
}

/**
 *      ----------------------------------------------
 *      --------------- Renderer Utils ---------------
 *      ----------------------------------------------
*/

void onTextureLoad(Texture* texture)
{
    // textures[currentTexture++] = *texture;
}

void Renderer::ChangeMode()
{
    static int mode = 0;
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL * mode + GL_LINE * (1-mode));
    mode = !mode;
}

void Renderer::SetShader(enum ShaderType type)
{
    r.currentShader = r.shaders[type];
    type = type;

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

void Renderer::SetViewport(int width, int height)
{
    glViewport(0, 0, width, height);
}
