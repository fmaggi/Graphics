#ifndef LAYER_H
#define LAYER_H

#include <string>

class Module
{
public:
    virtual void OnAttach(uint32_t width, uint32_t height) = 0;
    virtual void OnDetach() = 0;

    virtual void OnUpdate(float ts) {}
    virtual void OnRender() {}

    virtual void OnRenderUI() {}

    // OnEvent functions need to be defined by te user and registered to the EventHandler
    /*
    template<typename T>
    bool OnEvent(T event)
    {
        Handle event;
    }
    */
protected:
   uint32_t m_width, m_height;
   std::string m_title;
};

#endif
