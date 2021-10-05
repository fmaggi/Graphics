#ifndef WINDOW_H
#define WINDOW_h

#include "gfx.h"

#include "events/event.h"
#include "cglm/struct.h"

typedef void (*EventDispatchFunc)(EventHolder* event);
typedef struct _window Window;

Window* createWindow(int width, int height, const char* title, EventDispatchFunc callbackFunc);
void destroyWindow();

GLFWwindow* getNativeWindow();
mat4s getProjectionMatrix();

void prepareWindow();

#endif // WINDOW_H