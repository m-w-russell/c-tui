#ifndef TUI_H
#define TUI_H
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct {
    int width;
    int height;
} Display_t;

typedef struct {
    int x;
    int y;
} Cursor_t;

typedef struct {
    char** strings;
    int quantity;
} Sliced_t;

Display_t initDisplay(int width, int height);
Cursor_t initCursor();

void renderWindow(Display_t* Display_t, Cursor_t* cursor);
void renderWindowShadow(int width, int height, Display_t* display, Cursor_t* cursor);
void writeTextToWindow(char* text, int width, int height, Display_t* siplay, Cursor_t* cursor);
Sliced_t sliceStringIntoRows(char* str, int width);
void shiftCursor(int x, int y, Cursor_t* cursor);

#endif