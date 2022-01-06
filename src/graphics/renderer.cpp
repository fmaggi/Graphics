#include "renderer.h"

#include "gfx.h"
#include "log/log.h"

#include "stdlib.h"
#include "glm/glm.hpp"

namespace Renderer {

    void PrepareRenderer();
    void RendererSetProjectionMatrix(glm::mat4 proj);

    #define MAX_QUADS 16
    static constexpr uint32_t maxVertices = MAX_QUADS * 4;
    static constexpr uint32_t maxIndices  = MAX_QUADS * 6;

    struct QuadVertex
    {
        glm::vec3 pos;
        float rotation;
        glm::vec2 scale;
        glm::vec2 base;
        glm::vec4 color;
        glm::vec2 uv;
        float texIndex;
    };

    struct vertexData
    {
        glm::vec2 vertexPositions[4];
        glm::vec2 texturesCoords[4];
        QuadVertex* vertices;
        QuadVertex* vertexPtrCurrent;
    };

    struct shaderData
    {
        ShaderType type;
        Shader* currentShader;
        Shader* shaders[MAX_SHADER];
    };

    struct rendererData
    {
        VertexArray* vao;
        VertexBuffer* vbo;
        IndexBuffer* ibo;

        uint32_t indexCount = 0;
        uint32_t quadCount = 0;

        uint32_t renderCalls = 0;

        Texture* textures[16] = {0};
        uint32_t currentTexture = 0;

        Camera* camera = nullptr;
    };

    static rendererData r;
    static vertexData v;
    static shaderData s;

    void Init()
    {
        LOG_TRACE("Starting the renderer");
        s.shaders[basicShader] = new Shader("vertex.glsl", "fragment.glsl");
        s.shaders[uvShader]    = new Shader("texV.glsl", "texF.glsl");

        s.currentShader = s.shaders[basicShader];
        s.type = basicShader;

        v.vertices = new QuadVertex[maxVertices];
        ASSERT(v.vertices, "Renderer: Failed memory allocation");
        v.vertexPtrCurrent = v.vertices;

        r.indexCount = 0;
        r.quadCount = 0;

        r.vao = new VertexArray();

        r.vbo = new VertexBuffer(sizeof(QuadVertex), maxVertices);
        r.vbo->AddAttribute(3); // translation
        r.vbo->AddAttribute(1); // rotation
        r.vbo->AddAttribute(2); // scale
        r.vbo->AddAttribute(2); // base
        r.vbo->AddAttribute(4); // color
        r.vbo->AddAttribute(2); // uv coords;
        r.vbo->AddAttribute(1); // texIndex;

        uint32_t* indices = new uint32_t[maxIndices];
        ASSERT(indices, "Renderer: Failed memory allocation");

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
        delete[] indices;

        v.vertexPositions[0] = {-0.5f, -0.5f}; // bottom left
        v.vertexPositions[1] = { 0.5f, -0.5f}; // bottom right
        v.vertexPositions[2] = { 0.5f,  0.5f}; // top right
        v.vertexPositions[3] = {-0.5f,  0.5f}; // top left

        v.texturesCoords[0] = {0.0f, 0.0f};
        v.texturesCoords[1] = {1.0f, 0.0f};
        v.texturesCoords[2] = {1.0f, 1.0f};
        v.texturesCoords[3] = {0.0f, 1.0f};
    }

    void Destroy()
    {
        delete s.shaders[basicShader];
        delete s.shaders[uvShader];
        delete r.vao;
        delete r.vbo;
        delete r.ibo;
        delete[] v.vertices;

        for (int i = 0; i < r.currentTexture; i++)
        {
            Texture* t = r.textures[i];
            t->Destroy();
        }
    }

    void RenderBatch()
    {
        if (r.indexCount == 0)
            return;

        r.vao->Bind();
        glDrawElements(GL_TRIANGLES, r.indexCount, GL_UNSIGNED_INT, 0);
        r.renderCalls++;
    }

    void StartBatch()
    {
        v.vertexPtrCurrent = v.vertices;
        r.indexCount = 0;
        r.quadCount = 0;
    }

    void StartFrame(Camera& camera)
    {
        r.renderCalls = 0;
        r.camera = &camera;

        s.shaders[basicShader]->SetData("projview", camera.ViewProjectionMatrix);
        s.shaders[uvShader]->SetData("projview", camera.ViewProjectionMatrix);

        s.currentShader->Bind();

        StartBatch();

    }

    void Flush()
    {
        uint32_t size = (uint32_t)((unsigned char*) v.vertexPtrCurrent - (unsigned char*) v.vertices);
        r.vbo->PushData(size, v.vertices);

        RenderBatch();
    }

    void EndFrame()
    {
        Flush();
        LOG_INFO_DEBUG("Render calls: %i", r.renderCalls);
    }

    void PushQuad(glm::vec3 translation, float rotation, glm::vec2 scale, glm::vec4 color, TextureID textureID)
    {
        if(r.quadCount >= MAX_QUADS)
        {
            Flush();
            StartBatch();
        }

        float left = translation.x - scale.x/2;
        float right = translation.x + scale.x/2;
        float top = translation.y + scale.y/2;
        float bottom = translation.y - scale.y/2;

        if (!r.camera->InFrustum(left, right, top, bottom))
        {
            return;
        }

        for (int i = 0; i < 4; i++)
        {
            v.vertexPtrCurrent->pos = translation;
            v.vertexPtrCurrent->rotation = rotation;
            v.vertexPtrCurrent->scale = scale;
            v.vertexPtrCurrent->base = v.vertexPositions[i];
            v.vertexPtrCurrent->color = color;
            v.vertexPtrCurrent->uv = v.texturesCoords[i];
            v.vertexPtrCurrent->texIndex = (float) textureID;
            v.vertexPtrCurrent++;
        }
        r.indexCount += 6;
        r.quadCount  += 1;
    }

    void DrawIndexed(VertexBuffer* vbo, Shader* shader, uint32_t count)
    {
        shader->Bind();
        vbo->Bind();

        glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, 0);
    }

    /**
     *      ----------------------------------------------
     *      --------------- Renderer Utils ---------------
     *      ----------------------------------------------
    */

    void ChangeMode()
    {
        static int mode = 0;
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL * mode + GL_LINE * (1-mode));
        mode = !mode;
    }

    void SetShader(enum ShaderType type)
    {
        s.currentShader = s.shaders[type];
        s.type = type;

        s.currentShader->Bind();
    }

    void PrepareRenderer()
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void SetViewport(int width, int height)
    {
        glViewport(0, 0, width, height);
    }

    void SetClearColor(float r, float g, float b, float a)
    {
        glClearColor(r, g, b, a);
    }

};

using namespace Renderer;
void onTextureLoad(Texture* texture)
{
    r.textures[r.currentTexture++] = texture;
}
