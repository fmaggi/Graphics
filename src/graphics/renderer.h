#ifndef RENDERER_H
#define RENDERER_H

#include "glm/glm.hpp"

#include "vertexArray.h"
#include "shader.h"
#include "camera.h"
#include "texture.h"

#define MAX_SHADER 2
enum ShaderType
{
    basicShader = 0,
    uvShader
};


namespace Renderer {

    void Init();
    void Destroy();

    void PushQuad(glm::vec3 translation, float rotation, glm::vec2 scale, glm::vec4 color, TextureID textureID = NoTexture);
    void DrawIndexed(VertexBuffer* vbo, Shader* shader, uint32_t count);

    void StartFrame(Camera& camera);
    void EndFrame();

    void ChangeMode();
    void SetShader(enum ShaderType type);

    void SetViewport(int width, int height);
    void SetClearColor(float r, float g, float b, float a);
    void PrepareRenderer();

};

#endif
