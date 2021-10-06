#ifndef WINDOW_H
#define WINDOW_h

#include "gfx.h"

#include "events/event.h"


typedef void (*EventDispatchFunc)(EventHolder* event);
typedef struct _window Window;

Window* createWindow(int width, int height, const char* title, EventDispatchFunc callbackFunc);
void destroyWindow();

GLFWwindow* getNativeWindow();

void prepareWindow();

#endif // WINDOW_H