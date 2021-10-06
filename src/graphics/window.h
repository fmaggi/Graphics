#ifndef WINDOW_H
#define WINDOW_h

#include "events/event.h"

typedef void (*EventDispatchFunc)(EventHolder* event);

void createWindow(int width, int height, const char* title, EventDispatchFunc callbackFunc);
void destroyWindow();

int isKeyPressed(unsigned int key);

void updateWindow();

#endif // WINDOW_H