#ifndef TUI_H
#define TUI_H
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <termios.h>


typedef struct {
    int width;
    int height;
} Display_t;

typedef struct {
    int x;
    int y;
} Cursor_t;

typedef struct {
    int width;
    int height;
    int x;
    int y;
    char* title;
} Window_t;

typedef struct {
    char** strings;
    int quantity;
} Sliced_t;

typedef struct {
    void (*action)(int buttonId, Cursor_t* cursor);
    int buttonId;
    char* text;
    int x;
    int y;
} Button_t;

Display_t initDisplay(int width, int height);
Cursor_t initCursor();
Window_t initWindow(Display_t* display);
Button_t initButton(int id, char* text, int x, int y);

void renderWindow(Display_t* Display_t, Cursor_t* cursor, Window_t* window);
void renderWindowShadow(int width, int height, Display_t* display, Cursor_t* cursor);
void renderTitlebar(Window_t* window, Cursor_t* cursor, int gap);
void writeTextToWindow(char* text, int width, int height, Display_t* display, Cursor_t* cursor);
void renderButton(Button_t* button, Cursor_t* cursor);

void shiftCursor(int x, int y, Cursor_t* cursor);

//Utility function prototypes
Sliced_t sliceStringIntoRows(char* str, int width);

#endif