#ifndef MAIN_H
#define MAIN_H

#include "game/layer.h"

class Test : public Layer
{
public:
    Test()
        : Layer(1200, 800, "test")
    {}
    void OnAttach() override;
    void OnDetach() override;

    void OnUpdate(float ts) override;
    void OnRender() override;
};

#endif
