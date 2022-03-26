#include "core/Application.h"
#include "runtimeScript.h"

#include "mandelbrot.h"

int main()
{
    auto app = Application::Create(1280, 720, "Test");
    app->LoadModule<MyLayer>();
    app->Run();
    delete app;
}
