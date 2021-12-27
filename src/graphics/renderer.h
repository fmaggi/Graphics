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


namespace Renderer {

   void Init();
   void Destroy();

   void PushQuad(glm::vec3 translation, float rotation, glm::vec2 scale, glm::vec4 color, TextureID textureID = NoTexture);

   void StartFrame(Camera* camera);
   void EndFrame();

   void ChangeMode();
   void SetShader(enum ShaderType type);

   void SetViewport(int width, int height);

};

#endif
