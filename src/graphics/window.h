#ifndef WINDOW_H
#define WINDOW_h

#include "events/event.h"
#include "cglm/struct.h"

typedef void (*EventDispatchFunc)(EventHolder* event);
typedef struct _window Window;

Window* createWindow(int width, int height, const char* title, EventDispatchFunc callbackFunc);
void destroyWindow(Window* window);

void* getNativeWindow(Window* window);
mat4s getProjectionMatrix(Window* window);

void prepareWindow(Window* window);

#endif // WINDOW_H