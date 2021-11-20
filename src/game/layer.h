#ifndef LAYER_H
#define LAYER_H

#include "events/event.h"
#include <string>

class Layer
{
public:
    Layer(uint32_t width, uint32_t height, const std::string& title)
        : m_width(width), m_height(height), m_title(title)
    {};

    virtual void OnAttach() = 0;
    virtual void OnDetach() = 0;

    virtual void OnUpdate(float ts) = 0;
    virtual void OnRender() = 0;

    virtual void OnEvent(Event& event) = 0;

    uint32_t m_width, m_height;
    std::string m_title;
};

#endif
