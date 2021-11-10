#include "eventDispatcher.h"

extern void onEvent(void* event, enum EventType type);

void dispatchEvent(void* event, enum EventType type)
{
    onEvent(event, type);
}
