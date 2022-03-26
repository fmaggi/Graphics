#ifndef GAME_H
#define GAME_H

#include <string>
#include <vector>

#include "module.h"
#include "timestep.h"

#include "events/event.h"
#include "events/eventSystem.h"

class Application
{
public:
    static Application* Create(uint32_t width, uint32_t height, const std::string& name);

    void Run();

    template<typename T>
    void LoadModule()
    {
        Module* m = new T;
        m->OnAttach(m_width, m_height, &eventSystem);
        m_modules.push_back(m);
    }

private:
    Application() = default;
    Application(Application&) = delete;
    Application(Application&&) = delete;

    void Destroy();
    void OnUpdate(float ts);
    void OnRender();

    bool OnWindowClose(WindowClose event);
    bool OnWindowResize(WindowResize event);

private:
    EventSystem eventSystem;

    TimeStep ts;

    bool isRunning = false;
    std::vector<Module*> m_modules;
    float m_width, m_height;
    std::string name;

    static inline Application* app = nullptr;
};

#endif
