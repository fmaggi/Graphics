#ifndef WINDOW_H
#define WINDOW_H

#include <string>

class Window
{
public:
    static void Create(uint32_t width, uint32_t height, const std::string& title);
    static void Destroy();

    static bool IsKeyPressed(int key);
    static bool IsMouseButtonPressed(int button);
    static void GetCursorPos(double* x, double* y);

    static void Update();

    static void* GetNativeWindow();
};

#endif // WINDOW_H
