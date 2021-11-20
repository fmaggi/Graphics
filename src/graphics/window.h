#ifndef WINDOW_H
#define WINDOW_H

#include <string>

class Window
{
public:
    static void Create(uint32_t width, uint32_t height, const std::string& title);
    static void Destroy();

    static int IsKeyPressed(int key);
    static int IsMouseButtonPressed(int button);
    static void GetCursorPos(double* x, double* y);

    static void Update();
};

#endif // WINDOW_H
