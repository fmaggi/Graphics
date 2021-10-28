#ifndef WINDOW_H
#define WINDOW_H

void createWindow(int width, int height, const char* title);
void destroyWindow();

int windowIsKeyPressed(int key);
int windowIsMouseButtonPressed(int button);

void updateWindow();

#endif // WINDOW_H
