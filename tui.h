#ifndef TUI_H
#define TUI_H

typedef struct {
    int width;
    int height;
} Display_t;

typedef struct {
    int x;
    int y;
} Cursor_t;

Display_t initDisplay(int width, int height);
Cursor_t initCursor();

void renderWindow(Display_t* Display_t, Cursor_t* cursor);

void shiftCursor(int x, int y, Cursor_t* cursor);

#endif