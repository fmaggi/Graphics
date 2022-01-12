#ifndef WINDOW_H
#define WINDOW_H

#include <string>
#include "events/eventSystem.h"

namespace Window {

    void Create(uint32_t width, uint32_t height, const std::string& title, EventSystem* eventSystem, bool vsync = true);
    void Destroy();

    bool IsKeyPressed(int key);
    bool IsMouseButtonPressed(int button);
    void GetCursorPos(double* x, double* y);

    void Update();

    void* GetNativeWindow();

};

#endif // WINDOW_H
