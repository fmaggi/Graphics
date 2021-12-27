#include "core/Application.h"
#include "runtimeScript.h"

int main()
{
    auto app = Application::Create(1280, 720, "Test");
    app->LoadModule(new MyLayer);
    app->Run();
}
