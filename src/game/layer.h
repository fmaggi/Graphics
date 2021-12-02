#ifndef LAYER_H
#define LAYER_H

#include <string>

class Layer
{
public:
    static void OnAttach(uint32_t width, uint32_t height, const std::string& title);
    static void OnDetach();

    static void OnUpdate(float ts);
    static void OnRender();

    static void OnRenderUI();

    // template specializations need to be defined by te user and registered to the EventHandler
    template<typename T>
    static bool OnEvent(T event)
    {
        return false;
    }
};

#endif
