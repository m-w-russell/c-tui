#include "tui.h"
#include <stdio.h>

Display_t initDisplay(int width, int height) {
    Display_t newDisplay;
    newDisplay.width = width;
    newDisplay.height = height;
    return newDisplay;
}

Cursor_t initCursor() {
    Cursor_t newCursor;
    newCursor.x = 0;
    newCursor.y = 0;
    return newCursor;
}

void renderWindow(Display_t* Display_t, Cursor_t* cursor) {
    int windowWidth = Display_t->width - 2;
    int windowHeight = Display_t->height - 2;
    int gap = (Display_t->width - windowWidth) / 2;
    printf("Gap %d\n", gap);
    printf("\n");
    for (int i = 0; i < Display_t->height; i++) {
        for (int j = 0; j < Display_t->width; j++) {
            if (j >= gap*2 && j <= windowWidth-1 && i >= gap && i <= windowHeight) {
                printf("\e[0;37m\u2588");
            } else {
                printf("\e[0;31m\u2588");
            }
            cursor->x++;
            
        }

        cursor->y++;
        printf("\n");
        cursor->x = 0;
    }
    cursor->y++;
    printf("\n");
}

void shiftCursor(int x, int y, Cursor_t* cursor) {
    int shiftX = x - cursor->x;
    int shiftY = y - cursor->y;
    if (shiftX < 0) {
        printf("\e[%dD", -shiftX);
    } else {
        printf("\e[%dC", shiftX);
    }

    if (shiftY < 0) {
        printf("\e[%dB", -shiftY);
    } else {

    }
        printf("\e[%dA", shiftY);
    
    printf("P");
}

int main() {
    Display_t disp = initDisplay(100, 20);
    Cursor_t cursor = initCursor();
    renderWindow(&disp, &cursor);
    shiftCursor(10, 40, &cursor);
}