#ifndef WINDOW_H
#define WINDOW_h

#include "events/event.h"

typedef int (*EventDispatchFunc)(EventHolder* event);
typedef struct _window Window;

Window* createWindow(int width, int height, const char* title, EventDispatchFunc callbackFunc);
void destroyWindow(Window* window);

int shouldClose(Window* window);

void prepare(Window* window);

#endif // WINDOW_H