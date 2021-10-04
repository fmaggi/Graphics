#ifndef EVENT_H
#define EVENT_H

enum EventType
{
    WindowClose = 0, WindowResize,
    KeyPressed, KeyReleased,
    MousePressed, MouseReleased, MouseMoved, MouseScrolled,
};

typedef struct _event 
{
    void* instance;
    enum EventType type;
} EventHolder;

typedef struct _keyEvent
{
    int key, scancode, action, mods, repeat;
} KeyEvent;

KeyEvent createKeyEvent(int key, int scancode, int action, int mods);

typedef struct _mouseButton
{
    int button, action, mods;
} MouseButtonEvent;

EventHolder createMouseButtonEvent(int button, int action, int mods);
EventHolder createMouseMovedEvent();

typedef struct _windowClose
{
} WindowCloseEvent;

#endif