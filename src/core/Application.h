#ifndef GAME_H
#define GAME_H

#include <string>
#include "module.h"

class Application
{
public:
    static Application* Create(uint32_t width, uint32_t height, const std::string& name);

    void Run();

    void LoadModule(Module* module);

private:
    Application() = default;
    Application(Application&) = delete;
    Application(Application&&) = delete;

    void Destroy();
    void OnUpdate(float ts);
    void OnRender();

    bool isRunning = false;
    Module* mainModule = nullptr;
    float m_width, m_height;
    std::string name;
};

#endif
