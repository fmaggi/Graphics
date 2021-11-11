#include "eventDispatcher.h"

extern void onEvent(struct Event event);

void dispatchEvent(struct Event event)
{
    onEvent(event);
}
