#include "eventDispatcher.h"

extern void onEvent(EventHolder* event);

void dispatchEvent(EventHolder* event)
{
    onEvent(event);
}
