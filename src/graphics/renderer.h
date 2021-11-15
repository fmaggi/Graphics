#ifndef RENDERER_H
#define RENDERER_H

#include "glm/glm.hpp"

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

   static void PushQuad(glm::vec3 position, float rotation, glm::vec2 scale, glm::vec3 color, float texIndex);

   static void StartFrame();
   static void EndFrame();

   static void ChangeMode();
   static void SetShader(enum ShaderType type);

   static void SetViewport(int width, int height);
private:

};

#endif
