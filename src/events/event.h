#ifndef EVENT_H
#define EVENT_H

enum EventType
{
    WindowClose = 0, WindowResize,
    KeyPressed, KeyReleased,
    MousePressed, MouseReleased, MouseMoved, MouseScrolled,
};

typedef struct event
{
    void* instance;
    enum EventType type;
} EventHolder;

typedef struct keyEvent
{
    int key, scancode, mods, repeat;
} KeyEvent;

typedef struct mouseButton
{
    int button, action, mods;
} MouseButtonEvent;

typedef struct mouseMoved
{
    float x, y;
} MouseMovedEvent;

typedef struct scroll
{
    float xoffset, yoffset;
} MouseScrollEvent;

typedef struct windowClose
{
} WindowCloseEvent;

typedef struct windowResize
{
    float width, height;
} WindowResizeEvent;

#endif
