#ifndef WINDOW_H
#define WINDOW_H

#include "events/event.h"

typedef void (*EventDispatchFunc)(EventHolder* event);

void createWindow(int width, int height, const char* title, EventDispatchFunc callbackFunc);
void destroyWindow();

int isKeyPressed(int key);
int isMouseButtonPressed(int button);

void updateWindow();

#endif // WINDOW_H