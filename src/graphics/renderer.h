#ifndef RENDERER_H
#define RENDERER_H

#include "glm/glm.hpp"

#include "camera.h"
#include "texture.h"

#define MAX_SHADER 2
enum ShaderType
{
    basicShader = 0,
    uvShader
};

class Renderer
{
public:
   static void Init();
   static void Destroy();

   static void PushQuad(glm::vec3 translation, float rotation, glm::vec2 scale, glm::vec3 color, TextureID texture = NoTexture);

   static void StartFrame(Camera& camera);
   static void EndFrame();

   static void ChangeMode();
   static void SetShader(enum ShaderType type);

   static void SetViewport(int width, int height);
};

#endif
