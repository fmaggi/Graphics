#ifndef SCRIPT_H
#define SCRIPT_H

#include "engine.h"

class MyLayer : public Module
{
public:
    void OnAttach(uint32_t width, uint32_t height) override;
    void OnDetach() override;

    void OnUpdate(float ts) override;
    void OnRender() override;
    void OnRenderUI() override;

    template<typename E>
    bool OnEvent(E event);
private:
    TextureID t;
    EntityID player;
    bool renderUI = true;
    Camera camera;
};

template<>
bool MyLayer::OnEvent<KeyPressed>(KeyPressed event);

template<>
bool MyLayer::OnEvent<MouseMoved>(MouseMoved event);

template<>
bool MyLayer::OnEvent<MouseScrolled>(MouseScrolled event);

template<>
bool MyLayer::OnEvent<WindowResize>(WindowResize event);

#endif
